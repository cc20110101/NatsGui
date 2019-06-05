#ifndef PUBSUB_H
#define PUBSUB_H

#include "define.h"

namespace Ui {
class Pubsub;
}

class Pubsub : public QWidget
{
    Q_OBJECT

public:
    explicit Pubsub(QWidget *parent = 0);
    ~Pubsub();

protected:
    bool eventFilter(QObject *target, QEvent *event);

private:
    void onSend();
    bool connectNats();

private:
    bool m_bIsConnect;
    QString m_sNatsInfo;
    QString m_sMsgInfo;
    uint64_t m_ullSid = 0;
    Nats::Client m_natsClient;
    Nats::Options m_natsOptions;
    Ui::Pubsub *ui;


private slots:
    void on__pushButton1_clicked();
    void on__pushButton2_clicked();
    void on__textBrowser_textChanged();
};

#endif // PUBSUB_H
