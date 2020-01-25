#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include <optional>
#include <functional>
#include <QObject>
#include <QString>
#include <QDate>
#include <QProcess>

class UpdateManager : private QObject {
	Q_OBJECT

public:
	/**
	 * Determines the frequency in which the the version check is performed.
	 *
	 *	- Immediately:    Version check performed every time the app is launched.
	 *	- Daily:          Version check performedonce a day.
	 *	- Weekly:         Version check performed once a week.
	 */
	enum class CheckFrequency {
		Immediately,
		Daily,
		Weekly = 7
	};

	/**
	 * Determines the action that should be taken after an update is found.
	 *
	 *	- Skip:			Skip this version and stop reminding the user of this version.
	 *	- DoNothing:	Do nothing and keep reminding the user of this version.
	 *	- Update:		Close the application and begin the update.
	 */
	enum class UpdateAction {
		Skip,
		DoNothing,
		Update
	};

	/** The shared manager instance */
	static UpdateManager & sharedManager() {
		static UpdateManager instance;
		return instance;
	}

	/** 
	 * The function called after updates have been checked for.
	 * 
	 * @param	The version number of the update.
	 *
	 * @retval	The action that should be taken.
	 */
	typedef std::function<UpdateAction(std::optional<QString>)> Callback;

	/** 
	 * Checks for new releases.
	 * 
	 * @param	frequency	The frequency with which to check.
	 * @param	callback	The function called when updates have been checked.
	 */
	void checkVersion(CheckFrequency frequency, std::optional<Callback> callback = std::nullopt) {
		if (frequency == CheckFrequency::Immediately || static_cast<int>(frequency) <= daysSinceLastVersionCheckDate()) {
			checkForUpdates(callback);
		} else if (callback.has_value()) {
			std::invoke(callback.value(), std::nullopt);
		}
	}
private:
	/** 
	 * The version that the user does not want installed. If the user has never 
	 * skipped an update, this variable will be nullopt, otherwise it will be
	 * the last version that the user opted not to install. 
	 */
	std::optional<QString> skipReleaseVersion;

	/** The date on which updates were last checked. */
	std::optional<QDate> lastVersionCheckPerformedOnDate;

	void setSkipReleaseVersion(const QString &version);
	void setLastVersionCheckPerformedOnDate(const QDate &date);

	/**
	 * @retval	The number of days it has been since the application
	 * has been checked for updates.
	 */
	int daysSinceLastVersionCheckDate() const;

	void checkForUpdates(std::optional<Callback> callback);
	

	QProcess checkForUpdatesProcess;
	std::optional<Callback> m_callback;
					
	UpdateManager();
	~UpdateManager();

	Q_DISABLE_COPY(UpdateManager)
private slots:
	void finishedCheckingForUpdates(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // UPDATE_MANAGER_H

