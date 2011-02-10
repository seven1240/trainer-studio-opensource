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
#ifndef ISETTINGS_H
#define ISETTINGS_H

#include <QObject>
#include <QtXml>
#include <switch.h>
#include "trainer_studio.h"

class ISettings : public QObject {
	Q_OBJECT

public:
	ISettings(QObject *parent = 0);
	QDomElement getConfigNode(QString module);
	void setConfigNode(QDomElement node, QString module);
	void saveToFile();
	QVariantMap getGateway(QString name);
	void writeGateway(QVariantMap newgw);
	void resetGateway();
	QString getPaRingFile();
	void writePaConfig(QVariantMap newconf);

private:
	void setParam(QDomElement &parent, QString name, QString value);
	static QDomDocument *xml;
	static QMutex *mutex;
};

#endif // ISETTINGS_H
