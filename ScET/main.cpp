#include "Windows/MainWindow.h"
#include <QtWidgets/QApplication>
#include "Tracer/TracerManager.h"
#include "Tracer/Tracer.h"

int main(int argc, char *argv[]) {
	QApplication application(argc, argv);

	QCoreApplication::setOrganizationName("CardCentric");
	QCoreApplication::setOrganizationDomain("com.cardcentric");
	QCoreApplication::setApplicationName("ScET");
	
	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();

	std::optional<Tracer *> tracer = TracerManager::sharedManager().findTracer();

	if (tracer.has_value()) {
		tracer.value()->startSniffing();
	}

	return application.exec();
}
