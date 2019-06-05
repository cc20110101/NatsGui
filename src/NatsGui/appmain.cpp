#include <QApplication>
#include "mainwindow.h"
#include "define.h"

// PLEASE USE UTF-8 CODE
int main(int argc, char *argv[]) {
    // 程序框架
    QApplication app(argc, argv);
    // 初始化资源
    Q_INIT_RESOURCE(icon);

    // 主窗口
    MainWindow w;
    w.show();
    return app.exec();

    exit(0);
}
