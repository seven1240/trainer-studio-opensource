#include <QDebug.h>
#include <QMessageBox>
#include <QApplication>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QKeyEvent>
#include <QtGui/QInputDialog>
#include "application_controller.h"
#include "freeswitch.h"
#include "call_dialog.h"
#include "dial_pad_widget.h"
#include "utils.h"

CallDialog::CallDialog(QWidget *parent) : QDialog(parent)
{
	_display = new QTextEdit();
	_number = new QLineEdit();
	_call = new QPushButton("Call");
	_hold = new QPushButton("Hold");
	_answer = new QPushButton("Answer");
	_hangup = new QPushButton("Hangup");
	_conf = new QPushButton("Conference");
	_n800 = new QPushButton("800");
	_n800->setToolTip("Call 8888879913 via VoIP");
	_echo = new QPushButton("Echo Test");
	_sipStatusLabel = new QLabel("SIP: None");

	_display->setReadOnly(true);

	_dialPadWidget = new DialPadWidget();

	QGridLayout *layout = new QGridLayout();
	layout->setHorizontalSpacing(-1);
	layout->setVerticalSpacing(-1);
	layout->addWidget(_display, 0, 0, 1, 3);
	layout->addWidget(_number, 1, 0, 1, 2);
	layout->addWidget(_call, 1, 2, 1, 1);
	layout->addWidget(_hold, 2, 1, 1, 1);
	layout->addWidget(_answer, 2, 0, 1, 1);
	layout->addWidget(_hangup, 2, 2, 1, 1);
	layout->addWidget(_conf, 3, 0, 1, 1);
	layout->addWidget(_n800, 3, 1, 1, 1);
	layout->addWidget(_echo, 3, 2, 1, 1);
	layout->addWidget(_dialPadWidget, 4, 0, 1, 3);
	layout->addWidget(_sipStatusLabel, 5, 0, 1, 3);
	setLayout(layout);

	setWindowTitle("Call");
	setFixedSize(300, 380);

	connect(_number, SIGNAL(returnPressed()), this, SLOT(onCall()));
	connect(_dialPadWidget, SIGNAL(dialed(QString)), this, SLOT(dialed(QString)));
	connect(_call, SIGNAL(clicked()), this, SLOT(onCall()));
	connect(_hold, SIGNAL(clicked()), this, SLOT(onHold()));
	connect(_answer, SIGNAL(clicked()), this, SLOT(onAnswer()));
	connect(_hangup, SIGNAL(clicked()), this, SLOT(onHangup()));
	connect(_conf, SIGNAL(clicked()), this, SLOT(onCallConf()));
	connect(_n800, SIGNAL(clicked()), this, SLOT(onCall800()));
	connect(_echo, SIGNAL(clicked()), this, SLOT(onCallEcho()));

	connect(ApplicationController::fs(), SIGNAL(gatewayStateChange(QString, QString)),
		this, SLOT(onGatewayStateChange(QString, QString)));
	connect(ApplicationController::fs(), SIGNAL(newIncomingCall(QString, QString, QString)),
		this, SLOT(onCallIncoming(QString, QString, QString)));
	connect(ApplicationController::fs(), SIGNAL(callAnswered(QString, QString, QString)),
		this, SLOT(onCallAnswered(QString, QString, QString)));
	connect(ApplicationController::fs(), SIGNAL(callEnded(QString, QString, QString)),
		this, SLOT(onCallEnded(QString, QString, QString)));
}

CallDialog::~CallDialog()
{
}

void CallDialog::keyPressEvent(QKeyEvent * /*event*/)
{
}

void CallDialog::showEvent(QShowEvent * /*event*/)
{
	_call->setEnabled(true);
}

void CallDialog::closeEvent(QCloseEvent * /*event*/)
{
	emit closed();
}

void CallDialog::onCall()
{
	QString number = _number->text();
	if (!number.isEmpty()) {
		_display->append(QString("Calling %2").arg(number));
		ApplicationController::fs()->call(number);
	}
	_call->setEnabled(false);
}

void CallDialog::onHold()
{
}

void CallDialog::onAnswer()
{
	ApplicationController::fs()->answer();
}

void CallDialog::onHangup()
{
	ApplicationController::fs()->hangup(true);
	_call->setEnabled(true);
}

void CallDialog::onCallConf()
{
	_number->setText("conf");
}

void CallDialog::onCall800()
{
	_number->setText("8888879913");
}

void CallDialog::onCallEcho()
{
	_number->setText("echo");
}

void CallDialog::dialed(QString dtmf)
{
	if (dtmf.length() > 0) {
		ApplicationController::fs()->portAudioDtmf(dtmf.toAscii()[0]);
		_display->setText(_display->toPlainText() + dtmf);
	}
}

void CallDialog::onGatewayStateChange(QString /*gwName*/, QString gwState)
{
	if (gwState == "TRYING" || gwState == "REGISTER") {
		// do nothing
	}
	else { //REGED UNREGED UNREGISTER FAILED FAIL_WAIT EXPIRED NOREG NOAVAIL
	}
	_sipStatusLabel->setText(QString("SIP State: %1").arg(gwState));
}

void CallDialog::onCallIncoming(QString /*uuid*/, QString cidNumber, QString cidName)
{
	_display->append(QString("Ring From: %2").arg(
		Utils::formatCallerID(cidName, cidNumber)));
}

void CallDialog::onCallAnswered(QString /*uuid*/, QString cidNumber, QString cidName)
{
	_display->append(QString("Call Answered: %2").arg(
		Utils::formatCallerID(cidName, cidNumber)));
}

void CallDialog::onCallEnded(QString /*uuid*/, QString cidNumber, QString cidName)
{
	_display->append(QString("Call Ended: %2").arg(
		Utils::formatCallerID(cidName, cidNumber)));
}

