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
private:
    void setParam(QDomElement &parent, QString name, QString value);
    static QDomDocument *xml;
    static QMutex *mutex;
};

#endif // ISETTINGS_H
