#include "chat.h"
#include "ui_chat.h"
#include<QMessageBox>
#include"tcpclient.h"
#include<QIcon>
#include<QFont>
#include<QColorDialog>
#include<QToolButton>

chat::chat(const QString& PeerUserName, const QString& User, const QString& IpAddress) :
    ui(new Ui::chat)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/image/qq.png"));
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/image/bg6.jfif")));
    this->setPalette(palette);
    ipAddress=IpAddress;
    user = PeerUserName;
    use = User;
    QString str = "聊天对象："+user;
    ui->friendlabel->setText(str);
    setWindowTitle(use);


    connect(ui->message,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(curFmtChanged(const QTextCharFormat)));

}

chat::~chat()
{
    delete ui;
}

void chat::MsgArrived(quint8 type, const QString &from, const QString &content)
{
    QString nameWithTime;
    if(type == 'M'||type == 'N'){
        nameWithTime="【" + from + "】"+" " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        ui->hismessage->append(nameWithTime);
        ui->hismessage->append(content);
        ui->message->setFocus();
    }
}

void chat::on_sendbutton_clicked()
{
    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    QHostAddress receiver_ip(ipAddress);
    QString nameWithtime;
    //QString history_msg;
    if(ui->message->toPlainText() == QString("")){
        QMessageBox::warning(this, "MyQQChart", QString("message is empty! "));
        return;
    }

    context =ui->message->toHtml();
    nameWithtime="【" + use + "】"+" " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    out.setVersion(QDataStream::Qt_4_4);
    out << quint8('M') << use << user << context;
    tcpsocket->write(datagram);

    ui->hismessage->append(nameWithtime);
    ui->hismessage->append(context);
    ui->message->setPlainText("");
    ui->message->setFocus();
}

void chat::on_sendtoallbutton_clicked()
{
    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    QHostAddress receiver_ip(ipAddress);
    QString nameWithtime;
    //QString history_msg;
    if(ui->message->toPlainText() == QString("")){
        QMessageBox::warning(this, "MyQQChart", QString("message is empty! "));
        return;
    }
    context =ui->message->toHtml();
    nameWithtime="【" + use + "】"+" " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    out.setVersion(QDataStream::Qt_4_4);
    out << quint8('N') << use << context;
    tcpsocket->write(datagram);
    ui->hismessage->append(nameWithtime);
    ui->hismessage->append(context);
    ui->message->setPlainText("");
    ui->message->setFocus();
}

void chat::on_fontbox_currentFontChanged(const QFont &f)
{
    ui->message->setCurrentFont(f);
    ui->message->setFocus();
}

void chat::on_sizebox_currentIndexChanged(const QString &arg1)
{
    ui->message->setFontPointSize(arg1.toDouble());
    ui->message->setFocus();
}

void chat::on_colorbutton_clicked()
{
    color=QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        ui->message->setTextColor(color);
        ui->message->setFocus();
    }
}

void chat::curFmtChanged(const QTextCharFormat &fmt)
{
    ui->fontbox->setCurrentFont(fmt.font());
    if(fmt.fontPointSize()<8)
    {
        ui->sizebox->setCurrentIndex(4);
    }
    else
    {
        ui->sizebox->setCurrentIndex(ui->sizebox->findText(QString::number(fmt.fontPointSize())));
    }
    color=fmt.foreground().color();
}
