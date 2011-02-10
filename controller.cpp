/*
 * Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
 * Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
 *
 * Version: Apache License 2.0
 *
 * The content of this file is licensed under the Apache License, Version 2.0.  (the "License").
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
#include <QStateMachine>
#include <QDebug>
#include "controller.h"
#include "state_machines.h"

Controller::Controller(QObject *parent) :
	QObject(parent)
{
	_state_machine = NULL;
}

Controller::~Controller()
{
}

QStateMachine *Controller::stateMachine()
{
	if (_state_machine == NULL) {
		_state_machine = jDebugStateMachine(createStateMachine());
	}
	return _state_machine;
}

void Controller::startStateMachine()
{
	stateMachine()->start();
}

QStateMachine *Controller::createStateMachine()
{
	return NULL;
}
