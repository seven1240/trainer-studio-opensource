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
 * Seven Du <seven@eleutian.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 */
#include <QDebug>
#include "qJSON.h"

qJSON::qJSON()
{
	_json = NULL;
}

qJSON::~qJSON()
{
	if (_json) cJSON_Delete(_json);
}

void qJSON::parse(char *str, bool *ok)
{
	_json = cJSON_Parse(str);
	*ok = _json ? 1 : 0;
}

QVariantMap qJSON::toMap()
{
	return toMap(_json, 5);
}

QVariantMap qJSON::toMap(cJSON* json_object, int max_depth)
{
	QVariantMap map;

	if (max_depth < 0)
		return map;

	for (cJSON *j = json_object->child; j; j = j->next) {
		char *name = j->string;
		QVariant value;
		switch ((j->type) & 255)
		{
		case cJSON_NULL:	  value = "null"; break;
		case cJSON_False:	  value = false; break;
		case cJSON_True:	  value = true; break;
		case cJSON_Number:	value = j->valuedouble; break;
		case cJSON_String:	value = j->valuestring; break;
		case cJSON_Object:	value = toMap(j, max_depth - 1); break;
		case cJSON_Array:
							{
								QVariantList list;
								for (cJSON *item = j->child; item; item = item->next) {
									list << toMap(item, max_depth - 1);
								}
								value = list;
							}
		}
		if (name) {
			map.insert(name, value);
		}
	}

	return map;
}

