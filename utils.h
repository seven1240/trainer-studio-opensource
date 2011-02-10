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
 * Jadob Lewallen <jacob@eleutian.com>
 */
#ifndef UTILS_H
#define UTILS_H

#include "trainer_studio.h"

namespace Utils
{
	void centerWindowOnDesktop(QWidget *window);
	void msgbox(QString msg);
	QVariantMap getSystemInfos();
	void trim_json(char *);
	QString formatCallerID(QString cidName, QString CidNumber);
	/* compare version in <major.minor.rev>, returns
		3 - major2 > major1
		2 - minor2 > minor1
		1 - rev2 > rev1
		0 - equel
		-1 - rev2 < rev1
		-2 - error
	*/
	int compareVersion(QString ver1, QString ver2);
	void openFolder(QString path);
	QString getCacheDir();
	void clearWebCache();
}

#endif // UTILS_H
