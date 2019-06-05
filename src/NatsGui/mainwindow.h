#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "setnatsinfo.h"
#include "pubsub.h"

// 主窗口
class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = nullptr);

  signals:

  private:
    // 读取窗口位置大小信息
    void readSettings();
    // 创建菜单和工具条
    void createMenu();
    // 状态条
    void createStatusBar();
    QMenu *infoMenu;

  private slots:

    // 关于系统
    void about();
    // 保存窗口大小位置信息
    void writeSettings();
    // 退出系统
    void exit();
    // 设置查询用户NATS集群信息
    void setNatsInfo();
    // 清理连接配置
    void clearNatsInfo();
};

#endif // MAINWINDOW_H
