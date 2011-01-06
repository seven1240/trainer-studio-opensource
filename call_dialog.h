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
	void showEvent(QShowEvent *e);

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
	QPushButton *_conf;
	QPushButton *_n800;
	QPushButton *_echo;
	QLabel *_sipStatusLabel;

private slots:
	void dialed(QString dtmf);
	void onCall();
	void onHold();
	void onAnswer();
	void onHangup();
	void onCallConf();
	void onCall800();
	void onCallEcho();

	void onGatewayStateChange(QString gwName, QString gwState);
	void onCallIncoming(QString uuid, QString cidNumber, QString cidName);
	void onCallAnswered(QString uuid, QString cidNumber, QString cidName);
	void onCallEnded(QString uuid, QString cidNumber, QString cidName);

};

#endif // CALL_DIALOG_H
