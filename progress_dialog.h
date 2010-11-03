#ifndef PROGRESS_DIALOG_H
#define PROGRESS_DIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QFrame;
QT_END_NAMESPACE

class ProgressDialog : public QDialog {
	Q_OBJECT

public:
	ProgressDialog(QWidget *parent = 0);
	~ProgressDialog();

private:
	QLabel *_label;
	QTextEdit *_history;

protected:

private:
	void setProgress(QString string);

};

#endif // PROGRESS_DIALOG_H
