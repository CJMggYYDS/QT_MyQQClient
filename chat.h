#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include<QtNetwork>
#include<QFile>
#include<QProgressDialog>
#include<QTextCharFormat>
#include<QFileDialog>

namespace Ui {
class chat;
}

class chat : public QWidget
{
    Q_OBJECT

public:
    explicit chat(const QString& PeerUserName, const QString& User, const QString& IpAddress);//通过用户名，聊天对象用户名以及IP地址来创建聊天窗口
    ~chat();
    void MsgArrived(quint8 type, const QString& from, const QString& content);//对收到的信息进行处理

    QString user;         //聊天对象
    QString use;          //用户
    QString ipAddress;    //聊天对象的ip地址

private slots:
    void on_sendbutton_clicked();        //发送信息

    void on_sendtoallbutton_clicked();    //群发信息

    void on_fontbox_currentFontChanged(const QFont &f);      //更改字体

    void on_sizebox_currentIndexChanged(const QString &arg1);     //更改字体大小

    void on_colorbutton_clicked();                      //更改颜色

    void curFmtChanged(const QTextCharFormat &fmt);     //格式变化

private:
    Ui::chat *ui;

    QString context;          //用来存储发送消息

    QColor color;             //更改字体颜色时需要的QColor对象

};

#endif // CHAT_H
