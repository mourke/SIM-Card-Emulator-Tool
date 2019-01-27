#include "Windows/MainWindow.h"
#include "Widgets/Frame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication application(argc, argv);
	Frame frame;
	frame.move(0, 0);

	MainWindow *mainWindow = new MainWindow(frame.contentWidget());

	frame.show();
	return application.exec();
}
