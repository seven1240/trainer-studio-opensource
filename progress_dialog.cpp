#include <QDebug.h>
#include <QMessageBox>
#include <QApplication>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include "progress_dialog.h"
#include "progress_widget.h"

ProgressDialog::ProgressDialog(ProgressWidget *progressWidget, QWidget *parent) : QDialog(parent)
{
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(progressWidget);
	setLayout(layout);

	setWindowTitle("Progress");
	setFixedSize(500, 280);

	QMetaObject::connectSlotsByName(this);
}

ProgressDialog::~ProgressDialog()
{
}
