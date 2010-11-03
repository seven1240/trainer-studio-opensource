#ifndef ECHO_TEST_DIALOG_H
#define ECHO_TEST_DIALOG_H

#include <QDialog>
#include "freeswitch.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class EchoTestDialog : public QDialog {
	Q_OBJECT
public:
	EchoTestDialog(QWidget *parent = 0);
	~EchoTestDialog();

protected:
	void changeEvent(QEvent *e);
	void showEvent(QShowEvent *e);
	void closeEvent(QCloseEvent *e);

private:
	QPushButton *_pbBegin;
	QPushButton *_pbSkip;
	QPushButton *_pbFinish;
	QLabel *_lbProgress;

private:
	void setProgress(QString string);
	bool didTheyHearThemselves();

private slots:
	void onSkipClicked();
	void onBeginClicked();
	void onFinishClicked();
	void onNewEvent(QSharedPointer<switch_event_t>);
};

#endif // ECHO_TEST_DIALOG_H
