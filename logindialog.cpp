#include "logindialog.h"
#include "ui_logindialog.h"
#include "qdebug.h"
#include "TCPClient.h"
#include <qmessagebox.h>
#include "fshost.h"
#include "isettings.h"

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
        QMessageBox::critical( this, QApplication::applicationName(),
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
        QMessageBox::critical( this, QApplication::applicationName(),
                               QString("Authenticate Error:\nReason: NO RESPONSE!"));
        ui->frmSplash->hide();
        return;
    }

    ui->lbProgress->setText("Waiting VoIP module to be loaded...");
    ui->lbProgress->repaint();

    for(i=0; i<200 && (!fshost->isSofiaReady()); i++ ){
        if(_abort) return;
        QApplication::processEvents();
        switch_sleep(100000);
    }
    if(i == 200) {
        QMessageBox::critical( this, QApplication::applicationName(),
                               QString("Error Loading FreeSWITCH\nReason: NO RESPONSE!"));
        ui->frmSplash->hide();
        return;
    }

    //setup sip account and register
    ui->lbProgress->setText("Registering to VoIP sever...");
    ui->lbProgress->repaint();

    ISettings *isettings = new ISettings(this);

    QVariantMap gw = isettings->getGateway(QString("default"));
    QString res;

    if(gw["username"] == _user["voip_username"] && gw["password"] == _user["voip_password"]) {
        //this should not happen
        qDebug() << "No gateway info changed";
    }else{
        qDebug() << "Need to set gateway";

        QVariantMap newgw;
        newgw.insert("username", _user["voip_username"]);
        newgw.insert("password", _user["voip_password"]);
        QVariantList serverlist = _user["servers"].toList();
        QVariantMap server = serverlist[0].toMap();

        newgw.insert("realm", QString("%1:%2").arg(server["sip_proxy"].toString(), server["sip_port"].toString()));
        isettings->writeGateway(newgw);
        isettings->saveToFile();
        switch_sleep(1000000);
    }

    fshost->sendCmd("sofia", "profile softphone rescan reloadxml", &res);
    qDebug() << res;

    delete isettings;
    hide();
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
    _user = map;
    _authenticated = true;
//    hide();
}

void LoginDialog::onAuthenticateError(QString reason)
{
    QMessageBox::critical( this, QApplication::applicationName(),
                           QString("Authenticate Error:\nReason: %1").arg(reason));
    ui->frmSplash->hide();
}
