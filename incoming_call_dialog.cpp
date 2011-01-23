#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QShortcut>

#include "application_controller.h"
#include "incoming_call_dialog.h"
#include "freeswitch.h"
#include "utils.h"

IncomingCallDialog::IncomingCallDialog(QWidget *parent) :
	QDialog(parent)
{
	QGridLayout *layout = new QGridLayout();
	_answer = new QPushButton("&Answer (Ctrl-A)");
	_reject = new QPushButton("Hangup");
	_status = new QLabel();
	layout->addWidget(_status, 0, 0, 1, 4, Qt::AlignHCenter);
	layout->addWidget(_reject, 1, 0, 1, 1, Qt::AlignLeft | Qt::AlignBottom);
	layout->addWidget(_answer, 1, 1, 1, 3, Qt::AlignRight | Qt::AlignBottom);
	setLayout(layout);

	QShortcut *ctrlA = new QShortcut(QKeySequence(tr("Ctrl+A")), this);
	QShortcut *commandA = new QShortcut(QKeySequence(tr("Cmd+A")), this);
	_answer->setFixedWidth(200);

	setFixedSize(320, 140);
	setWindowTitle("Incoming Call");
	setWindowFlags(Qt::WindowStaysOnTopHint);

	Utils::centerWindowOnDesktop(this);

	connect(ctrlA, SIGNAL(activated()), this, SLOT(onAnswerClicked()));
	connect(commandA, SIGNAL(activated()), this, SLOT(onAnswerClicked()));
	connect(_answer, SIGNAL(clicked()), this, SLOT(onAnswerClicked()));
	connect(_reject, SIGNAL(clicked()), this, SLOT(onRejectClicked()));
	connect(ApplicationController::fs(), SIGNAL(callIncoming(QString,QString,QString)), this, SLOT(onIncomingCall(QString,QString,QString)));
}

IncomingCallDialog::~IncomingCallDialog()
{
}

void IncomingCallDialog::showEvent(QShowEvent * /*e*/)
{
	_answer->setFocus();
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

void IncomingCallDialog::onIncomingCall(QString /*uuid*/, QString cidNumber, QString cidName)
{
	_callerName = cidName;
	_callerNumber = cidNumber;
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
	ApplicationController::fs()->hangup();
	reject();
}
