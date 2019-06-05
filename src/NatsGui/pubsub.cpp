#include "pubsub.h"
#include "ui_pubsub.h"

Pubsub::Pubsub(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pubsub)
{
    ui->setupUi(this);
    ui->_horizontalLayout->setStretch(0,1);
    ui->_horizontalLayout->setStretch(1,4);
    ui->_horizontalLayout->setStretch(2,1);
    ui->_horizontalLayout->setStretch(3,1);
    ui->_horizontalLayout->setStretch(4,4);
    ui->_horizontalLayout->setStretch(5,10);
    ui->_horizontalLayout->setStretch(6,1);
    ui->_horizontalLayout->setStretch(7,1);
    ui->_horizontalLayout->setStretch(8,1);
    ui->_splitter->setStretchFactor(0,3);
    ui->_splitter->setStretchFactor(1,1);

    ui->_plainTextEdit->installEventFilter(this);

    m_bIsConnect = false;
    m_sNatsInfo.clear();
    QFont font;
    font.setPointSize(10);
    ui->_textBrowser->setFont(font);

    QObject::connect(&m_natsClient,
                     &Nats::Client::error, [this](const QString & error) {
        m_sNatsInfo = error;
        m_bIsConnect = false;
    });
}

Pubsub::~Pubsub()
{
    m_natsClient.unsubscribe(m_ullSid);
    m_natsClient.disconnect();
    m_bIsConnect = false;
    delete ui;
}

void Pubsub::on__pushButton1_clicked()
{
    m_natsClient.unsubscribe(m_ullSid);
    m_natsClient.disconnect();
    m_bIsConnect = false;
}

void Pubsub::on__pushButton2_clicked()
{
    ui->_textBrowser->clear();
}

void Pubsub::onSend() {
    m_sMsgInfo = ui->_plainTextEdit->toPlainText().trimmed();
    if(m_sMsgInfo.isEmpty()) {
        m_sNatsInfo = "错误,Nats发送消息为空!";
        return;
    }

    if(!connectNats()) {
        QMessageBox::about(this, "提示", m_sNatsInfo);
        return;
    }

    QString ntsSendTopic = ui->_sendLineEdit->text().trimmed();
    if(ntsSendTopic.isEmpty()) {
        m_sNatsInfo = "错误,Nats发送主题为空";
        QMessageBox::about(this, "提示", m_sNatsInfo);
        return;
    }

    m_natsClient.publish(ntsSendTopic, m_sMsgInfo);
    ui->_textBrowser->append(QString("Send: %1").arg(m_sMsgInfo));
    ui->_plainTextEdit->clear();
}

bool Pubsub::connectNats() {

    if(m_bIsConnect) {
        m_sNatsInfo = "已成功连接Nats集群";
        return true;
    }

    QString natsLableName;
    QString natsAddr;
    QString natsPort;
    QString natsUser;
    QString natsPasswd;
    QString natsRecvTopic = ui->_recvLineEdit->text().trimmed();

    // 初始化配置
    QString sPath = QCoreApplication::applicationDirPath() + "/" + IniFileName;
    QSettings settings(sPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    // 读取上次登入信息
    QString lastLable = settings.value("natsLastLable").toString();
    int size = settings.beginReadArray("natsinfo");
    for(int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        natsLableName = settings.value("natsLableName").toString().trimmed();
        if(natsLableName == lastLable) {
            natsAddr = settings.value("natsAddr").toString().trimmed();
            natsPort = settings.value("natsPort").toString().trimmed();
            natsUser = settings.value("natsUser").toString().trimmed();
            natsPasswd = settings.value("natsPasswd").toString().trimmed();
            break;
        }
    }
    settings.endArray();

    if(natsAddr.isEmpty()) {
        m_sNatsInfo = "错误,Nats连接地址未配置";
        return m_bIsConnect;
    }
    if(natsPort.isEmpty()) {
        m_sNatsInfo = "错误,Nats连接端口未配置";
        return m_bIsConnect;
    }

    m_natsClient.setdebugmode(false);
    m_natsOptions.user = natsUser;
    m_natsOptions.pass = natsPasswd;
    if(!m_natsClient.connectSync(natsAddr, natsPort.toUShort(), m_natsOptions)) {
        return m_bIsConnect;
    }

    if(!natsRecvTopic.isEmpty()) {
        m_ullSid = m_natsClient.subscribe(natsRecvTopic,
                                          [this](QString message,
                                          QString reply_inbox,
                                          QString subject) {
                ui->_textBrowser->append(QString("Recv: %1").arg(message));
                subject = reply_inbox; // 消除未使用警告
    });
    }

    m_bIsConnect = true;
    return m_bIsConnect;
}

bool Pubsub::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->_plainTextEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            if(k->key() == Qt::Key_Return)
            {
                onSend();
                return true;
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

void Pubsub::on__textBrowser_textChanged()
{
    ui->_textBrowser->moveCursor(QTextCursor::End);
}
