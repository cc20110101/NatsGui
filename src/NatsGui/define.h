#ifndef DEFINE_H
#define DEFINE_H
#include <QString>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QtWidgets>
#include <QSettings>
#include "natsclient.h"

class Global {
  public:
    Global();
    ~Global();

  public:
    static QString gUserName;
    static QString gPassWord;
    static QString gNatsAddr;
    static QString gNatsPort;
    static QString gTagName;
};

// 定义字符串
#define WindowTitle             "NatsGui"
#define IniFileName             "conf.ini"

// 图标动画
#define ICON_36                    ":/resources/tray.ico"
#define ICON_47                    ":/resources/update.ico"
#define ICON_49                    ":/resources/about.ico"
#define ICON_44                    ":/resources/exit.ico"
#define ICON_103                   ":/resources/103.ico"
#define ICON_104                   ":/resources/104.ico"


#endif // DEFINE_H
