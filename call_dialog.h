#ifndef CALL_DIALOG_H
#define CALL_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QPushButton;
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
	QPushButton *_call;
	QPushButton *_hold;
	QPushButton *_answer;
	QPushButton *_hangup;

private slots:
	void dialed(QString dtmf);
	void onCall();
	void onHold();
	void onAnswer();
	void onHangup();

};

#endif // CALL_DIALOG_H
