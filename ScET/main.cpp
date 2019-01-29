#include "Windows/MainWindow.h"
#include "Widgets/Frame.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[]) {
	QApplication application(argc, argv);
	Frame frame;

	MainWindow *mainWindow = new MainWindow(frame.contentWidget());

	frame.show();
	return application.exec();
}
