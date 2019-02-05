#include "Windows/MainWindow.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[]) {
	QApplication application(argc, argv);
	
	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();

	return application.exec();
}
