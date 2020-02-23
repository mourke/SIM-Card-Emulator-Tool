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

	/** The shared manager instance */
	static UpdateManager & sharedManager() {
		static UpdateManager instance;
		return instance;
	}

#if defined(Q_OS_WIN)
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
    void checkVersion(CheckFrequency frequency, std::optional<Callback> callback = std::nullopt);

private:
    void checkForUpdates(std::optional<Callback> callback);

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
	

	QProcess checkForUpdatesProcess;
    std::optional<Callback> m_callback;

private slots:
	void finishedCheckingForUpdates(int exitCode, QProcess::ExitStatus exitStatus);

#elif defined(Q_OS_MAC)
public:
    /**
     * Checks for updates. Checks will be done in the background unless
     * frequency == CheckFrequency::Immediately, in which case, checking UI will be presented
     * to the user.
     *
     * Regardless of the frequency, when an update is found, a popup is presented and shown to the
     * user where they can decide what to do.
     *
     * @param	frequency	The frequency with which to check.
     */
    void checkForUpdates(CheckFrequency frequency);
private:
    class UpdateManagerPrivate;
    UpdateManagerPrivate *p;
#endif

    UpdateManager();
    ~UpdateManager();

    /**
     * @retval	The number of days it has been since the application
     * has been checked for updates.
     */
    long long int daysSinceLastVersionCheckDate() const;

    Q_DISABLE_COPY(UpdateManager)
};

#endif // UPDATE_MANAGER_H

