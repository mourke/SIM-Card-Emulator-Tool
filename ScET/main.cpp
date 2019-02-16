#include "Windows/MainWindow.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[]) {
	QApplication application(argc, argv);

	QCoreApplication::setOrganizationName("CardCentric");
	QCoreApplication::setOrganizationDomain("com.cardcentric");
	QCoreApplication::setApplicationName("ScET");
	
	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();

	return application.exec();
}
