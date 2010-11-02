#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

#include <QObject>

class MainWindow;

class ApplicationController : public QObject
{
	Q_OBJECT

public:
	 ApplicationController();
	 ~ApplicationController();

private:
	MainWindow *_main_window;

public:
	int32_t run();

};

#endif // APPLICATION_CONTROLLER_H
