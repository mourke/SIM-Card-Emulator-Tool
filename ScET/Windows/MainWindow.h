#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "FramelessMainWindow.h"
#include "ui_MainWindow.h"
#include <optional>
#include "UpdateManager.h"
#include <libusb.h>

class QMouseEvent;
class Tracer;
class APDUCommand;
class AboutDialog;
class QStandardItemModel;
class QItemSelection;

class MainWindow : public FramelessMainWindow {

	Q_OBJECT
		
		Q_PROPERTY(QTextBrowser textBrowser READ textBrowser)
		Q_PROPERTY(QListView listView READ listView)
		Q_PROPERTY(QWidget titleBar READ titleBar)
		Q_PROPERTY(QWidget mainToolBar READ mainToolBar)

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	QTextBrowser * textBrowser() const { return ui.textBrowser; }
	QListView * listView() const { return ui.listView; }
	QWidget * mainToolBar() const { return ui.mainToolBar; }
	QWidget * titleBar() const { return ui.titleBar; }

	enum APDUFilter : uint8_t {
		STK = 1 << 0,
		FileIO = 1 << 1,
		Authentication = 1 << 2,
		Unknown = 1 << 3,
		All = STK | FileIO | Authentication | Unknown
	};

	Q_ENUM(APDUFilter)

private slots:
	// MARK: - UI signals
	void startButtonClicked();
	void stopButtonClicked();
	void openButtonClicked();
	void saveButtonClicked();
	bool clearButtonClicked(); // returns true if user has confirmed clear
	void showSettingsContextMenu();
	void checkboxStateChanged(int rawValue);
	void showMaxRestore();
	void listViewItemClicked(const QItemSelection &item);
	void textBrowserTextSelected();
    void openUserManual();
    void contactSupport();
    void openFile(const QString &fileName);
    void showAboutDialog();

	// MARK: - Tracer signals
	void tracerStartedSniffing(Tracer *tracer);
	void tracerStoppedSniffing(Tracer *tracer, libusb_transfer_status errorCode);
	void traceStartedMidSession(Tracer *tracer);
	void apduCommandRecieved(Tracer *tracer, const QString &output, const APDUCommand &command);
	void atrCommandReceived(Tracer *tracer, const QString &output);
	void simTraceCommandReceived(Tracer *tracer, const QString &input);
	void tracerConnected(Tracer *tracer);
	void tracerDisconnected(Tracer *tracer);
	
private:
	Ui::MainWindow ui;
    APDUFilter filter = All;
	std::optional<Tracer *> tracer;
	QVector<std::tuple<QString, std::optional<const APDUCommand *>>> commands;
	QString simTraceCommands;
	QStandardItemModel *listViewModel;

	void clearCurrentTrace();
	void updateTextBrowser(const QString &output, const APDUCommand *command);
	void textBrowserInsertAtStart();
    void textBrowserInsertAtEnd();
	void applicationReceivedArguments(QStringList arguments);
	void checkForUpdates(UpdateManager::CheckFrequency frequency);

#if defined(Q_OS_WIN)
	void changeEvent(QEvent *event) override;
	bool shouldMoveWindow() override;
#endif
	void updateCurrentPageWidget();
	QString commandsToLDR();
};

#endif // MAIN_WINDOW_H
