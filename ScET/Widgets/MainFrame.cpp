#include "MainFrame.h"
#include <QtWidgets>
#include "TitleBar.h"
#include "SegmentedControl.h"
#include "Tracer/TracerManager.h"
#include <libusb.h>
#include "Tracer/Tracer.h"
#include "Tracer/APDUCommand.h"
#include "Errors.h"


MainFrame::MainFrame(QWidget *parent) : BorderlessWindowFrame(parent) {
	ui.setupUi(this);

	restoreState();  // call again here after UI has been loaded in

	if (isMaximized()) {
		ui.maximizeButton->setType(TitleBarButton::Type::Restore);
	}
	
	ui.segmentedControl->addSegment(tr("Protocol Layer"));
	ui.segmentedControl->addSegment(tr("Application Layer"));

	TracerManager &manager = TracerManager::sharedManager();
	QObject::connect(&manager, SIGNAL(tracerStartedSniffing(Tracer *)), this, SLOT(tracerStartedSniffing(Tracer *)));
	QObject::connect(&manager, SIGNAL(tracerStoppedSniffing(Tracer *, libusb_transfer_status)), this, SLOT(tracerStoppedSniffing(Tracer *, libusb_transfer_status)));
	QObject::connect(&manager, SIGNAL(traceStartedMidSession(Tracer *)), this, SLOT(traceStartedMidSession(Tracer *)));
	QObject::connect(&manager, SIGNAL(apduCommandRecieved(Tracer *, const QString &, const APDUCommand &)), this, SLOT(apduCommandRecieved(Tracer *, const QString &, const APDUCommand &)));
	QObject::connect(&manager, SIGNAL(atrCommandReceived(Tracer *, const QString &)), this, SLOT(atrCommandReceived(Tracer *, const QString &)));
}

void MainFrame::startTracing() {
	// TODO: Implement hotplugging 
	std::optional<Tracer *> tracer = TracerManager::sharedManager().findTracer();

	if (tracer.has_value()) {
		libusb_error error = TracerManager::sharedManager().startSniffing(tracer.value());

		if (error != LIBUSB_SUCCESS) {
			QMessageBox messageBox;
			messageBox.critical(this, tr("Error Starting Trace Session"), libUSBErrorToString(error));
			messageBox.setFixedSize(500, 200);
		}
	} else {
		// No tracer is connected to the computer
	}
}

void MainFrame::stopTracing() {
	if (tracer.has_value() && tracer.value()->isSniffing()) {
		TracerManager::sharedManager().stopSniffing(tracer.value());
	}
}

void MainFrame::tracerStartedSniffing(Tracer *tracer) {
	ui.startButton->setDisabled(true);
	ui.stopButton->setDisabled(false);
	this->tracer = std::make_optional(tracer);
}

void MainFrame::tracerStoppedSniffing(Tracer *tracer, libusb_transfer_status errorCode) {
	ui.startButton->setDisabled(false);
	ui.stopButton->setDisabled(true);
	this->tracer.reset();
}

void MainFrame::traceStartedMidSession(Tracer *tracer) {
	QMessageBox messageBox;
	messageBox.warning(this, tr("Trace Started Mid Session"), tr("The SIM card about to be sniffed has already been making requests for which ScET has not been listening. You may find yourself at an arbitrary point in the trace which may result in incorrect values."));
	messageBox.setFixedSize(500, 200);
}

void MainFrame::apduCommandRecieved(Tracer *tracer, const QString &output, const APDUCommand &command) {
	textBrowser()->insertPlainText(output + "\n");
}

void MainFrame::atrCommandReceived(Tracer *tracer, const QString &output) {
	textBrowser()->insertPlainText(output + "\n");
}