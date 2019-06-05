#include "setnatsinfo.h"

SetNatsInfo::SetNatsInfo(QWidget *parent) :
    QDialog(parent) {
    setWindowTitle("设置NATS信息");

    QLabel *labelUserName = new QLabel("用  户 ");
    QLabel *labelPassword = new QLabel("密  码 ");
    QLabel *labelPort = new QLabel("端  口 ");
    QLabel *labelHostName = new QLabel("地  址 ");
    QLabel *labelLableName = new QLabel("标  签 ");

    username = new QLineEdit;
    password = new QLineEdit;
    port = new QLineEdit;
    addr = new QLineEdit;
    lablename = new QComboBox;
    lablename->setEditable(true);

    QPushButton *OK = new QPushButton("确定");
    QPushButton *Exit = new QPushButton("取消");

    QVBoxLayout *vBox = new QVBoxLayout;
    QHBoxLayout *hBox = new QHBoxLayout;
    QGridLayout *grid = new QGridLayout;
    QGridLayout *mainLayout = new QGridLayout;

    grid->addWidget(labelUserName, 0, 0);
    grid->addWidget(labelPassword, 1, 0);
    grid->addWidget(labelPort, 2, 0);
    grid->addWidget(labelHostName, 3, 0);
    grid->addWidget(labelLableName, 4, 0);

    grid->addWidget(username, 0, 1);
    grid->addWidget(password, 1, 1);
    grid->addWidget(port, 2, 1);
    grid->addWidget(addr, 3, 1);
    grid->addWidget(lablename, 4, 1);

    hBox->addWidget(OK);
    hBox->addWidget(Exit);

    vBox->addLayout(grid);
    vBox->addWidget(new QLabel(" "));
    vBox->addLayout(hBox);

    mainLayout->addWidget(new QLabel("   "), 0, 0);
    mainLayout->addLayout(vBox, 0, 1);
    mainLayout->addWidget(new QLabel("   "), 0, 2);

    setLayout(mainLayout);

    connect(OK, SIGNAL(clicked()), this, SLOT(onOK()));
    connect(Exit, SIGNAL(clicked()), this, SLOT(onExit()));

    // 初始化配置
    QString sPath = QCoreApplication::applicationDirPath() + "/" + IniFileName;
    //settings.setPath(QSettings::IniFormat, QSettings::SystemScope, sPath);
    QSettings settings(sPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    // 读取上次登入信息
    QString lastLable = settings.value("natsLastLable").toString();
    int lastIndex = 0;
    int size = settings.beginReadArray("natsinfo");
    natsInfoMap.clear();
    for(int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        NatsInfo natsInfo;
        natsInfo.sLablename = settings.value("natsLableName").toString().trimmed();
        natsInfo.sAddr = settings.value("natsAddr").toString().trimmed();
        natsInfo.sPort = settings.value("natsPort").toString().trimmed();
        natsInfo.sUsername = settings.value("natsUser").toString().trimmed();
        natsInfo.sPassword = settings.value("natsPasswd").toString().trimmed();
        if(natsInfo.sLablename == lastLable)
            lastIndex = i;
        natsInfoMap.insert(natsInfo.sLablename, natsInfo);
        lablename->addItem(natsInfo.sLablename);
    }
    settings.endArray();

    connect(lablename, static_cast<void(QComboBox::*)(int)>
            (&QComboBox::currentIndexChanged),
            [ = ](int index) {
        QString sPath = QCoreApplication::applicationDirPath() + "/" + IniFileName;
        //settings.setPath(QSettings::IniFormat, QSettings::SystemScope, sPath);
        QSettings settings(sPath, QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        settings.beginReadArray("natsinfo");
        settings.setArrayIndex(index);
        username->setText(settings.value("natsUser").toString());
        password->setText(settings.value("natsPasswd").toString());
        port->setText(settings.value("natsPort").toString());
        addr->setText(settings.value("natsAddr").toString());
        lablename->setCurrentText(settings.value("natsLableName").toString());
        settings.endArray();
    }
    );
    // 发信号触发一次曹函数来初始化
    emit lablename->currentIndexChanged(lastIndex);
}

void SetNatsInfo::onOK() {

    // 成功时保存连接信息
    QString sPath = QCoreApplication::applicationDirPath() + "/" + IniFileName;
    //settings.setPath(QSettings::IniFormat, QSettings::SystemScope, sPath);
    QSettings settings(sPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    NatsInfo natsInfo;
    natsInfo.sAddr = addr->text().trimmed();
    natsInfo.sPort = port->text().trimmed();
    natsInfo.sUsername = username->text().trimmed();
    natsInfo.sPassword = password->text().trimmed();
    natsInfo.sLablename = lablename->currentText().trimmed();
    natsInfoMap.insert(natsInfo.sLablename, natsInfo);
    Global::gTagName = natsInfo.sLablename;
    // 保存上次登入信息
    settings.setValue("natsLastLable", natsInfo.sLablename);
    // 保存所有登入记录
    settings.beginWriteArray("natsinfo");
    int i = 0;
    foreach(NatsInfo value, natsInfoMap) {
        settings.setArrayIndex(i);
        i++;
        settings.setValue("natsUser", value.sUsername);
        settings.setValue("natsPasswd", value.sPassword);
        settings.setValue("natsPort", value.sPort);
        settings.setValue("natsAddr", value.sAddr);
        settings.setValue("natsLableName", value.sLablename);
    }
    settings.endArray();
    accept();
}

void SetNatsInfo::onExit() {
    reject();
}
