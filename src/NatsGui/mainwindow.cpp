#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle(WindowTitle);
    setWindowIcon(QIcon(ICON_36));
    createMenu();
    createStatusBar();
    readSettings();
}

void MainWindow::createStatusBar() {
    /* 状态条*/
    statusBar()->showMessage("Ready");
}

void MainWindow::createMenu() {

    // 开始菜单
    QMenu *beginMenu = menuBar()->addMenu("开始");

    QAction *setnatsAct = beginMenu->addAction("连接配置", this,
                                               &MainWindow::setNatsInfo);
    setnatsAct->setStatusTip("设置查询NATS集群信息");
    beginMenu->addSeparator();

    QAction *usercleaneAct = beginMenu->addAction("清理连接", this,
                                                   &MainWindow::clearNatsInfo);
    usercleaneAct->setStatusTip("清理NATS连接配置信息");
    beginMenu->addSeparator();

    QAction *saveAct = beginMenu->addAction("保存窗口", this,
                                            SLOT(writeSettings()));
    saveAct->setStatusTip("保存窗口大小位置信息...");
    beginMenu->addSeparator();

    QAction *aboutAct = beginMenu->addAction("关于系统", this, &MainWindow::about);
    aboutAct->setStatusTip("显示帮助信息...");
    beginMenu->addSeparator();

    QAction *exitAct = beginMenu->addAction("退出系统", this, SLOT(exit()));
    exitAct->setStatusTip("退出系统...");
    beginMenu->addSeparator();

    // 显示登入信息菜单
    QString sPath = QCoreApplication::applicationDirPath() + "/" + IniFileName;
    QSettings settings(sPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    infoMenu = menuBar()->addMenu("");
    infoMenu->setEnabled(false);
    infoMenu->setTitle(settings.value("natsLastLable").toString());

    /* 设置工具条 */
    QToolBar *toolBar = addToolBar("MyToolBar");
    toolBar->addSeparator();
    toolBar->addAction(setnatsAct);
    toolBar->addSeparator();
    toolBar->addSeparator();
    toolBar->addSeparator();
    toolBar->addAction(exitAct);
    toolBar->addSeparator();

    //设置图标
    saveAct->setIcon(QIcon(ICON_47));
    aboutAct->setIcon(QIcon(ICON_49));
    exitAct->setIcon(QIcon(ICON_44));

    usercleaneAct->setIcon(QIcon(ICON_103));
    setnatsAct->setIcon(QIcon(ICON_104));

    setCentralWidget(new Pubsub());
}

void MainWindow::clearNatsInfo() {
    int ret = QMessageBox::question(this, "确认", "是否要清除NATS连接信息？");
    if(ret == QMessageBox::Yes) {
        QString sPath = QCoreApplication::applicationDirPath() + "/" + IniFileName;
        //settings.setPath(QSettings::IniFormat, QSettings::SystemScope, sPath);
        QSettings settings(sPath, QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        settings.remove("natsinfo");
        QMessageBox::information(this, "提示", "成功清除NATS连接信息！");
    }
}

void MainWindow::setNatsInfo() {
    SetNatsInfo setNatsInfo;
    setNatsInfo.exec();
    infoMenu->setTitle(Global::gTagName);
}

void MainWindow::exit() {
    int ret = QMessageBox::question(this, "确认", "是否要退出系统？");
    if(ret == QMessageBox::Yes) {
        this->close();
    }
}

void MainWindow::about() {
    QMessageBox::about(this, "关于",
                       "<br>"
                       "<b>福州OCS产品部</b><br><br>"
                       "作者 ：王长春<br>"
                       "团队 ：福州OCS项目组<br>"
                       "版本 ：Version1.0业余版<br>"
                       "邮箱 ：cc20110101@126.com<br>"
                       "版权 ：Copyright ?2018 Powered By FZOCS<br>"
                       );
}

void MainWindow::readSettings() {
    QString sPath = QCoreApplication::applicationDirPath() + "/" + IniFileName;
    //settings.setPath(QSettings::IniFormat, QSettings::SystemScope, sPath);
    QSettings settings(sPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    const QByteArray geometry = settings.value("geometry",
                                               QByteArray()).toByteArray();
    if(geometry.isEmpty()) {
        this->showMaximized();
        /*
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(
                    this);
        resize(availableGeometry.width(), availableGeometry.height());
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
             */
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings() {
    QString sPath = QCoreApplication::applicationDirPath() + "/" + IniFileName;
    //settings.setPath(QSettings::IniFormat, QSettings::SystemScope, sPath);
    QSettings settings(sPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.setValue("geometry", saveGeometry());
    QMessageBox::about(this, "保存信息", "成功保存窗口大小与位置信息！");
}
