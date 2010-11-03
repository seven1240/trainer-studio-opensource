#ifndef PROGRESS_WIDGET_H
#define PROGRESS_WIDGET_H

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

class ProgressWidget : public QWidget {
	Q_OBJECT

public:
	ProgressWidget(QWidget *parent = 0);
	~ProgressWidget();

private:
	QLabel *_label;
	QTextEdit *_history;
	QPushButton *_cancel;

signals:
	void cancelled();

private slots:
	void on_Cancel_clicked();

private:
	void setProgress(QString string);

};

#endif // PROGRESS_WIDGET_H
