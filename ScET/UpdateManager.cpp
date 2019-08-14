#include "UpdateManager.h"
#include <QSettings>
#include <SingleApplication.h>


UpdateManager::UpdateManager() {
	QSettings settings;
	settings.beginGroup("UpdateManager");

	QVariant skipReleaseVersion = settings.value("skipReleaseVersion");
	if (!skipReleaseVersion.isNull()) {
		this->skipReleaseVersion = skipReleaseVersion.toString();
	}

	QVariant lastVersionCheckPerformedOnDate = settings.value("lastVersionCheckPerformedOnDate");
	if (!skipReleaseVersion.isNull()) {
		this->lastVersionCheckPerformedOnDate = lastVersionCheckPerformedOnDate.toDate();
	}

	settings.endGroup();
	QObject::connect(&checkForUpdatesProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishedCheckingForUpdates(int, QProcess::ExitStatus)));
}

int UpdateManager::daysSinceLastVersionCheckDate() const {
	if (!lastVersionCheckPerformedOnDate.has_value()) return 0;
	return QDate::currentDate().daysTo(lastVersionCheckPerformedOnDate.value());
}

void UpdateManager::setSkipReleaseVersion(const QString &version) {
	this->skipReleaseVersion = version;
	QSettings settings;
	settings.beginGroup("UpdateManager");
	settings.setValue("skipReleaseVersion", version);
	settings.endGroup();
}

void UpdateManager::setLastVersionCheckPerformedOnDate(const QDate &date) {
	this->lastVersionCheckPerformedOnDate = date;
	QSettings settings;
	settings.beginGroup("UpdateManager");
	settings.setValue("lastVersionCheckPerformedOnDate", date);
	settings.endGroup();
}

void UpdateManager::checkForUpdates(std::optional<Callback> callback) {
	if (checkForUpdatesProcess.state() != QProcess::ProcessState::NotRunning) { // user is already checking for updates
		if (callback.has_value()) std::invoke(callback.value(), std::nullopt);
		return;
	}
	m_callback = callback;
	checkForUpdatesProcess.start("Uninstaller --checkupdates");
}

void UpdateManager::finishedCheckingForUpdates(int exitCode, QProcess::ExitStatus exitStatus) {
	if (exitStatus != QProcess::ExitStatus::NormalExit) {
		if (m_callback.has_value()) std::invoke(m_callback.value(), std::nullopt);
		return;
	}

	setLastVersionCheckPerformedOnDate(QDate::currentDate());

	SingleApplication *application = (SingleApplication *)SingleApplication::instance();
	QByteArray data = checkForUpdatesProcess.readAllStandardOutput();


	if (data.isEmpty()) { // no updates available
		if (m_callback.has_value()) std::invoke(m_callback.value(), std::nullopt);
		return;
	}

	QString version(data);
	version = version.split("version=\"").last().split("\"").first(); // extract version number
	UpdateAction action = std::invoke(m_callback.value(), version);

	switch (action) {
	case UpdateAction::Update: {
		QStringList args("--updater");
		bool success = QProcess::startDetached("Uninstaller", args);
		if (success) application->closeAllWindows();
		break;
	}
	case UpdateAction::Skip:
		setSkipReleaseVersion(version);
	case UpdateAction::DoNothing:
		break;
	}

	m_callback.reset();
	
}

UpdateManager::~UpdateManager() {
	QObject::disconnect(&checkForUpdatesProcess);
}
