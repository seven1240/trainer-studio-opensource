#ifndef CALL_DIALOG_H
#define CALL_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QLineEdit;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class DialPadWidget;

class CallDialog : public QDialog {
	Q_OBJECT

public:
	CallDialog(QWidget *parent = 0);
	~CallDialog();

protected:
	void keyPressEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *event);

signals:
	void closed();

private:
	DialPadWidget *_dialPadWidget;
	QTextEdit *_display;
	QLineEdit *_number;
	QPushButton *_call;
	QPushButton *_hold;
	QPushButton *_answer;
	QPushButton *_hangup;
	QLabel *_sipStatusLabel;

private slots:
	void dialed(QString dtmf);
	void onCall();
	void onHold();
	void onAnswer();
	void onHangup();

	void onGatewayStateChange(QString, QString);
	void onCallIncoming(QString, QString, QString);
	void onCallAnswered(QString, QString, QString);
	void onCallEnded(QString, QString, QString);

};

#endif // CALL_DIALOG_H
