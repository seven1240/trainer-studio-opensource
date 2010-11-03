#include <QDebug.h>
#include <QMessageBox>
#include <QApplication>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include "application_controller.h"
#include "progress_widget.h"

ProgressWidget::ProgressWidget(QWidget *parent) :
	QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();
	_label = new QLabel();
	_history = new QTextEdit();
	_cancel = new QPushButton("Cancel");

	_label->setObjectName("Label");
	_history->setObjectName("History");
	_cancel->setObjectName("Cancel");

	layout->addWidget(_label);
	layout->addWidget(_history);
	layout->addWidget(_cancel);
	setLayout(layout);

	setWindowTitle("Progress");

	QMetaObject::connectSlotsByName(this);
}

ProgressWidget::~ProgressWidget()
{
}

void ProgressWidget::setProgress(QString string)
{
	_label->setText(string);
	_label->repaint();
}

void ProgressWidget::on_Cancel_clicked()
{
	emit cancelled();
}
