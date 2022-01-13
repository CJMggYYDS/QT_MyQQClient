#include "qqlist.h"
#include "ui_qqlist.h"
#include"tcpclient.h"
#include<QtGui/QCloseEvent>
#include<QInputDialog>
#include<QIcon>

qqlist::qqlist(QString& ipaddress, QString& username):
    ui(new Ui::qqlist)
{
    ui->setupUi(this);
    ui->menubar->hide();
    this->setWindowIcon(QIcon(":/image/qq.png"));
    this->setWindowTitle(username);
    ui->userlabel->setText(username);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/image/bg6.jfif")));
    this->setPalette(palette);
    ipAddress = ipaddress;
    user = username;
    tcpsocket = new QTcpSocket;
    tcpsocket->abort();
    tcpsocket->connectToHost(ipAddress, 8888);        //与服务器连接

    connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(processPendingMsg()));
}

qqlist::~qqlist()
{
    delete ui;
}

//接受信息处理
void qqlist::processPendingMsg()
{
    QByteArray datagram;
    datagram = tcpsocket->readAll();
    quint8 type;
    QString user;
    QDataStream in(&datagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_4);
    in >> type;                 //服务器发来的信号
    switch(type){
    case 'G':{     //有在线好友，则在复选框内显示
         ui->friendlist->clear();
         while(1){
             in >> user;
             if(user == "over"){
                 break;
             }
             QListWidgetItem*item=new QListWidgetItem(ui->friendlist);
             item->setText(user);
             ui->friendlist->addItem(item);
         }
         break;
     }
     case 'M':{     //私聊发送的信息
          QString msgfrom;
          QString content;
          QString msgto;
          in >> msgfrom >> msgto >> content;
          int chatIndex = getIndexOfChat(msgfrom);
          chatlist[chatIndex]->MsgArrived(type, msgfrom, content);
          chatlist[chatIndex]->show();
          break;
     }
    case 'N':{     //群发的信息
        QString msgfrom;
        QString content;
        in >> msgfrom >> content;
        int chatIndex = getIndexOfChat(msgfrom);
        chatlist[chatIndex]->MsgArrived(type, msgfrom, content);
        chatlist[chatIndex]->show();
        break;
     }
   }
}

//获得聊天窗口的下标并建立聊天窗口
int qqlist::getIndexOfChat(const QString& peerUserName)
{
    int chatWinIndex = 0;
    for(; chatWinIndex < chatlist.size(); ++chatWinIndex)
    {
         if (chatlist[chatWinIndex]->user == peerUserName)
         {
               break;
         }
    }
    if (chatWinIndex == chatlist.size() ) // 若改窗口不存在，则创建
    {
         chat* pChatWin = new chat(peerUserName, user, ipAddress);
         chatlist.push_back(pChatWin);
    }
    return chatWinIndex;

}

//关闭
void qqlist::closeEvent(QCloseEvent *event)
{
    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);;
    out.setVersion(QDataStream::Qt_4_4);
    out <<quint8('Q')<<user;       //向服务器发送退出信息
    tcpsocket->write(datagram);
    event->accept();
}

//打开聊天界面窗口
void qqlist::on_friendlist_doubleClicked(const QModelIndex &index)
{
    if ( ui->friendlist->currentRow() >= 0 &&ui->friendlist->currentRow() <  ui->friendlist->count() )
    {
         int nCurrentRow = ui->friendlist->currentRow();
         QString peerUserName = ui->friendlist->item(nCurrentRow)->text();

         int chatWinIndex = getIndexOfChat(peerUserName);
         chatlist[chatWinIndex]->show();
         chatlist[chatWinIndex]->setFocus();
    }
}

//添加好友
void qqlist::on_addfriend_clicked()
{
    bool ok;
    QInputDialog*d=new QInputDialog(this);
    d->setMinimumSize(219,107);
    QString username1 = d->getText(this,tr("添加好友"),tr("请输入用户名: "),QLineEdit::Normal,tr("username"),&ok);
    if(username1!=""&&ok)
    {
        QByteArray datagram;
        QDataStream out(&datagram, QIODevice::WriteOnly);;
        out.setVersion(QDataStream::Qt_4_4);
        out <<quint8('A')<<username1<<user;      //向服务器发送添加好友的信息
        tcpsocket->write(datagram);
    }
}

//好友复选框
void qqlist::on_friend_2_activated(int index)
{
    if(index==0){
        QByteArray datagram;
        QDataStream out(&datagram, QIODevice::WriteOnly);;
        out.setVersion(QDataStream::Qt_4_4);
        out <<quint8('O')<<user;          //发送上线信息
        tcpsocket->write(datagram);
    }
    else if(index==1){
        QByteArray datagram;
        QDataStream out(&datagram, QIODevice::WriteOnly);;
        out.setVersion(QDataStream::Qt_4_4);
        out <<quint8('U')<<user;          //发送离线信息
        tcpsocket->write(datagram);
    }

}
