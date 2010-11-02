#include <QtGui>
#include <QDebug.h>
#include <QMessageBox>
#include "application_controller.h"
#include "server_connection.h"
#include "main_window.h"
#include "isettings.h"
#include "utils.h"

QSystemTrayIcon *sysTray;

MainWindow::MainWindow(QWidget *parent) :
	QWidget(parent)
{
	createBody();

	setWindowTitle("Trainer Studio - Idapted Ltd.");
	setFixedSize(218, 430);
	Utils::centerWindowOnDesktop(this);

	// We should not need set NULL manually, but without this,
	// flash_dialog is NULL but not login_dialog, weird
	// Also, Mac & Win have different default value
	login_dialog = NULL;
	flash_dialog = new FlashDialog(this);
	incoming_call_dialog = new IncomingCallDialog();
	settings_dialog = NULL;
	_sipStateReady = false;
	btnState->setChecked(false);
	_timer = new QTimer(this);
	_timer->setInterval(20000);

	sysTray = new QSystemTrayIcon(QIcon(":/images/taskbar_icon"), this);
	sysTray->setToolTip(QApplication::applicationName());
	sysTray->show();
	sysTray->showMessage(QApplication::applicationName(), "Initialized", QSystemTrayIcon::Information, 2000);

	connect(ApplicationController::server(), SIGNAL(pauseChanged(bool)), this, SLOT(onPaused(bool)));
	connect(ApplicationController::server(), SIGNAL(forcedPause(QString)), this, SLOT(onForcedPause(QString)));
	connect(ApplicationController::server(), SIGNAL(reservedForInteraction(QVariantMap)), this, SLOT(onReservedForInteraction(QVariantMap)));
	connect(ApplicationController::server(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
	connect(incoming_call_dialog, SIGNAL(answered(QString, QString)), this, SLOT(onAnswered(QString, QString)));
	connect(ApplicationController::fs(), SIGNAL(gatewayStateChange(QString, QString)), this, SLOT(onGatewayStateChange(QString, QString)));
	connect(_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));

	QMetaObject::connectSlotsByName(this);
}

QLayout *MainWindow::createBody()
{
	QPushButton *settingsButton = new QPushButton("Settings");
	settingsButton->setObjectName("Settings");
	QPushButton *loginButton = new QPushButton("Login");
	loginButton->setObjectName("Login");
	QPushButton *logoutButton = new QPushButton("Logout");
	logoutButton->setObjectName("Logout");
	QPushButton *echoButton = new QPushButton("Echo");
	echoButton->setObjectName("Echo");
	QPushButton *pauseButton = new QPushButton("Pause");
	pauseButton->setObjectName("State");
	QPushButton *closeButton = new QPushButton("Close");
	closeButton->setObjectName("Close");
	QPushButton *aboutButton = new QPushButton("About");
	aboutButton->setObjectName("About");

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(settingsButton);
	layout->addWidget(loginButton);
	layout->addWidget(logoutButton);
	layout->addWidget(echoButton);
	layout->addWidget(pauseButton);
	layout->addWidget(closeButton);
	layout->addWidget(aboutButton);

	QLabel *connectionLabel = new QLabel("Connection: No");
	layout->addWidget(connectionLabel);

	QLabel *sipLabel = new QLabel("SIP: None");
	layout->addWidget(sipLabel);

	btnState = pauseButton;
	lbStatus = connectionLabel;
	lbSIPStatus = sipLabel;

	setLayout(layout);

	return layout;
}

MainWindow::~MainWindow()
{
	if (login_dialog) delete login_dialog;
	if (flash_dialog) delete flash_dialog;
	if (incoming_call_dialog) delete incoming_call_dialog;
	if (settings_dialog) delete settings_dialog;
	delete (_timer);
}

void MainWindow::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		// retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::showLoginDialog()
{
	if (!login_dialog) {
		login_dialog = new LoginDialog(this);
	}
	login_dialog->raise();
	login_dialog->show();
	login_dialog->activateWindow();
}

void MainWindow::on_Flash_clicked()
{
	flash_dialog->raise();
	flash_dialog->show();
	flash_dialog->activateWindow();
}

void MainWindow::on_Login_clicked()
{
	showLoginDialog();
}

void MainWindow::on_Logout_clicked()
{
	ApplicationController::server()->logout();
}

void MainWindow::on_State_clicked()
{
	if (!_sipStateReady) {
		QMessageBox::warning(this, QApplication::applicationName(), "Cannot Pause, VoIP not ready!");
	}
	// why isChecked shows reversed?
	qDebug() << btnState->isChecked();
	ApplicationController::server()->pause(!btnState->isChecked());
}

void MainWindow::onPaused(bool state)
{
	if (state){
		btnState->setText("> Start Working");
		btnState->setChecked(false);
		QApplication::alert(this, 0);
		_timer->start();
	}
	else {
		btnState->setText("|| Pause");
		btnState->setChecked(true);
		_timer->stop();
	}
}

void MainWindow::onForcedPause(QString reason)
{
	QMessageBox::warning(this, QApplication::applicationName(), reason);
	onPaused(true);
}

void MainWindow::onAnswered(QString cid_name, QString /*cid_number*/)
{
	if (cid_name.left(2) == "IT") {
		flash_dialog->raise();
		flash_dialog->show();
		flash_dialog->activateWindow();
	}
	else {
	}
}

void MainWindow::onGatewayStateChange(QString /*name*/, QString state)
{
	if (state == "REGED") {
		if (!_sipStateReady) {
			_sipStateReady = true;
		}
	}
	else if (state == "TRYING" || state == "REGISTER") {
		// do nothing
	}
	else { //UNREGED UNREGISTER FAILED FAIL_WAIT EXPIRED NOREG NOAVAIL
		if (_sipStateReady) {
			_sipStateReady = false;
			if (btnState->isChecked())
				ApplicationController::server()->pause(true); //force pause
		}
	}
	lbSIPStatus->setText(QString("SIP State: %1").arg(state));
}

void MainWindow::onReservedForInteraction(QVariantMap data)
{
	QString msg = QString("New learner comming with InteractionID %1").arg(data["interaction_id"].toString());
	lbStatus->setText(msg);
	sysTray->showMessage(QApplication::applicationName(),msg, QSystemTrayIcon::Information, 3000);
}

void MainWindow::onDisconnected()
{
	QMessageBox::critical(this,QApplication::applicationName(), "Socket Broken!!");
	showLoginDialog();
	hide();
	delete flash_dialog;
	flash_dialog = new FlashDialog(this);
}

void MainWindow::on_About_clicked()
{
	QMessageBox::about(this, QApplication::applicationName(),
					   QString("Version: %1\n\nCopyright(c): Idapted Ltd.").arg(QApplication::applicationVersion()));
}

void MainWindow::on_Settings_clicked()
{
	if (!settings_dialog)
		settings_dialog = new SettingsDialog();
	settings_dialog->show();
}

void MainWindow::on_Echo_clicked()
{
	if (!_activeUUID.isEmpty()) return;
	lbStatus->setText("Echo test");
	parseCallResult(ApplicationController::fs()->call("echo"));
}

void MainWindow::on_Conference_clicked()
{
	if (!_activeUUID.isEmpty()) return;
	lbStatus->setText("Conference");
	parseCallResult(ApplicationController::fs()->call("conf"));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (false) return; // TODO if in call?

	if (event->key() == 35 || event->key() == Qt::Key_Asterisk ||  // # *
		event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9 ||  // 0 - 9
		event->key() >= Qt::Key_A && event->key() <= Qt::Key_D ) { // A-D
		ApplicationController::fs()->portAudioDtmf((char)event->key());
	}
}

void MainWindow::onNewEvent(QSharedPointer<switch_event_t> spEvent)
{
	switch_event_t *event = spEvent.data();

	QString uuid = switch_event_get_header_nil(event, "Unique-ID");

	if (uuid != _activeUUID )  return;

	QString eventName = switch_event_get_header_nil(event, "Event-Name");
	QString eventSubclass = switch_event_get_header_nil(event, "Event-Subclass");
	lbStatus->setText(eventName + "::" + eventSubclass);

	switch (event->event_id) {
	case SWITCH_EVENT_CHANNEL_HANGUP_COMPLETE:
		{
			_activeUUID = "";
			break;
		}
	default:
		break;
	}
}

void MainWindow::parseCallResult(QString res)
{
	qDebug() << "Parse: " << res;
	QStringList sl = res.split(":");
	if (sl.count() == 3 && sl.at(0) == "SUCCESS") {
		_activeUUID = sl.at(2).trimmed();
		connect(ApplicationController::fs(), SIGNAL(newEvent(QSharedPointer<switch_event_t>)), this, SLOT(onNewEvent(QSharedPointer<switch_event_t>)));
	} else {
		_activeUUID = "";
		disconnect(this, SLOT(onNewEvent(QSharedPointer<switch_event_t>)));
		lbStatus->setText(res.trimmed());
	}
}

void MainWindow::on_Hangup_clicked()
{
	ApplicationController::fs()->hangup(true);
}

void MainWindow::onTimerTimeout()
{
	sysTray->showMessage(QApplication::applicationName(), "Trainer Studio Paused!", QSystemTrayIcon::Information, 1000);
}
