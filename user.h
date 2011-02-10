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
#ifndef USER_H
#define USER_H

#include <QObject>
#include <QVariantMap>
#include <QVariantList>

class User : public QObject
{
	Q_OBJECT

private:
	QVariantMap _details;

public:
	User(QVariantMap &details);
	~User();
	QVariantMap &details() { return _details; }
	QString getLogin() { return _details["login"].toString(); }
	QString getPassword() { return _details["password"].toString(); }
	QString getVoipUsername() { return _details["voip_username"].toString(); }
	QString getVoipPassword() { return _details["voip_password"].toString(); }
	QString getServerVersion() { return _details["version"].toString(); }
	QString getVoipServer();
	bool skipEchoTesting() { return _details["skip_echo_test"].toString() == "true"; }
};

#endif // USER_H
