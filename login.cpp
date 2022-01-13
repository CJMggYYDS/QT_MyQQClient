#include "login.h"
#include "ui_login.h"
#include<QMessageBox>
#include<QIcon>

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/image/qq.png"));
    this->setWindowTitle("My QQChart");

    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/image/bg4.jfif")));
    this->setPalette(palette);

    socket = new QTcpSocket;

    connect(socket, SIGNAL(readyRead()), this, SLOT(showresult()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

}

login::~login()
{
    delete ui;
}

//登录操作
void login::on_loginbutton_clicked()
{
    //连接服务器
    QString ip_address("127.0.0.1");
    socket->abort();
    socket->connectToHost(ip_address, 8888);

    QString username,password;
    username=ui->usernameline->text();
    password=ui->passwordline->text();
    if(username == ""){
        QMessageBox::warning(this, tr("警告"),tr("请确保输入了用户名！"));
        return;
    }
    if(password == ""){
        QMessageBox::warning(this, tr("警告"),tr("请确保输入了密码！"));
        return;
    }
    //向服务器发起登录请求
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 寻找使用第一个非本地的IPv4地址
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address())
            ipAddress = ipAddressesList.at(i).toString();
    }
    // 如果找不到，就使用本地IPv4地址
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_4);
    QString port = "8888";
    out<<quint8('L')<<ui->usernameline->text()<<ui->passwordline->text()<<ipAddress<<port;//发送登录信息
    socket->write(block);

}

//注册操作
void login::on_regeditbutton_clicked()
{
    regedit* reg = new regedit;
    reg->show();
}

//关闭链接
void login::connectionClosed()
{
    socket->close();
}

//反馈结果
void login::showresult()
{
    QByteArray datagram;
    datagram = socket->readAll();
    QDataStream in(&datagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_4);
    quint8 resulttype;
    in>>resulttype;         //接收服务器发来的信号反馈
    switch(resulttype){
          case 'P':                //登录成功
          {
            socket->close();
            QString ip_address("127.0.0.1");
            QString username= ui->usernameline->text();
            qqlist* friendlist = new qqlist(ip_address, username);
            friendlist->show();
            login::close();
            break;
           }
           case 'D':           //数据库打开失败
          {
           QMessageBox::information(this,tr("My QQChart ERROR"), tr("数据库错误，请稍后重试！"));
           socket->close();
           break;
           }
           case 'E':         //密码错误
          {
           QMessageBox::information(this,tr("My QQChart ERROR"), tr("密码输入错误\n 请重试！ "));
           socket->close();
           ui->passwordline ->setText("");
           break;
           }
           case 'N':       //无法在数据库中找到要登录的用户
           {
           QString username;
           in >> username;
           QMessageBox::information(this,tr("My QQChart ERROR"), tr("%1 不存在\n 请检查用户名是否正确！").arg(username));
           ui->usernameline->setText("");
           ui->passwordline ->setText("");
           socket->close();
           break;
          }
     }
}

//错误异常分析
void login::error(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("My QQChart ERROR"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("My QQChart ERROR"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the MyQQChart Server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("My QQChart ERROR"),
                                 tr("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    }
}


