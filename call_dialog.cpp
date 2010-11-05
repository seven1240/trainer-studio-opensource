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

CallDialog::CallDialog(QWidget *parent) : QDialog(parent)
{
	_display = new QTextEdit();
	_call = new QPushButton("New Call");
	_hold = new QPushButton("Hold");
	_answer = new QPushButton("Answer");
	_hangup = new QPushButton("Hangup");

	_display->setEnabled(false);

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
	setLayout(layout);

	setWindowTitle("Call");
	setFixedSize(300, 330);

	connect(_dialPadWidget, SIGNAL(dialed(QString)), this, SLOT(dialed(QString)));
	connect(_call, SIGNAL(clicked()), this, SLOT(onCall()));
	connect(_hold, SIGNAL(clicked()), this, SLOT(onHold()));
	connect(_answer, SIGNAL(clicked()), this, SLOT(onAnswer()));
	connect(_hangup, SIGNAL(clicked()), this, SLOT(onHangup()));
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
