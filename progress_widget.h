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
class QStringListModel;
QT_END_NAMESPACE

class ProgressController;

class ProgressWidget : public QWidget {
	Q_OBJECT

public:
	ProgressWidget(QWidget *parent = 0);
	~ProgressWidget();

public:
	void setModel(QStringListModel *model);

private:
	QLabel *_label;
	QListView *_history;
	QPushButton *_cancel;

signals:
	void cancelled();

public slots:
	void onHistoryChanged();

private slots:
	void onCancelClicked();

private:
	void setProgress(QString string);

};

#endif // PROGRESS_WIDGET_H
