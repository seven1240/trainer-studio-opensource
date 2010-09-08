#include "isettings.h"
#include <QtGui>

QMutex *ISettings::mutex = new QMutex();
QDomDocument *ISettings::xml = 0;

ISettings::ISettings(QObject *parent) :
    QObject(parent)
{
    if (!(ISettings::xml)) {
        qDebug() << "lock1";
        ISettings::mutex->lock();
        qDebug() << "locked1";

        QFile *f = new QFile(QString("%1/freeswitch.xml").arg(SWITCH_GLOBAL_dirs.conf_dir));
        if ( !f->open(QIODevice::ReadOnly | QIODevice::Text ) ) {
            /* TODO: Let the user know */
            qDebug() << "Could not read from file.";
            goto fail;
        }
        {//run in it's own block
            QString errMsg;
            int errLine = 0, errCol = 0;
            ISettings::xml = new QDomDocument();
            if ( !ISettings::xml->setContent(f, &errMsg, &errLine, &errCol) ) {
                /* TODO: Let the user know */
                qDebug() << "Could not set content";
            }
            f->close();
        }
    fail:
        delete(f);
        ISettings::mutex->unlock();
        qDebug() << "unlocked1";

    }

}

QDomElement ISettings::getConfigNode(QString module) {
    /* We don't need to lock since we are just reading (true?) */
    QDomElement e = ISettings::xml->documentElement();
    QDomNodeList nl = e.elementsByTagName("configuration");
    for(int i = 0; i < nl.count(); i++) {
        QDomElement el = nl.at(i).toElement();
        if ( el.attribute("name") == module ) {
            return el;
        }
    }
    return QDomElement();
}

QString ISettings::getPaRingFile() {

    QDomElement cfg = getConfigNode("portaudio.conf");

    QDomNodeList nodeList = cfg.elementsByTagName("param");

    for (int i = 0; i < nodeList.count(); i++) {
        QDomElement param = nodeList.at(i).toElement();
        if(param.attributeNode("name").value() == "ring-file"){
            return param.attributeNode("value").value();
        }
    }
    return "tone_stream://%(2000,4000,440.0,480.0);loops=20";
}

void ISettings::writePaConfig(QVariantMap newconf)
{
    qDebug() << "lock2";
    ISettings::mutex->lock();
    qDebug() << "locked2";

    QDomElement cfg = getConfigNode("portaudio.conf");

    QDomElement settings = cfg.elementsByTagName("settings").at(0).toElement();

    if(settings.isNull()) {
        qDebug() << "NULL PA settings!";
        goto fail;
    }

    {//run in it's own block, or it will throw jump to label crosses initialization ...
        QDomNodeList params = settings.childNodes();

        for(int i=0; i< params.count(); i++) {
            QDomElement param = params.at(i).toElement();
            QString name = param.attributeNode("name").value();
            QString value = param.attributeNode("value").value();

            if(!newconf[name].toString().isNull() && newconf[name].toString() != value) {
                param.setAttribute("value", newconf[name].toString());
            }
        }
    }

fail:
    ISettings::mutex->unlock();
    qDebug() << "unlocked2";


}

QVariantMap ISettings::getGateway(QString gwname) {

    QDomElement cfg = getConfigNode("sofia.conf");

    QDomNodeList nl = cfg.elementsByTagName("gateway");

    QVariantMap gateway;

    for (int i = 0; i < nl.count(); i++) {
        QDomElement gw = nl.at(i).toElement();
        if (gw.attributeNode("name").value() == gwname) {
            /* Iterate the params and set the values */
            QDomNodeList params = gw.elementsByTagName("param");


            for (int j = 0; j < params.count(); j++) {
                QDomElement param = params.at(j).toElement();
                QString var = param.attributeNode("name").value();
                QString val = param.attributeNode("value").value();
                gateway.insert(var, val);
            }

            /* Stop processing the gateway list */
            break;
        }

    }
    return gateway;
}

void ISettings::writeGateway(QVariantMap newgw)
{
    QDomElement cfg = getConfigNode("sofia.conf");

    QDomElement gws = cfg.elementsByTagName("gateways").at(0).toElement();

    if(gws.isNull()) {
        qDebug() << "NULL gateways tag!";
//        QDomDocument c = cfg.toDocument() ;
////        gws = c.c
//        cfg = c.toElement();
    }


        QDomDocument d = gws.toDocument();
        QDomElement nGw = d.createElement("gateway");
//        gws.clear();
        gws.insertAfter(nGw, QDomNode());
        nGw.setAttribute("name","default");

        /* Set each one of the parameters */
        setParam(nGw, "username", newgw["username"].toString());
        setParam(nGw, "password", newgw["password"].toString());
        setParam(nGw, "register", "true");
        setParam(nGw, "realm", newgw["realm"].toString());
        setParam(nGw, "expire-seconds", "600");
        setParam(nGw, "retry-seconds", "60");


    setConfigNode(cfg, "sofia.conf");

}

void ISettings::resetGateway()
{
    QDomElement cfg = getConfigNode("sofia.conf");

    QDomNode gws = cfg.elementsByTagName("gateways").at(0);

    for(QDomNode c = gws.firstChild(); !c.isNull(); c = gws.firstChild()) {
        gws.removeChild(c);
    }
//    gws.clear();  // this doesn't work?

    setConfigNode(cfg, "sofia.conf");
}

void ISettings::setConfigNode(QDomElement node, QString module) {
    qDebug() << "lock4";
    ISettings::mutex->lock();
    qDebug() << "locked4";

    QDomElement e = ISettings::xml->documentElement();
    QDomNodeList l = e.elementsByTagName("configuration");
    for (int i = 0; i < l.count(); i++) {
        QDomElement el = l.at(i).toElement();
        if ( el.attribute("name") == module ) {
            /* Found the proper module to replace */
            el.parentNode().replaceChild(node.toDocumentFragment(),el);
        }
    }
    ISettings::mutex->unlock();
    qDebug() << "unlocked4";

}

void ISettings::saveToFile() {
    qDebug() << "lock5";
    ISettings::mutex->lock();

    if (ISettings::xml) {
        QFile *f = new QFile(QString("%1/freeswitch.xml").arg(SWITCH_GLOBAL_dirs.conf_dir));
		qDebug() << "Write to: " << QString("%1/freeswitch.xml").arg(SWITCH_GLOBAL_dirs.conf_dir);
        if ( !f->open(QFile::WriteOnly | QFile::Truncate) ) {
            /* TODO: Let the user know */
            qDebug() << "Could not open from file.";
            goto fail;
        }
        QTextStream out(f);
        ISettings::xml->save(out, 2);
        f->close();
        if ( !f->open(QFile::ReadOnly) ) {
            /* TODO: Let the user know */
            qDebug() << "Could not open from file.";
            goto fail;
        }
        QString errMsg;
        int errLine = 0, errCol = 0;
        if ( !ISettings::xml->setContent(f, &errMsg, &errLine, &errCol) ) {
            /* TODO: Let the user know */
            qDebug() << "Could not set content";
        }
        f->flush();
        f->close();
        delete(f);
    }

fail:
    ISettings::mutex->unlock();
    qDebug() << "unlocked5";

}

void ISettings::setParam(QDomElement &parent, QString name, QString value) {
    QDomDocument d = parent.toDocument();
    QDomElement e = d.createElement("param");
    e.setAttribute("name", name);
    e.setAttribute("value", value);
    parent.appendChild(e);
}
