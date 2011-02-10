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
 * Seven Du <seven@eleutian.com>
 * Portions created by the Initial Developer are Copyright (C)
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Jadob Lewallen <jacob@eleutian.com>
 */
#ifndef CHANNEL_H
#define CHANNEL_H

#include <QtCore>

class Channel
{
public:
	Channel() {}
	Channel(QString uuid);
	QString getUuid() { return _uuid; }
	void setCidName(QString cidName) { _cidName = cidName; }
	QString getCidName() { return _cidName; }
	void setCidNumber(QString cidNumber) { _cidNumber = cidNumber; }
	QString getCidNumber() { return _cidNumber; }
	void setDestinatinonNumber(QString destinationNumber) { _destinationNumber = destinationNumber; }
	QString getDestinationNumber() { return _destinationNumber; }

	int getPaCallId() { return _paCallId; }
	void setPaCallId(int paCallId) { _paCallId = paCallId;}

	void setProgressEpoch(qulonglong time) { _progressEpoch = time/1000000; }
	qulonglong getProgressEpoch() { return _progressEpoch; }
	void setProgressMediaEpoch(qulonglong time) { _progressMediaEpoch = time/1000000; }
	qulonglong getProgressMediaEpoch() { return _progressMediaEpoch; }
	void setCreatedEpoch(qulonglong time) { _createdEpoch = time/1000000; }
	qulonglong getCreatedEpoch() { return _createdEpoch; }

private:
	QString _uuid;
	QString _cidName;
	QString _cidNumber;
	QString _destinationNumber;
	int _paCallId;
	qulonglong _progressEpoch;
	qulonglong _progressMediaEpoch;
	qulonglong _createdEpoch;
};

Q_DECLARE_METATYPE(Channel)

#endif // CHANNEL_H
