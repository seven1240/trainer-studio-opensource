#include "logindialog.h"
#include "ui_logindialog.h"
#include "qdebug.h"
#include "TCPClient.h"
#include <qmessagebox.h>
#include "fshost.h"
#include "isettings.h"
#include "mainwindow.h"
#include "SettingsDialog.h"
#include "EchoTestDialog.h"
#include "cjson.h"
#include "Utils.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    qDebug() << "LoginDialog starting";
    ui->setupUi(this);
    ui->pbSettings->setVisible(false);
    ui->lePassword->setEchoMode(QLineEdit::Password);
    ui->frmSplash->hide();
    ui->frmLogin->move(QPoint(10, 10));
    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    this->setWindowTitle("Login");
    _authenticated = false;

    this->connect(tcp_client, SIGNAL(authenticated(QVariantMap)), this, SLOT(onAuthenticated(QVariantMap)));
    this->connect(tcp_client, SIGNAL(authenticateError(QString)), this, SLOT(onAuthenticateError(QString)));
    this->connect(tcp_client, SIGNAL(socketError(QString)), this, SLOT(onSocketError(QString)));
    this->connect(fshost, SIGNAL(moduleLoaded(QString, QString, QString)), this, SLOT(onFSModuleLoaded(QString, QString, QString)));

    QSettings settings;
    ui->leUsername->setText(settings.value("StoredData/Username", "").toString());
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

void LoginDialog::closeEvent(QCloseEvent *e)
{
//    qDebug() << "Close Event" << e;
}

void LoginDialog::KeyPressEvent(QKeyEvent *e)
{
    //ignore ESC ? Why it doesn't work?
    if (e->key() == Qt::Key_Escape ) return;
}

void LoginDialog::abortLogin()
{
    _abort = true;
    ui->frmSplash->hide();
    ui->pbSettings->setVisible(true);
}

void LoginDialog::abortLogin(QString msg)
{
    abortLogin();
    QMessageBox::critical( this, QApplication::applicationName(), msg);
}

void LoginDialog::on_cancelLogin_clicked()
{
    qDebug() << "Cancel";
    tcp_client->close();
    abortLogin();
}

void LoginDialog::onAuthenticated(QVariantMap map)
{
    qDebug() << "Authenticated";
    _user = map;
    _authenticated = true;

    // set timer to wait FreeSWITCH to be ready
    QTimer::singleShot(1000, this, SLOT(doRegisterToVoIP()));
}

void LoginDialog::onAuthenticateError(QString reason)
{
    tcp_client->close();
    QString msg = QString("Authenticate Error:\nReason: %1").arg(reason);
    abortLogin(msg);
}

void LoginDialog::onAuthenticateTimeout()
{
    if (_abort || _authenticated) return;
    abortLogin("Authenticate Timed out!");
}

void LoginDialog::onSocketError(QString error)
{
    abortLogin(error);
}

void LoginDialog::on_btnLogin_clicked()
{

    QSettings settings;
    settings.setValue("StoredData/Username", ui->leUsername->text());
    QString host = settings.value("General/trainer_server").toString();
    int port = settings.value("trainer_server_port").toInt();
    host = host.isEmpty() ? "voip.idapted.com" : host;
    port = port == 0 ? 7000 : port;

    qDebug() << host << ":" << port;
    ui->frmSplash->setGeometry( ui->frmLogin->geometry());
    ui->frmSplash->show();
    tcp_client->connectToHost(host, port);

    QVariantMap map = Utils::getSystemInfos();

    char *json = NULL;

    cJSON *info = cJSON_CreateArray();

    cJSON *item;
    item = cJSON_CreateObject();
    cJSON_AddItemToObject(item, "memory", cJSON_CreateNumber(100));
    cJSON_AddItemToArray(info, item);
    item = cJSON_CreateObject();
    cJSON_AddItemToObject(item, "os", cJSON_CreateString(map["os"].toString().toAscii().data()));
    cJSON_AddItemToArray(info, item);

    cJSON *cj = cJSON_CreateObject();

    cJSON_AddItemToObject(cj, "username", cJSON_CreateString(ui->leUsername->text().toAscii().data()));
    cJSON_AddItemToObject(cj, "password", cJSON_CreateString(ui->lePassword->text().toAscii().data()));
    cJSON_AddItemToObject(cj, "action", cJSON_CreateString("Authenticate"));
    cJSON_AddItemToObject(cj, "system_info", info);

    json = cJSON_Print(cj);
    cJSON_Delete(cj);

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
        abortLogin("Login Error, Please Try again!");
        return;
    }

    //else connected
    ui->lbProgress->setText("Socket connected, authenticating...");
    ui->lbProgress->repaint();

    tcp_client->write(json);

    QTimer::singleShot(20000, this, SLOT(onAuthenticateTimeout()));

}

void LoginDialog::doRegisterToVoIP()
{
    if (_abort) return;

    static int count = 0;
    if (count++ == 20) {
        //20 seconds past and FS still didn't load
        abortLogin("Error loading VoIP module, Please close and reopen!!");
        count = 0;
        return;
    }

    if (!fshost->isSofiaReady()) {
        ui->lbProgress->setText("Loading VoIP modules...");
        ui->lbProgress->repaint();
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
//    emit login(); // show maimWindow
    hide();
    EchoTestDialog *etd = new EchoTestDialog((QWidget*)this->parent());
    etd->setAttribute(Qt::WA_DeleteOnClose);
    etd->show();
}

void LoginDialog::on_pbSettings_clicked()
{
    SettingsDialog *settings_dialog = new SettingsDialog(this);
    // auto delete on close
    settings_dialog->setAttribute(Qt::WA_DeleteOnClose);
    settings_dialog->show();
}

void LoginDialog::onFSModuleLoaded(QString modType, QString modKey, QString modName)
{
    ui->teProgress->insertPlainText(QString("Loaded: [%1] %2 %3\n")
                            .arg(modType).arg(modKey).arg(modName));
    ui->teProgress->textCursor().movePosition(QTextCursor::End);
    ui->teProgress->ensureCursorVisible();
}
