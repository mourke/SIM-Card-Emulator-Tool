#include "Windows/MainWindow.h"
#include "Version.h"
#include <QFontDatabase>
#include "FontSizes.h"

#if defined(Q_OS_WIN)
#include "singleapplication.h"
#include <windows.h>
#include <WinUser.h>
#elif defined(Q_OS_MAC)
#include "OpenFileApplication.h"
#endif


int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);

#if defined(Q_OS_WIN)
	SetProcessDPIAware(); // must be called before the main event loop
    SingleApplication application(argc, argv, true, SingleApplication::Mode::SecondaryNotification);

    if (application.isSecondary()) {
        AllowSetForegroundWindow(DWORD(application.primaryPid()));
        application.sendMessage(application.arguments().join("»").toUtf8());
        exit(0);
    }
#elif defined(Q_OS_MAC)
    OpenFileApplication application(argc, argv);
#endif

    QApplication::setOrganizationName(VER_COMPANYNAME_STR);
    QApplication::setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    QApplication::setApplicationName(VER_FILEDESCRIPTION_STR);
    QApplication::setApplicationVersion(VER_FILEVERSION_STR);

    if (!QFontDatabase().hasFamily("Avenir")) {
        QFontDatabase::addApplicationFont(":/Fonts/Avenir");
    }

    QApplication::setFont(QFont("Avenir", FONT_SIZE_NORMAL, QFont::Black));

	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();

#if defined(Q_OS_WIN)

	if (application.isPrimary()) {
		QObject::connect(&application, &SingleApplication::instanceStarted, [mainWindow]() {
			if (mainWindow->isMinimized()) {
				mainWindow->showNormal();
			} else {
                mainWindow->activateWindow();
			}
		});
	}

#endif

	return application.exec();
}
