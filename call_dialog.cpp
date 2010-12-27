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
	_call = new QPushButton("New Call");
	_hold = new QPushButton("Hold");
	_answer = new QPushButton("Answer");
	_hangup = new QPushButton("Hangup");
	_sipStatusLabel = new QLabel("SIP: None");

	// _display->setEnabled(false);

	_dialPadWidget = new DialPadWidget();

	QGridLayout *layout = new QGridLayout();
	layout->setHorizontalSpacing(-1);
	layout->setVerticalSpacing(-1);
	layout->addWidget(_display, 0, 0, 1, 2);
	layout->addWidget(_call, 1, 0, 1, 1);
	layout->addWidget(_hold, 1, 1, 1, 1);
	layout->addWidget(_answer, 2, 0, 1, 1);
	layout->addWidget(_hangup, 2, 1, 1, 1);
	layout->addWidget(_dialPadWidget, 3, 0, 1, 2);
	layout->addWidget(_sipStatusLabel, 4, 0, 1, 1);
	setLayout(layout);

	setWindowTitle("Call");
	setFixedSize(300, 330);

	connect(_dialPadWidget, SIGNAL(dialed(QString)), this, SLOT(dialed(QString)));
	connect(_call, SIGNAL(clicked()), this, SLOT(onCall()));
	connect(_hold, SIGNAL(clicked()), this, SLOT(onHold()));
	connect(_answer, SIGNAL(clicked()), this, SLOT(onAnswer()));
	connect(_hangup, SIGNAL(clicked()), this, SLOT(onHangup()));

	connect(ApplicationController::fs(), SIGNAL(gatewayStateChange(QString, QString)),
		this, SLOT(onGatewayStateChange(QString, QString)));
	connect(ApplicationController::fs(), SIGNAL(callIncoming(QString, QString, QString)),
		this, SLOT(onCallIncoming(QString, QString, QString)));
	connect(ApplicationController::fs(), SIGNAL(callAnswered(QString, QString, QString)),
		this, SLOT(onCallAnswered(QString, QString, QString)));
	connect(ApplicationController::fs(), SIGNAL(callEnded(QString, QString, QString)),
		this, SLOT(onCallEnded(QString, QString, QString)));
}

CallDialog::~CallDialog()
{
}

void CallDialog::keyPressEvent(QKeyEvent *event)
{
}

void CallDialog::closeEvent(QCloseEvent *event)
{
	emit closed();
}

void CallDialog::onCall()
{
	bool ok = true;
	QString dialString = QInputDialog::getText(this, tr("Make Call"), tr("Number:"), QLineEdit::Normal, NULL, &ok);
	if (ok && !dialString.isEmpty())
	{
		ApplicationController::fs()->call(dialString);
	}
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
}

void CallDialog::dialed(QString dtmf)
{
	if (dtmf.length() > 0) {
		ApplicationController::fs()->portAudioDtmf(dtmf.toAscii()[0]);
		_display->setText(_display->toPlainText() + dtmf);
	}
}

void CallDialog::onGatewayStateChange(QString /*name*/, QString state)
{
	if (state == "TRYING" || state == "REGISTER") {
		// do nothing
	}
	else { //REGED UNREGED UNREGISTER FAILED FAIL_WAIT EXPIRED NOREG NOAVAIL
	}
	_sipStatusLabel->setText(QString("SIP State: %1").arg(state));
}

void CallDialog::onCallIncoming(QString uuid, QString cidNumber, QString cidName)
{
	_display->setText(QString("%1\nI: %2").arg(
		_display->toPlainText()).arg(
		Utils::formatCallerID(cidName, cidNumber)));
}

void CallDialog::onCallAnswered(QString uuid, QString cidNumber, QString cidName)
{
	_display->setText(QString("%1\nA: %2").arg(
		_display->toPlainText()).arg(
		Utils::formatCallerID(cidName, cidNumber)));
}

void CallDialog::onCallEnded(QString uuid, QString cidNumber, QString cidName)
{
	_display->setText(QString("%1\nE: %2").arg(
		_display->toPlainText()).arg(
		Utils::formatCallerID(cidName, cidNumber)));
}

