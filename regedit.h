#ifndef REGEDIT_H
#define REGEDIT_H

#include <QWidget>
#include<QTcpSocket>
#include<QtNetwork>

namespace Ui {
class regedit;
}

class regedit : public QWidget
{
    Q_OBJECT

public:
    explicit regedit(QWidget *parent = nullptr);
    ~regedit();
private slots:
    void on_regeditbutton_clicked();    //注册

    void on_cancelbutton_clicked();    //取消

    void showresult();                //反馈结果

    void connectionClosedByServer();   //断开连接

    void error(QAbstractSocket::SocketError socketError);   //错误异常分析



private:
    Ui::regedit *ui;
    QTcpSocket *socket;
};

#endif // REGEDIT_H
