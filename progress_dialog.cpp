#include <QDebug.h>
#include <QMessageBox>
#include <QApplication>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include "application_controller.h"
#include "progress_dialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
	QDialog(parent)
{
	_label = new QLabel();
	_history = new QTextEdit();

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(_label);
	layout->addWidget(_history);
	setLayout(layout);

	setFixedSize(480, 280);
	setWindowTitle("Progress");

	QMetaObject::connectSlotsByName(this);
}

ProgressDialog::~ProgressDialog()
{
}

void ProgressDialog::setProgress(QString string)
{
	_label->setText(string);
	_label->repaint();
}
