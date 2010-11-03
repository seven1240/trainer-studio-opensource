#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "application_controller.h"
#include "incoming_call_dialog.h"
#include "freeswitch.h"

IncomingCallDialog::IncomingCallDialog(QWidget *parent) :
	QDialog(parent)
{
	QGridLayout *layout = new QGridLayout();
	_answer = new QPushButton("Answer");
	_reject = new QPushButton("Hangup");
	_status = new QLabel();
	layout->addWidget(_status, 0, 0, 1, 2, Qt::AlignHCenter);
	layout->addWidget(_reject, 1, 0, 1, 1, Qt::AlignLeft);
	layout->addWidget(_answer, 1, 1, 1, 1, Qt::AlignRight);
	setLayout(layout);

	setFixedSize(320, 140);
	setWindowTitle("Incoming Call");

	connect(_answer, SIGNAL(clicked()), this, SLOT(onAnswerClicked()));
	connect(_reject, SIGNAL(clicked()), this, SLOT(onRejectClicked()));
	connect(ApplicationController::fs(), SIGNAL(callIncoming(QString,QString,QString)), this, SLOT(onIncomingCall(QString,QString,QString)));
}

IncomingCallDialog::~IncomingCallDialog()
{
}

void IncomingCallDialog::changeEvent(QEvent *e)
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

void IncomingCallDialog::onIncomingCall(QString uuid, QString number, QString name)
{
	_callerName = name;
	_callerNumber = number;
	_status->setText(QString("\"%1\" <%2>").arg(_callerName).arg(_callerNumber));
	_status->repaint();
}

void IncomingCallDialog::onAnswerClicked()
{
	ApplicationController::fs()->answer();
	accept();
}

void IncomingCallDialog::onRejectClicked()
{
	ApplicationController::fs()->hangup(false);
	reject();
}
