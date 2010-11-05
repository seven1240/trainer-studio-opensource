#ifndef DIAL_PAD_WIDGET_H
#define DIAL_PAD_WIDGET_H

#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE
class QKeyEvent;
QT_END_NAMESPACE

class DialPadWidget : public QWidget {
	Q_OBJECT

public:
	DialPadWidget(QWidget *parent = 0);
	~DialPadWidget();

public:

signals:
	void dialed(QString dtmf);

public slots:
	void pressed(QString dtmf);

};

#endif // DIAL_PAD_WIDGET_H
