#include <SingleApplication.h>
#include "Windows/MainWindow.h"
#include <windows.h>
#include <WinUser.h>
#include "Version.h"


int main(int argc, char *argv[]) {
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
