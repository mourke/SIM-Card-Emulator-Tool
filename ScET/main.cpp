#include <SingleApplication.h>
#include "Windows/MainWindow.h"
#include <windows.h>
#include <WinUser.h>
#include "Version.h"
#include <QFontDatabase>


int main(int argc, char *argv[]) {
#ifdef Q_OS_WIN
	SetProcessDPIAware(); // must be called before the main event loop
#endif

	SingleApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
	SingleApplication application(argc, argv, true, SingleApplication::Mode::SecondaryNotification);

	if (application.isSecondary()) {
		AllowSetForegroundWindow(DWORD(application.primaryPid()));
		application.sendMessage(application.arguments().join("»").toUtf8());
		exit(0);
	}

	SingleApplication::setOrganizationName(VER_COMPANYNAME_STR);
	SingleApplication::setOrganizationDomain(VER_COMPANYDOMAIN_STR);
	SingleApplication::setApplicationName(VER_FILEDESCRIPTION_STR);
	SingleApplication::setApplicationVersion(VER_FILEVERSION_STR);

	QFontDatabase::addApplicationFont(":/Fonts/Avenir");

	SingleApplication::setFont(QFont("Avenir", 8, QFont::Black));
	
	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();

	if (application.isPrimary()) {
		QObject::connect(&application, &SingleApplication::instanceStarted, [mainWindow]() {
			if (mainWindow->isMinimized()) {
				mainWindow->showNormal();
			} else {
				mainWindow->activateWindow();
			}
		});
	}

	return application.exec();
}
