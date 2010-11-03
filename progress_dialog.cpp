#include <QDebug.h>
#include <QMessageBox>
#include <QApplication>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include "progress_dialog.h"
#include "progress_widget.h"

ProgressDialog::ProgressDialog(QWidget *parent) : QDialog(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(new ProgressWidget());
	setLayout(layout);

	setWindowTitle("Progress");
	setFixedSize(480, 280);

	QMetaObject::connectSlotsByName(this);
}

ProgressDialog::~ProgressDialog()
{
}
