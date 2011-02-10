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
#include "application_controller.h"
#include "flash_controller.h"
#include "freeswitch.h"

FlashController::FlashController(FlashDialog *view, QObject *parent) : Controller(parent)
{
	_view = view;
}

FlashController::~FlashController()
{
}
