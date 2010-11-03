#ifndef INCOMING_CALL_DIALOG_H
#define INCOMING_CALL_DIALOG_H

#include <QDialog>
#include <switch.h>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class IncomingCallDialog : public QDialog {
	Q_OBJECT

public:
	IncomingCallDialog(QWidget *parent = 0);
	~IncomingCallDialog();

protected:
	void changeEvent(QEvent *e);

private slots:
	void onRejectClicked();
	void onAnswerClicked();
	void onIncomingCall(QString uuid, QString number, QString name);

private:
	QPushButton *_answer;
	QPushButton *_reject;
	QLabel *_status;
	QString _callerName;
	QString _callerNumber;
};

#endif // INCOMING_CALL_DIALOG_H
