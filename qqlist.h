#ifndef QQLIST_H
#define QQLIST_H

#include <QMainWindow>
#include<QTcpSocket>
#include"chat.h"

namespace Ui {
class qqlist;
}

class qqlist : public QMainWindow
{
    Q_OBJECT

public:
    explicit qqlist(QString& ipaddress, QString& username);//通过登录的用户名及Ip地址创建列表界面
    ~qqlist();
    QList<chat*>chatlist;       //用来存放好友聊天窗口的链表

protected:
    void closeEvent(QCloseEvent *event);     //关闭列表界面

private slots:
    void processPendingMsg();    //接收服务器信息处理

    int getIndexOfChat(const QString& peerUserName);     //得到要打开的聊天窗口在链表中的下标

    void on_friend_2_activated(int index);               //打开好友复选框

    void on_addfriend_clicked();                         //添加好友

    void on_friendlist_doubleClicked(const QModelIndex &index);         //打开聊天窗口

private:
    Ui::qqlist *ui;
    QString ipAddress;      //用户ip地址
    QString user;           //用户
};

#endif // QQLIST_H
