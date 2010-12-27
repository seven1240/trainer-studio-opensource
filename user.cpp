#include "user.h"

User::User(QVariantMap &details)
{
	_details = details;
}

QString User::getVoipServer()
{
	QVariantList servers = _details["servers"].toList();
	QVariantMap server = servers[0].toMap();
	return QString("%1:%2").arg(server["sip_proxy"].toString(), server["sip_port"].toString());
}

User::~User()
{
}
