#include "logindialog.h"
#include "ui_logindialog.h"
#include "qdebug.h"
#include "TCPClient.h"
#include <qmessagebox.h>
#include "fshost.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    qDebug() << "LoginDialog starting";
    ui->setupUi(this);
    ui->frmSplash->hide();
    this->connect(tcp_client, SIGNAL(authenticated(QVariantMap)), this, SLOT(onAuthenticated(QVariantMap)));
    this->connect(tcp_client, SIGNAL(authenticateError(QString)), this, SLOT(onAuthenticateError(QString)));
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
    QSettings settings;
    QString host = settings.value("General/trainer_server").toString();
    int port = settings.value("trainer_server_port").toInt();
    port = port == 0 ? 7000 : port;

    qDebug() << host << ": " << port;
    ui->frmSplash->setGeometry( ui->frmLogin->geometry());
    ui->frmSplash->show();
    tcp_client->connectToHost(host, port);

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

    _abort = false;

    // wait socket
    int i;
    for(i=0; i<200 && (!tcp_client->isConnected()); i++) {
        if(_abort) return;
        QApplication::processEvents();
        switch_sleep(100000);
    }

    if(i == 200) {
        // error socket connect
        QMessageBox::critical( this, "Idapted Trainer Studio",
                                  "Login Error, please try again!" );
        ui->frmSplash->hide();
        return;

    }


    //else connected
    ui->lbProgress->setText("Socket connected, authenticating...");
    ui->lbProgress->repaint();

    tcp_client->write(json);

    for(i=0; i<200 && (!_authenticated); i++) {
        if(_abort) return;
        QApplication::processEvents();
        switch_sleep(100000);
    }
    if(i == 200) {
        QMessageBox::critical( this, "Idapted Trainer Studio",
                               QString("Authenticate Error:\nReason: NO RESPONSE!"));
        ui->frmSplash->hide();
        return;
    }

    ui->lbProgress->setText("Waiting FreeSWITCH to be loaded...");
    ui->lbProgress->repaint();

    for(i=0; i<200 && (!fshost->isRunning()); i++ ){
        if(_abort) return;
        QApplication::processEvents();
        switch_sleep(100000);
    }
    if(i == 200) {
        QMessageBox::critical( this, "Idapted Trainer Studio",
                               QString("Error Loading FreeSWITCH\nReason: NO RESPONSE!"));
        ui->frmSplash->hide();
        return;
    }
//    emit Login();
}

void LoginDialog::on_cancelLogin_clicked()
{
    qDebug() << "Cancel";
    tcp_client->close();
    ui->frmSplash->hide();
}

void LoginDialog::onAuthenticated(QVariantMap map)
{
    qDebug() << "Authenticated";
    user = map;
    _authenticated = true;
    hide();
}

void LoginDialog::onAuthenticateError(QString reason)
{
    QMessageBox::critical( this, "Idapted Trainer Studio",
                           QString("Authenticate Error:\nReason: %1").arg(reason));
    ui->frmSplash->hide();
}
