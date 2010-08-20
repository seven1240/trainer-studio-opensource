#include "logindialog.h"
#include "ui_logindialog.h"
#include "qdebug.h"
#include "TCPClient.h"
#include <QSplashScreen>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    qDebug() << "LoginDialog starting";
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void LoginDialog::on_buttonBox_accepted()
{
    tcp_client->ConnectToHost();

    QVariantList system;

    QVariantMap info;
    info.insert("memory", 100);
    system << info;

    QVariantMap user;
    user.insert("username", ui->leUsername->text());
    user.insert("password", ui->lePassword->text());

    user.insert("action", "Authenticate");
    user.insert("system_info", system);

    QJson::Serializer serializer;

    QByteArray json = serializer.serialize(user);

    qDebug() << json;


    // wait socket
    int i;
    for(i=0; i<20 && (!tcp_client->isConnected()); i++) {
        qDebug() << tcp_client->isConnected();
        sleep(1);
    }

    if( i == 10) {
        // error socket connect
        this->show();
    } else { //connected
        tcp_client->write(json);
    }
    emit Login();

}

void LoginDialog::on_buttonBox_rejected()
{
    qDebug() << "Rejected";
    this->show();
}

void LoginDialog::on_pushButton_clicked()
{
    emit Login();
}

void LoginDialog::on_btnLogin_clicked()
{
    ui->frmSplash->setGeometry( ui->frmLogin->geometry());
}
