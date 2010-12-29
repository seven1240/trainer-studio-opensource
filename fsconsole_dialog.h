#ifndef FSCONSOLEDIALOG_H
#define FSCONSOLEDIALOG_H

#include <QDialog>
#include <QVariant.h>
#include <QDateTime.h>
#include "freeswitch.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
class QLineEdit;
QT_END_NAMESPACE

class FSConsoleDialog : public QDialog {
	Q_OBJECT

public:
	FSConsoleDialog(QWidget *parent = 0);
	~FSConsoleDialog();

protected:

public slots:

private slots:
	void onSendCommand();
	void onEventLog(QSharedPointer<switch_log_node_t>,switch_log_level_t);
	void onClear();

signals:
	void closed();

private:
	QTextEdit *_consoleBox;
	QLineEdit *_commandLine;
};

#endif // FSConsoleDIALOG_H
