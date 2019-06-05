#ifndef SETNATSINFO_H
#define SETNATSINFO_H
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QGridLayout>
#include <QSettings>
#include <QApplication>
#include <QComboBox>
#include "define.h"

class SetNatsInfo : public QDialog {
    Q_OBJECT
public:
    explicit SetNatsInfo(QWidget *parent = nullptr);

private:
    QComboBox *lablename;
    QLineEdit *username;
    QLineEdit *password;
    QLineEdit *port;
    QLineEdit *addr;

    typedef struct NatsInfo {
        QString sLablename;
        QString sUsername;
        QString sPassword;
        QString sPort;
        QString sAddr;
    } NatsInfo;

    QMap<QString, NatsInfo> natsInfoMap;
signals:

public slots:
    void onOK();
    void onExit();
};

#endif // SETNATSINFO_H
