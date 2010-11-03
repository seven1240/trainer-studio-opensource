#include <QDebug.h>
#include <QMessageBox>
#include <QApplication>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QTextEdit>
#include <QtGui/QPushButton>
#include "application_controller.h"
#include "login_dialog.h"
#include "server_connection.h"
#include "fs_host.h"
#include "isettings.h"
#include "settings_dialog.h"
#include "echo_test_dialog.h"
#include "utils.h"

LoginDialog::LoginDialog(QWidget *parent) :
	QDialog(parent)
{
	_lbProgress = new QLabel();
	_pbSettings = new QPushButton("Settings");
	_pbSettings->setObjectName("Settings");
	_pbCancel = new QPushButton("Cancel");
	_pbCancel->setObjectName("Cancel");
	_pbLogin = new QPushButton("Login");
	_pbLogin->setObjectName("Login");
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
	setWindowTitle("Login");

	showLogin();

	QSettings settings;
	_authenticated = false;
	_leUsername->setText(settings.value("StoredData/Username", "").toString());

	connect(ApplicationController::server(), SIGNAL(connected()), this, SLOT(onServerConnectionConnected()));
	connect(ApplicationController::server(), SIGNAL(authenticated(User*)), this, SLOT(onServerConnectionAuthenticated(User*)));
	connect(ApplicationController::server(), SIGNAL(disconnected()), this, SLOT(onServerConnectionDisconnected()));
	connect(ApplicationController::server(), SIGNAL(authenticateError(QString)), this, SLOT(onAuthenticateError(QString)));
	connect(ApplicationController::server(), SIGNAL(socketError(QString)), this, SLOT(onSocketError(QString)));
	connect(ApplicationController::fs(), SIGNAL(moduleLoaded(QString, QString, QString)), this, SLOT(onFSModuleLoaded(QString, QString, QString)));
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
	ApplicationController::server()->login(_leUsername->text(), _lePassword->text());
}

void LoginDialog::onServerConnectionAuthenticated(User *user)
{
	_user = user;
	_authenticated = true;
	ApplicationController::fs()->setupGateway(user->getVoipUsername(), user->getVoipPassword(), user->getVoipServer(), false);
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

void LoginDialog::onKeyPressEvent(QKeyEvent *e)
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
	if (_abort || _authenticated)
		return;
	QMessageBox::critical(this, QApplication::applicationName(), msg);
}

void LoginDialog::onCancelClicked()
{
	qDebug() << "Cancel";
	ApplicationController::server()->close();
	abortLogin();
}

void LoginDialog::onAuthenticateError(QString reason)
{
	ApplicationController::server()->close();
	QString msg = QString("Authenticate Error:\nReason: %1").arg(reason);
	abortLogin(msg);
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
	host = "192.168.0.127";

	qDebug() << host << ":" << port;
	_abort = false;
	showProgress();
	ApplicationController::server()->open(host, port);
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
