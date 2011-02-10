/*
 * Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
 * Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
 *
 * Version: Apache License 2.0
 *
 * The content of this file is licensed under the Apache License, Version 2.0. (the "License").
 * You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Eleutian Trainer Studio Application Opensource Edition.
 *
 * The Initial Developer of the Original Code is
 * Jadob Lewallen <jacob@eleutian.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Seven Du <seven@eleutian.com>
 */
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
		{ "7", "8", "9" },
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
