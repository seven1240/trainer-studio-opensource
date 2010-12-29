#include <QtGui>
#include <QTextEdit>
#include <QLineEdit>
#include "fsconsole_dialog.h"
#include "application_controller.h"
#include "utils.h"

FSConsoleDialog::FSConsoleDialog(QWidget *parent) :
	QDialog(parent)
{
	_consoleBox = new QTextEdit();
	_consoleBox->setReadOnly(true);

	_commandLine = new QLineEdit();

//	QHBoxLayout *toolBar = new QHBoxLayout();
	QPushButton *clear = new QPushButton("Clear");

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(clear);
	layout->addWidget(_consoleBox);
	layout->addWidget(_commandLine);
	setLayout(layout);

	this->setMinimumSize(640, 480);
	Utils::centerWindowOnDesktop(this);

	connect(clear, SIGNAL(clicked()), this, SLOT(onClear()));
	connect(_commandLine, SIGNAL(returnPressed()), this, SLOT(onSendCommand()));
	connect(ApplicationController::fs(),
			SIGNAL(eventLog(QSharedPointer<switch_log_node_t>,switch_log_level_t)),
			this,
			SLOT(onEventLog(QSharedPointer<switch_log_node_t>,switch_log_level_t)));
}

FSConsoleDialog::~FSConsoleDialog()
{
	delete _consoleBox;
	delete _commandLine;
}

void FSConsoleDialog::onClear()
{
	_consoleBox->clear();
}

void FSConsoleDialog::onSendCommand()
{
	QString cmd, args, res;
	QStringList cmdList = _commandLine->text().split(" ");

	if (cmdList.isEmpty()) return;

	_consoleBox->append(">>> " + _commandLine->text());

	cmd = cmdList[0];
	cmdList.removeFirst();
	args = cmdList.join(" ");
	ApplicationController::fs()->command(cmd.toAscii(), args.toAscii(), &res);
	_consoleBox->append(">>> " + res);
}

void FSConsoleDialog::onEventLog(QSharedPointer<switch_log_node_t> node,switch_log_level_t level)
{
	QString color = "";

	switch (level) {
	case SWITCH_LOG_DEBUG:
			color = "#ffaa56"; break;
	case SWITCH_LOG_ALERT:
	case SWITCH_LOG_CRIT:
	case SWITCH_LOG_ERROR:
			color = "#FF0000"; break;
	case SWITCH_LOG_WARNING:
			color = "#ff56aa"; break;
	case SWITCH_LOG_NOTICE:
			color = "#0000ff"; break;
	case SWITCH_LOG_INFO:
			color = "#00ffff"; break;
	default:
			color = "#000000";
	}
	_consoleBox->insertHtml("<span style='color:" + color + "'>" +
		QString(node.data()->data).trimmed().replace("\n", "<br>") +
		"</span><br>\n");

	QScrollBar *sb = _consoleBox->verticalScrollBar();
	sb->setValue(sb->maximum());
}
