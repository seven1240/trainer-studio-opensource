#include <QtGui/QVBoxLayout>
#include <QMessageBox>
#include "application_controller.h"
#include "echo_test_dialog.h"
#include "settings_dialog.h"
#include "main_window.h"
#include "server_connection.h"
#include "utils.h"

EchoTestDialog::EchoTestDialog(QWidget *parent) :
	QDialog(parent)
{
	_progress = new QLabel("We will now test your microphone. Press Begin and after the beep, say 'Testing 1, 2, 3' out loud.");
	_progress->setWordWrap(true);
	_skip = new QPushButton("Skip");
	_begin = new QPushButton("Begin");
	_finish = new QPushButton("Finish");

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(new QFrame());
	layout->addWidget(_progress);
	layout->addWidget(_begin);
	layout->addWidget(_skip);
	layout->addWidget(_finish);
	layout->addWidget(new QFrame());
	setLayout(layout);

	_progress->setWordWrap(true);

	setModal(true);
	setWindowTitle("Echo Testing");
	setFixedSize(400, 240);
	Utils::centerWindowOnDesktop(this);

	connect(_begin, SIGNAL(clicked()), this, SLOT(onBeginClicked()));
	connect(_skip, SIGNAL(clicked()), this, SLOT(onSkipClicked()));
	connect(_finish, SIGNAL(clicked()), this, SLOT(onFinishClicked()));
	connect(ApplicationController::fs(), SIGNAL(newEvent(QSharedPointer<switch_event_t>)), this, SLOT(onNewEvent(QSharedPointer<switch_event_t>)));
}

EchoTestDialog::~EchoTestDialog()
{
}

void EchoTestDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		// ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void EchoTestDialog::showEvent(QShowEvent * /*e*/)
{
	/* only force echo test the first time we login */
	static bool passedEchoTest = false;

	progress("We will now test your microphone. Press Begin and after the beep, say 'Testing 1, 2, 3' out loud.");

	User *user = ApplicationController::user();
	_skip->setVisible(user->skipEchoTesting() || passedEchoTest);
	_finish->setVisible(false);
	_begin->setVisible(true);
	passedEchoTest = true;
}

void EchoTestDialog::closeEvent(QCloseEvent * /*e*/)
{
	ApplicationController::fs()->hangup(true);
	emit closed();
}

void EchoTestDialog::progress(QString string)
{
	_progress->setText(string);
	_progress->repaint();
}

void EchoTestDialog::onNewEvent(QSharedPointer<switch_event_t> spEvent)
{
	switch_event_t *event = spEvent.data();

	if (!isVisible())
		return;

	switch (event->event_id) {
	case SWITCH_EVENT_CHANNEL_CREATE:
		{
			progress("Dialing...");
			break;
		}
	case SWITCH_EVENT_CHANNEL_ANSWER:
		{
			progress("Answered");
			break;
		}
	case SWITCH_EVENT_CHANNEL_BRIDGE:
		{
			progress("Server Answered, Please talk...");
			break;
		}
	case SWITCH_EVENT_CHANNEL_HANGUP:
		{
			qDebug() << "Direction:" << switch_event_get_header_nil(event, "Caller-Direction");
			if (!strcmp(switch_event_get_header_nil(event, "Caller-Direction"), "outbound")) {
				return;
			}
			progress("Channel Hangup!");
			if (didTheyHearThemselves()) {
				ApplicationController::server()->pause(TRUE);
				accept();
			}
			else {
				progress("Please try again. If the problems continues, please contact your supervisor.");
				User *user = ApplicationController::user();
				_skip->setVisible(user->skipEchoTesting());
				_begin->setVisible(true);
				_finish->setVisible(false);
			}
			break;
		}
	default:
		;
	}
}

bool EchoTestDialog::didTheyHearThemselves()
{
	return QMessageBox::question(NULL, "Testing", "Your test call is finished, did you hear yourself?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes;
}

void EchoTestDialog::onFinishClicked()
{
	ApplicationController::fs()->hangup(true);
}

void EchoTestDialog::onBeginClicked()
{
	_finish->setVisible(true);
	_skip->setVisible(false);
	_begin->setVisible(false);
	progress("Calling...");
	ApplicationController::fs()->call("echo");
}

void EchoTestDialog::onSkipClicked()
{
	ApplicationController::server()->pause(TRUE);
	accept();
}
