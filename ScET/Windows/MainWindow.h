#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "FramelessMainWindow.h"
#include "ui_MainWindow.h"
#include <optional>
#include "UpdateManager.h"

class TitleBar;
class ToolBar;
class QMouseEvent;
class Tracer;
class APDUCommand;
class AboutDialog;
enum libusb_transfer_status;

class MainWindow : public FramelessMainWindow {
	Q_OBJECT

		Q_PROPERTY(QTextBrowser textBrowser READ textBrowser)
		Q_PROPERTY(TitleBar titleBar READ titleBar)
		Q_PROPERTY(ToolBar mainToolBar READ mainToolBar)

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

	QTextBrowser * textBrowser() const { return ui.textBrowser; }
	ToolBar * mainToolBar() const { return ui.mainToolBar; }
	TitleBar * titleBar() const { return ui.titleBar; }

	enum APDUFilter : uint8_t {
		STK = 1 << 0,
		FileIO = 1 << 1,
		Authentication = 1 << 2,
		All = STK | FileIO | Authentication
	};

	Q_ENUM(APDUFilter)

private slots:
	void startButtonClicked();
	void stopButtonClicked();
	void openButtonClicked();
	void saveButtonClicked();
	bool clearButtonClicked(); // returns true if user has confirmed clear
	void showSettingsContextMenu();
	void checkboxStateChanged(int rawValue);
	void showMaxRestore();

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
	AboutDialog *about;
	std::optional<Tracer *> tracer;
	QVector<std::tuple<QString, std::optional<APDUCommand>>> commands;
	QString simTraceCommands;

	void reset();
	void updateTextBrowser(const QString &output, const APDUCommand &command);
	void moveCursorToStart();
	void moveCursorToEnd();
	void openFile(const QString &fileName);
	void applicationReceivedArguments(QStringList arguments);
	void checkForUpdates(UpdateManager::CheckFrequency frequency);
	void changeEvent(QEvent *event) override;

	bool shouldMoveWindow() override;
	void updateCurrentPageWidget();
};

#endif // MAIN_WINDOW_H