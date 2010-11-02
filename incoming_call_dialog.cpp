#include "incoming_call_dialog.h"
#include "ui_incoming_call_dialog.h"
#include "fs_host.h"

IncomingCallDialog::IncomingCallDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::IncomingCallDialog)
{
	ui->setupUi(this);
	this->connect(fs, SIGNAL(incomingCall(QSharedPointer<switch_event_t>)), this, SLOT(onIncomingCall(QSharedPointer<switch_event_t>)));
}

IncomingCallDialog::~IncomingCallDialog()
{
	delete ui;
}

void IncomingCallDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void IncomingCallDialog::onIncomingCall(QSharedPointer<switch_event_t>event)
{
	qDebug() << "incoming call";
	_cid_name = QString(switch_event_get_header_nil(event.data(), "Caller-Caller-ID-Name"));
	_cid_number = QString(switch_event_get_header_nil(event.data(), "Caller-Caller-ID-Number"));
	ui->lbCallerID->setText(QString("\"%1\" <%2>").arg(_cid_name).arg(_cid_number));
	show();
	raise();
	activateWindow();
}

void IncomingCallDialog::on_pbAnswer_clicked()
{
	fs->answer();

	emit answered(_cid_name, _cid_number);
	lower();
	hide();
}

void IncomingCallDialog::on_pbReject_clicked()
{
	fs->hangup(false);
	lower();
	hide();
}
