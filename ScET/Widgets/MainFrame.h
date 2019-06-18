#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "BorderlessWindowFrame.h"
#include "ui_MainFrame.h"
#include <optional>

class TitleBar;
class ToolBar;
class QMouseEvent;
class Tracer;
class APDUCommand;
enum libusb_transfer_status;

class MainFrame : public BorderlessWindowFrame {
	Q_OBJECT

	Q_PROPERTY(QTextBrowser textBrowser READ textBrowser)
	Q_PROPERTY(TitleBar titleBar READ titleBar)
	Q_PROPERTY(ToolBar mainToolBar READ mainToolBar)

public:
	MainFrame(QWidget *parent = Q_NULLPTR);

	QTextBrowser * textBrowser() const { return ui.textBrowser; }
	ToolBar * mainToolBar() const { return ui.mainToolBar; }
	TitleBar * titleBar() const { return ui.titleBar;  }

public slots:
	void startTracing();
	void stopTracing();

private slots:
	void tracerStartedSniffing(Tracer *tracer);
	void tracerStoppedSniffing(Tracer *tracer, libusb_transfer_status errorCode);
	void traceStartedMidSession(Tracer *tracer);
	void apduCommandRecieved(Tracer *tracer, const QString &output, const APDUCommand &command);
	void atrCommandReceived(Tracer *tracer, const QString &output);

private:
	Ui::MainFrame ui;
	std::optional<Tracer *> tracer;
	friend class TitleBar;
};

#endif // MAINFRAME_H