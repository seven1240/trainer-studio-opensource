#ifndef TRAINER_STUDIO_H
#define TRAINER_STUDIO_H

#include "qdebug.h"
#include <qsettings.h>
#include <QSystemTrayIcon>


#ifdef _WIN32
#define DOTDIR "trainer_studio"
#else
#define DOTDIR ".trainer_studio"
#endif

#define TONE_BUSY "tone_stream://%(500,500,480,620);loops=2"

#define TS_DOWNLOAD_URL "http://www.eqenglish.com/trainer/helps/download"
extern QSystemTrayIcon *sysTray;

#endif //TRAINER_STUIDIO_H
