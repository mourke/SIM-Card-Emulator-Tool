#include "MainFrame.h"
#include <QtWidgets>
#include "TitleBar.h"
#include "SegmentedControl.h"
#include "Tracer/TracerManager.h"
#include <libusb.h>
#include "Tracer/Tracer.h"
#include "Tracer/APDUCommand.h"
#include "Errors.h"
#include "Colors.h"


MainFrame::MainFrame(QWidget *parent) : BorderlessWindowFrame(parent) {
	ui.setupUi(this);

	restoreState();  // call again here after UI has been loaded in

	if (isMaximized()) {
		ui.maximizeButton->setType(TitleBarButton::Type::Restore);
	}
	
	ui.segmentedControl->addSegment(tr("Protocol Layer"));
	ui.segmentedControl->addSegment(tr("Application Layer"));

	TracerManager &manager = TracerManager::sharedManager();
	auto tracer = manager.findTracer();

	if (tracer.has_value()) {
		tracerConnected(tracer.value()); // TODO: remove this once libUSB implements hotplugging for windows
	}
	
	QObject::connect(&manager, SIGNAL(tracerConnected(Tracer *)), this, SLOT(tracerConnected(Tracer *)));
	QObject::connect(&manager, SIGNAL(tracerDisconnected(Tracer *)), this, SLOT(tracerDisconnected(Tracer *)));
	QObject::connect(&manager, SIGNAL(tracerStartedSniffing(Tracer *)), this, SLOT(tracerStartedSniffing(Tracer *)));
	QObject::connect(&manager, SIGNAL(tracerStoppedSniffing(Tracer *, libusb_transfer_status)), this, SLOT(tracerStoppedSniffing(Tracer *, libusb_transfer_status)));
	QObject::connect(&manager, SIGNAL(traceStartedMidSession(Tracer *)), this, SLOT(traceStartedMidSession(Tracer *)));
	QObject::connect(&manager, SIGNAL(apduCommandRecieved(Tracer *, const QString &, const APDUCommand &)), this, SLOT(apduCommandRecieved(Tracer *, const QString &, const APDUCommand &)));
	QObject::connect(&manager, SIGNAL(atrCommandReceived(Tracer *, const QString &)), this, SLOT(atrCommandReceived(Tracer *, const QString &)));
}

void MainFrame::startButtonPressed() {
	assert(tracer.has_value()); // logic error somewhere if this fails.

	libusb_error error = TracerManager::sharedManager().startSniffing(tracer.value());

	if (error != LIBUSB_SUCCESS) {
		QMessageBox messageBox;
		messageBox.critical(this, tr("Error Starting Trace Session"), libUSBErrorToString(error));
		messageBox.setFixedSize(500, 200);
	}
}

void MainFrame::stopButtonPressed() {
	if (tracer.has_value() && tracer.value()->isSniffing()) {
		TracerManager::sharedManager().stopSniffing(tracer.value());
	}
}

void MainFrame::exportButtonPressed() {

}

void MainFrame::importButtonPressed() {

}

void MainFrame::saveButtonPressed() {

}

void MainFrame::checkboxStateChanged(int rawValue) {
	filter = All;

	if (!ui.authenticationCheckBox->isChecked()) {
		filter = APDUFilter(filter ^ Authentication);
	}

	if (!ui.fileIOCheckBox->isChecked()) {
		filter = APDUFilter(filter ^ FileIO);
	}

	if (!ui.SIMToolkitCheckBox->isChecked()) {
		filter = APDUFilter(filter ^ STK);
	}

	textBrowser()->clear();

	for (auto tuple : commands) {
		QString output = std::get<0>(tuple);
		std::optional<APDUCommand> command = std::get<1>(tuple);
		if (command.has_value()) {
			if (command.value().fileType() & filter) updateTextBrowser(output, command.value());
		} else { // ATR
			textBrowser()->insertPlainText(output + "\n");
		}
	}
}

void MainFrame::tracerConnected(Tracer *tracer) {
	this->tracer = std::make_optional(tracer);
	ui.startButton->setEnabled(true);
}

void MainFrame::tracerDisconnected(Tracer *tracer) {
	this->tracer.reset();
	ui.startButton->setDisabled(true);
	if (tracer->isSniffing()) {
		QMessageBox messageBox;
		messageBox.critical(this, tr("Sniffer Disconnected"), tr("The Sniffer was disconnected during a trace session. It is advised that the trace be stopped before unplugging the SIM card Sniffer."));
		messageBox.setFixedSize(500, 200);
	}
}

void MainFrame::tracerStartedSniffing(Tracer *tracer) {
	ui.startButton->setDisabled(true);
	ui.stopButton->setEnabled(true);
}

void MainFrame::tracerStoppedSniffing(Tracer *tracer, libusb_transfer_status errorCode) {
	ui.startButton->setEnabled(tracer->isConnected());
	ui.stopButton->setDisabled(true);
}

void MainFrame::traceStartedMidSession(Tracer *tracer) {
	QMessageBox messageBox;
	messageBox.warning(this, tr("Trace Started Mid Session"), tr("The SIM card about to be sniffed has already been making requests for which ScET has not been listening. You may find yourself at an arbitrary point in the trace which may result in unexpected values."));
	messageBox.setFixedSize(500, 200);
}

void MainFrame::apduCommandRecieved(Tracer *tracer, const QString &output, const APDUCommand &command) {
	commands.append(std::make_tuple(output, command));
	if (command.fileType() & filter) updateTextBrowser(output, command); // only display if current filter allows
}

void MainFrame::atrCommandReceived(Tracer *tracer, const QString &output) {
	commands.append(std::make_tuple(output, std::nullopt));
	textBrowser()->insertPlainText(output + "\n");
}

void MainFrame::updateTextBrowser(const QString &output, const APDUCommand &command) {
	textBrowser()->setTextColor(Qt::black);
	int offset = 26; // apdu and timestamp
	textBrowser()->insertPlainText(output.left(offset));
	textBrowser()->setTextColor(headerColor());
	textBrowser()->insertPlainText(output.mid(offset, 15));
	offset += 15; // header
	textBrowser()->setTextColor(command.type() == APDUCommand::Type::Command ? dataColor() : responseColor());
	textBrowser()->insertPlainText(output.mid(offset, command.dataLength() * 3));
	offset += command.dataLength() * 3; // data (two hexadecimal characters per data) and spacings between and after each character
	textBrowser()->setTextColor(statusCodeColor());
	textBrowser()->insertPlainText(output.mid(offset, 7));
	textBrowser()->insertPlainText("\n");
}