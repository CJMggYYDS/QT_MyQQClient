#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include<QTcpSocket>
#include<QtNetwork>
#include"regedit.h"
#include"qqlist.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
private slots:
    void on_loginbutton_clicked();         //登录

    void on_regeditbutton_clicked();       //注册

    void showresult();                     //反馈登录结果

    void connectionClosed();               //链接关闭

    void error(QAbstractSocket::SocketError socketError);    //异常错误分析

private:
    Ui::login *ui;
    QTcpSocket *socket;          //用于与服务器链接的套接字
};

#endif // LOGIN_H
