#include <SingleApplication.h>
#include "Windows/MainWindow.h"
#include "Widgets/MainFrame.h"
#include <windows.h>
#include <WinUser.h>


int main(int argc, char *argv[]) {
	SingleApplication application(argc, argv, true, SingleApplication::Mode::SecondaryNotification);

	if (application.isSecondary()) {
		AllowSetForegroundWindow(DWORD(application.primaryPid()));
		application.sendMessage(application.arguments().join("»").toUtf8());
		exit(0);
	}

	SingleApplication::setOrganizationName("CardCentric");
	SingleApplication::setOrganizationDomain("com.cardcentric");
	SingleApplication::setApplicationName("ScET");
	SingleApplication::setApplicationVersion("0.1.0-1");
	
	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();

	if (application.isPrimary()) {
		QObject::connect(&application, &SingleApplication::instanceStarted, [mainWindow]() {
			QFrame *frame = mainWindow->frame();
			if (frame->isMinimized()) {
				frame->showNormal();
			} else {
				frame->activateWindow();
			}
		});
	}

	return application.exec();
}
