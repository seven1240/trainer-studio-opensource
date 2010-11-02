#include <QDebug.h>
#include <QMessageBox>
#include <QApplication>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QTextEdit>
#include "login_dialog.h"
#include "server_connection.h"
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
	// setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	setWindowTitle("Login");

	showLogin();

	QSettings settings;
	_authenticated = false;
	_leUsername->setText(settings.value("StoredData/Username", "").toString());

	connect(server_connection, SIGNAL(connected()), this, SLOT(onServerConnectionConnected()));
	connect(server_connection, SIGNAL(authenticated(User*)), this, SLOT(onServerConnectionAuthenticated(User*)));
	connect(server_connection, SIGNAL(disconnected()), this, SLOT(onServerConnectionDisconnected()));
	connect(server_connection, SIGNAL(authenticateError(QString)), this, SLOT(onAuthenticateError(QString)));
	connect(server_connection, SIGNAL(socketError(QString)), this, SLOT(onSocketError(QString)));
	connect(fs, SIGNAL(moduleLoaded(QString, QString, QString)), this, SLOT(onFSModuleLoaded(QString, QString, QString)));
	connect(_pbLogin, SIGNAL(clicked()), this, SLOT(onLoginClicked()));
	connect(_pbSettings, SIGNAL(clicked()), this, SLOT(onSettingsClicked()));
	connect(_pbCancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));

	QMetaObject::connectSlotsByName(this);
}

LoginDialog::~LoginDialog()
{
}

void LoginDialog::onServerConnectionConnected()
{
	setProgress("Connected, authenticating...");
	server_connection->login(_leUsername->text(), _lePassword->text());
	QTimer::singleShot(20000, this, SLOT(onAuthenticateTimeout()));
}

void LoginDialog::onServerConnectionAuthenticated(User *user)
{
	_user = user;
	_authenticated = true;
	QTimer::singleShot(1000, this, SLOT(doRegisterToVoIP()));
}

void LoginDialog::onServerConnectionDisconnected()
{
	showLogin();
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

void LoginDialog::closeEvent(QCloseEvent * /*e*/)
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
}

void LoginDialog::abortLogin(QString msg)
{
	abortLogin();
	QMessageBox::critical(this, QApplication::applicationName(), msg);
}

void LoginDialog::onCancelClicked()
{
	qDebug() << "Cancel";
	server_connection->close();
	abortLogin();
}

void LoginDialog::onAuthenticateError(QString reason)
{
	server_connection->close();
	QString msg = QString("Authenticate Error:\nReason: %1").arg(reason);
	abortLogin(msg);
}

void LoginDialog::onAuthenticateTimeout()
{
	if (_abort || _authenticated)
		return;
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
	_abort = false;
	showProgress();
	server_connection->open(host, port);
}

void LoginDialog::doRegisterToVoIP()
{
	if (_abort) return;

	static int count = 0;
	if (count++ == 20) {
		abortLogin("Error loading VoIP module, Please close and reopen!!");
		count = 0;
		return;
	}

	if (!fs->isSofiaReady()) {
		setProgress("Loading VoIP modules...");
		QTimer::singleShot(1000, this, SLOT(doRegisterToVoIP()));
		return;
	}

	setProgress("Registering to VoIP sever...");

	ISettings *isettings = new ISettings(this);

	QVariantMap gw = isettings->getGateway(QString("default"));
	QString res;

	if (gw["username"] == _user->getVoipUsername() && gw["password"] == _user->getVoipPassword()) {
		qDebug() << "No gateway info changed";
	}
	else {
		qDebug() << "Need to set gateway";

		QVariantMap newgw;
		newgw.insert("username", _user->getVoipUsername());
		newgw.insert("password", _user->getVoipPassword());
		newgw.insert("realm", _user->getVoipServer());
		QSettings settings;
		if (settings.value("sip_transport").toString() == "tcp") {
			newgw.insert("register-transport", "tcp");
		}
		isettings->writeGateway(newgw);
		isettings->saveToFile();
		switch_sleep(1000000);
	}

	fs->reload();

	delete isettings;
	hide();

	EchoTestDialog *echo_dialog = new EchoTestDialog((QWidget *)this->parent());
	echo_dialog->setAttribute(Qt::WA_DeleteOnClose);
	echo_dialog->show();
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
