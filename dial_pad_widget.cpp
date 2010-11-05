#include <QDebug.h>
#include <QMessageBox>
#include <QApplication>
#include <QSignalMapper>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>
#include <QtGui/QShortcut>
#include <QtGui/QKeyEvent>
#include "application_controller.h"
#include "dial_pad_widget.h"

DialPadWidget::DialPadWidget(QWidget *parent) :
	QWidget(parent)
{
    QSignalMapper *mapper = new QSignalMapper(this);

	QString pad[4][3] = {
		{ "1", "2", "3" },
		{ "4", "5", "6" },
		{ "9", "8", "9" },
		{ "*", "0", "#" },
	};

	QGridLayout *layout = new QGridLayout();
	for (short i = 0; i < 4; ++i) {
		for (short j = 0; j < 3; ++j) {
			QPushButton *button = new QPushButton(pad[i][j]);
			button->setFixedSize(40, 30);
			connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
			mapper->setMapping(button, pad[i][j]);
			layout->addWidget(button, i, j, 1, 1, Qt::AlignHCenter);
			QShortcut *shortcut = new QShortcut(pad[i][j], this);
			connect(shortcut, SIGNAL(activated()), button, SLOT(click()));
		}
	}
	setLayout(layout);

	connect(mapper, SIGNAL(mapped(QString)), this, SLOT(pressed(QString)));
}

void DialPadWidget::pressed(QString dtmf)
{
	emit dialed(dtmf);
}

DialPadWidget::~DialPadWidget()
{
}
