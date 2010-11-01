#include <QDebug.h>
#include <QMessageBox.h>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include "login_dialog.h"
#include "tcp_client.h"
#include "fs_host.h"
#include "isettings.h"
#include "main_window.h"
#include "settings_dialog.h"
#include "echo_test_dialog.h"
#include "cjson.h"
#include "utils.h"

LoginDialog::LoginDialog(QWidget *parent) :
  QDialog(parent)
{
  _lbProgress = new QLabel();
  _pbSettings = new QPushButton("Settings");
  _pbCancel = new QPushButton("Cancel");
  _pbLogin = new QPushButton("Login");
  _leUsername = new QLineEdit();
  _lePassword = new QLineEdit();
  _lePassword->setEchoMode(QLineEdit::Password);
  _teProgress = new QTextEdit();

  QGroupBox *loginFormGroupBox = new QGroupBox(tr("Login"));
  QFormLayout *formLayout = new QFormLayout;
  formLayout->addRow(new QLabel(tr("Username:")), _leUsername);
  formLayout->addRow(new QLabel(tr("Password:")), _lePassword);
  loginFormGroupBox->setLayout(formLayout);

  _loginFrame = new QFrame();
  QVBoxLayout *loginLayout = new QVBoxLayout();
  loginLayout->addWidget(_lbProgress);
  loginLayout->addWidget(loginFormGroupBox);
  loginLayout->addWidget(_pbLogin);
  loginLayout->addWidget(_pbSettings);
  loginLayout->addWidget(new QFrame());
  _loginFrame->setLayout(loginLayout);

  _progressFrame = new QFrame();
  QVBoxLayout *progressLayout = new QVBoxLayout();
  progressLayout->addWidget(_lbProgress);
  progressLayout->addWidget(_teProgress);
  progressLayout->addWidget(_pbCancel);
  _progressFrame->setLayout(progressLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_loginFrame);
  mainLayout->addWidget(_progressFrame);
  setLayout(mainLayout);

  setFixedSize(320, 240);
  setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
  setWindowTitle("Login");

  showLogin();

  QSettings settings;
  _authenticated = false;
  _leUsername->setText(settings.value("StoredData/Username", "").toString());

  this->connect(tcp_client, SIGNAL(authenticated(QVariantMap)), this, SLOT(onAuthenticated(QVariantMap)));
  this->connect(tcp_client, SIGNAL(authenticateError(QString)), this, SLOT(onAuthenticateError(QString)));
  this->connect(tcp_client, SIGNAL(socketError(QString)), this, SLOT(onSocketError(QString)));
  this->connect(fshost, SIGNAL(moduleLoaded(QString, QString, QString)), this, SLOT(onFSModuleLoaded(QString, QString, QString)));
  this->connect(_pbLogin, SIGNAL(clicked()), this, SLOT(onLoginClicked()));
  this->connect(_pbSettings, SIGNAL(clicked()), this, SLOT(onSettingsClicked()));
  this->connect(_pbCancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    // retranslateUi(this);
    break;
  default:
    break;
  }
}

void LoginDialog::closeEvent(QCloseEvent *e)
{
}

void LoginDialog::KeyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Escape) return;
}

void LoginDialog::abortLogin()
{
  _abort = true;
  showLogin();
  _pbSettings->setVisible(true);
}

void LoginDialog::abortLogin(QString msg)
{
  abortLogin();
  QMessageBox::critical( this, QApplication::applicationName(), msg);
}

void LoginDialog::onCancelClicked()
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

void LoginDialog::showProgress()
{
  _loginFrame->hide();
  _progressFrame->show();
}

void LoginDialog::showLogin()
{
  _progressFrame->hide();
  _loginFrame->show();
}

void LoginDialog::setProgress(QString string)
{
  _lbProgress->setText(string);
  _lbProgress->repaint();
}

void LoginDialog::onLoginClicked()
{
  QSettings settings;
  settings.setValue("StoredData/Username", _leUsername->text());
  QString host = settings.value("General/trainer_server").toString();
  int port = settings.value("trainer_server_port").toInt();
  host = host.isEmpty() ? "voip.idapted.com" : host;
  port = port == 0 ? 7000 : port;

  qDebug() << host << ":" << port;
  showProgress();
  tcp_client->connectToHost(host, port);

  QVariantMap map = Utils::getSystemInfos();

  char *json = NULL;

  cJSON *info = cJSON_CreateArray();

  cJSON *item;
  item = cJSON_CreateObject();
  cJSON_AddItemToObject(item, "os", cJSON_CreateString(map["os"].toString().toAscii().data()));
  cJSON_AddItemToObject(item, "memory", cJSON_CreateString(map["memory"].toString().toAscii().data()));
  cJSON_AddItemToObject(item, "screen_res", cJSON_CreateString(map["screen_res"].toString().toAscii().data()));
  cJSON_AddItemToObject(item, "flash_player_version", cJSON_CreateString(map["flash_player_version"].toString().toAscii().data()));
  cJSON_AddItemToArray(info, item);

  cJSON *cj = cJSON_CreateObject();

  cJSON_AddItemToObject(cj, "username", cJSON_CreateString(_leUsername->text().toAscii().data()));
  cJSON_AddItemToObject(cj, "password", cJSON_CreateString(_lePassword->text().toAscii().data()));
  cJSON_AddItemToObject(cj, "action", cJSON_CreateString("Authenticate"));
  cJSON_AddItemToObject(cj, "system_info", info);

  json = cJSON_Print(cj);
  cJSON_Delete(cj);

  qDebug() << json;

  _abort = false;

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

  setProgress("Socket connected, authenticating...");

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
    setProgress("Loading VoIP modules...");
    QTimer::singleShot(1000, this, SLOT(doRegisterToVoIP()));
    return;
  }

  //setup sip account and register
  setProgress("Registering to VoIP sever...");

  ISettings *isettings = new ISettings(this);

  QVariantMap gw = isettings->getGateway(QString("default"));
  QString res;

  if(gw["username"] == _user["voip_username"] && gw["password"] == _user["voip_password"]) {
    qDebug() << "No gateway info changed";
  }else{
    qDebug() << "Need to set gateway";

    QVariantMap newgw;
    newgw.insert("username", _user["voip_username"]);
    newgw.insert("password", _user["voip_password"]);
    QVariantList serverlist = _user["servers"].toList();
    QVariantMap server = serverlist[0].toMap();

    newgw.insert("realm", QString("%1:%2").arg(server["sip_proxy"].toString(), server["sip_port"].toString()));
    QSettings qsettings;
    if (qsettings.value("sip_transport").toString() == "tcp") {
      newgw.insert("register-transport", "tcp");
    }
    isettings->writeGateway(newgw);
    isettings->saveToFile();
    switch_sleep(1000000);
  }

  fshost->sendCmd("sofia", "profile softphone rescan reloadxml", &res);
  qDebug() << res;

  delete isettings;
  hide();

  EchoTestDialog *etd = new EchoTestDialog((QWidget *)this->parent());
  etd->setAttribute(Qt::WA_DeleteOnClose);
  etd->show();
}

void LoginDialog::onSettingsClicked()
{
  SettingsDialog *settings_dialog = new SettingsDialog(this);
  settings_dialog->setAttribute(Qt::WA_DeleteOnClose);
  settings_dialog->show();
}

void LoginDialog::onFSModuleLoaded(QString modType, QString modKey, QString modName)
{
  _teProgress->insertPlainText(QString("Loaded: [%1] %2 %3\n").arg(modType).arg(modKey).arg(modName));
  _teProgress->textCursor().movePosition(QTextCursor::End);
  _teProgress->ensureCursorVisible();
}
