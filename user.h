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
	QString getVoipUsername() { return _details["voip_username"].toString(); }
	QString getVoipPassword() { return _details["voip_password"].toString(); }
	QString getServerVersion() { return _details["version"].toString(); }
	QString getVoipServer();
	bool skipEchoTesting() { return _details["skip_echo_test"].toString() == "true"; }
};

#endif // USER_H
