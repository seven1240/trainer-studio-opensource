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
    ui->lePassword->setEchoMode(QLineEdit::Password);
    ui->frmSplash->hide();
    ui->frmLogin->move(QPoint(10, 10));

    this->setWindowTitle("Login");
    _authenticated = false;

    this->connect(tcp_client, SIGNAL(authenticated(QVariantMap)), this, SLOT(onAuthenticated(QVariantMap)));
    this->connect(tcp_client, SIGNAL(authenticateError(QString)), this, SLOT(onAuthenticateError(QString)));
    this->connect(fshost, SIGNAL(moduleLoaded(QString,QString)), this, SLOT(onFSModuleLoaded(QString, QString)));
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

void LoginDialog::on_btnLogin_clicked()
{
    QSettings settings;
    QString host = settings.value("General/trainer_server").toString();
    int port = settings.value("trainer_server_port").toInt();
    host = host.isEmpty() ? "voip.idapted.com" : host;
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

    QTimer::singleShot(20000, this, SLOT(onAuthenticateTimeout()));

//    emit Login();
}

void LoginDialog::on_cancelLogin_clicked()
{
    qDebug() << "Cancel";
    tcp_client->close();
    ui->frmSplash->hide();
    _abort = true;
}

void LoginDialog::onAuthenticated(QVariantMap map)
{
    qDebug() << "Authenticated";
    _user = map;
    _authenticated = true;

    QTimer::singleShot(1000, this, SLOT(doRegisterToVoIP()));

//    hide();
}

void LoginDialog::onAuthenticateError(QString reason)
{
    QMessageBox::critical( this, QApplication::applicationName(),
                           QString("Authenticate Error:\nReason: %1").arg(reason));
    ui->frmSplash->hide();
}

void LoginDialog::onAuthenticateTimeout()
{
    if (_abort || _authenticated) return;
    onAuthenticateError("Time out");
}

void LoginDialog::onFSModuleLoaded(QString modType, QString modKey)
{
    ui->teProgress->insertPlainText(QString("Loaded: [%1] %2\n")
                            .arg(modType).arg(modKey));
    ui->teProgress->textCursor().movePosition(QTextCursor::End);
    ui->teProgress->ensureCursorVisible();
}

void LoginDialog::doRegisterToVoIP()
{
    if (_abort) return;

    static int count = 0;
    if (count++ == 20) {
        //20 seconds past and FS still didn't load
        QMessageBox::critical(this, QApplication::applicationName(),
                              "Error loading VoIP");
        ui->frmSplash->hide();
        count = 0;
        return;
    }

    if (!fshost->isSofiaReady()) {
        QTimer::singleShot(1000, this, SLOT(doRegisterToVoIP()));
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
    ((QDialog*)parent())->show();
    hide();
}
