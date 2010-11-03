#include <QDebug.h>
#include <QMessageBox>
#include <QApplication>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include "application_controller.h"
#include "progress_controller.h"
#include "progress_widget.h"

ProgressWidget::ProgressWidget(QWidget *parent) :
	QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();
	_label = new QLabel();
	_history = new QListView();
	_cancel = new QPushButton("Cancel");

	_label->setObjectName("Label");
	_history->setObjectName("History");
	_cancel->setObjectName("Cancel");

	_cancel->setVisible(false);
	_label->setVisible(false);

	layout->addWidget(_label);
	layout->addWidget(_history);
	layout->addWidget(_cancel);
	setLayout(layout);

	connect(_cancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));

	setWindowTitle("Progress");

	QMetaObject::connectSlotsByName(this);
}

ProgressWidget::~ProgressWidget()
{
}

void ProgressWidget::setModel(QStringListModel *model)
{
	_history->setModel(model);
}

void ProgressWidget::onHistoryChanged()
{
	_history->scrollToBottom();
}

void ProgressWidget::setProgress(QString string)
{
	_label->setText(string);
	_label->repaint();
}

void ProgressWidget::onCancelClicked()
{
	emit cancelled();
}
