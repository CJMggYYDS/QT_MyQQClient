#include "regedit.h"
#include "ui_regedit.h"
#include<QMessageBox>
#include<QIcon>

regedit::regedit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::regedit)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/image/qq.png"));
    this->setWindowTitle("My QQChart");
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/image/bg4.jfif")));
    this->setPalette(palette);
    socket=new QTcpSocket;

    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(showresult()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    setWindowTitle("注册");
}

regedit::~regedit()
{
    delete ui;
}

//注册操作
void regedit::on_regeditbutton_clicked()
{
    //连接服务器
    QHostAddress receiver_ip("127.0.0.1");
    socket->abort();
    socket->connectToHost(receiver_ip, 8888);
    QString pw1, pw2, username;
    pw1 = ui->passwordline ->text();
    pw2 = ui->cpasswordline ->text();
    username = ui->usernameline->text();
    if(username == ""){
        QMessageBox::warning(this, tr("警告"),tr("请确保输入了用户名！"));
        return;
    }
    if(pw1 == ""){
        QMessageBox::warning(this, tr("警告"),tr("请确保输入了密码！"));
        return;
    }
    if(pw2 == ""){
        QMessageBox::warning(this, tr("警告"),tr("请确保输入了确认密码！"));
        return;
    }
    if(pw1 == pw2){
        //输入格式正确，向服务器发起注册请求
        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (int i = 0; i < ipAddressesList.size(); ++i) {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                    ipAddressesList.at(i).toIPv4Address())
                ipAddress = ipAddressesList.at(i).toString();
        }
        // if we did not find one, use IPv4 localhost
        if (ipAddress.isEmpty())
            ipAddress = QHostAddress(QHostAddress::LocalHost).toString();


        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_4);
        QString port = "8888";
        out<<quint8('R')<<ui->usernameline->text()<<ui->passwordline->text()<<ipAddress<<port;  //发送注册信息
        socket->write(block);
    }
    else{
        QMessageBox::warning(this, tr("错误"),tr("密码与确认密码不同，请重新输入！"));

        ui->passwordline ->setText("");
        ui->cpasswordline->setText("");
    }

}

//取消
void regedit::on_cancelbutton_clicked()
{
    socket->close();
    regedit::close();
}

//反馈结果
void regedit::showresult(){
    QByteArray datagram;
    datagram = socket->readAll();
    QDataStream in(&datagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_4);
    quint8 resulttype;
    in>>resulttype;
    switch(resulttype){
    case 'S':           //注册成功
        QMessageBox::information(this,tr("My QQChart"), tr("欢迎使用 My QQChart\n"
                                                          "请记住你的密码，以便登录使用！"));
        socket->close();
        regedit::close();
        break;
    case 'D':           //数据库打开失败
        QMessageBox::information(this,tr("My QQChart ERROR"), tr("数据库错误，请稍后重试！"));
        socket->close();
        break;
    case 'F':            //重复注册
        QString username;
        in >> username;
        QMessageBox::information(this,tr("My QQChart ERROR"), tr("%1 已存在\n 请重新注册！").arg(username));
        ui->usernameline->setText("");
        ui->passwordline ->setText("");
        ui->cpasswordline->setText("");
        socket->close();
        break;
    }
}

//服务器断开连接
void regedit::connectionClosedByServer(){
    socket->close();
}

//错误异常分析
void regedit::error(QAbstractSocket::SocketError socketError)
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
                                    "Make sure the MyMsn server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("My QQChart ERROR"),
                                 tr("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    }
}

