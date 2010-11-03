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
class QListView;
class QFrame;
QT_END_NAMESPACE

class ProgressController;

class ProgressWidget : public QWidget {
	Q_OBJECT

public:
	ProgressWidget(ProgressController *progressController, QWidget *parent = 0);
	~ProgressWidget();

private:
	QLabel *_label;
	QListView *_history;
	QPushButton *_cancel;

signals:
	void cancelled();

private slots:
	void onCancelClicked();
	void onHistoryChanged();

private:
	void setProgress(QString string);

};

#endif // PROGRESS_WIDGET_H
