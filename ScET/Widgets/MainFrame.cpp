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

	// TODO: remove this once libUSB implements hotplugging for windows
	auto tracer = manager.findTracer();

	if (tracer.has_value()) {
		tracerConnected(tracer.value()); 
	}
	///////////////////////////////////////////////////////////////////
	
	QObject::connect(&manager, SIGNAL(tracerConnected(Tracer *)), this, SLOT(tracerConnected(Tracer *)));
	QObject::connect(&manager, SIGNAL(tracerDisconnected(Tracer *)), this, SLOT(tracerDisconnected(Tracer *)));
	QObject::connect(&manager, SIGNAL(tracerStartedSniffing(Tracer *)), this, SLOT(tracerStartedSniffing(Tracer *)));
	QObject::connect(&manager, SIGNAL(tracerStoppedSniffing(Tracer *, libusb_transfer_status)), this, SLOT(tracerStoppedSniffing(Tracer *, libusb_transfer_status)));
	QObject::connect(&manager, SIGNAL(traceStartedMidSession(Tracer *)), this, SLOT(traceStartedMidSession(Tracer *)));
	QObject::connect(&manager, SIGNAL(apduCommandRecieved(Tracer *, const QString &, const APDUCommand &)), this, SLOT(apduCommandRecieved(Tracer *, const QString &, const APDUCommand &)));
	QObject::connect(&manager, SIGNAL(atrCommandReceived(Tracer *, const QString &)), this, SLOT(atrCommandReceived(Tracer *, const QString &)));
	QObject::connect(&manager, SIGNAL(simTraceCommandReceived(Tracer *, const QString &)), this, SLOT(simTraceCommandReceived(Tracer *, const QString &)));
}

void MainFrame::startButtonClicked() {
	assert(tracer.has_value()); // logic error somewhere if this fails.

	libusb_error error = TracerManager::sharedManager().startSniffing(tracer.value());

	if (error != LIBUSB_SUCCESS) {
		QMessageBox messageBox;
		messageBox.critical(this, tr("Error Starting Trace Session"), libUSBErrorToString(error));
	}
}

void MainFrame::stopButtonClicked() {
	if (tracer.has_value() && tracer.value()->isSniffing()) {
		TracerManager::sharedManager().stopSniffing(tracer.value());
	}
}

void MainFrame::saveButtonClicked() {
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save SIM Tracing Session"), "",
		tr("HTML Document (*.html);;SIM Tracing Session (*.sts)"));

	if (!fileName.isEmpty()) {
		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly)) {
			QTextStream out(&file);
			QString extension = QFileInfo(fileName).suffix();
			if (extension == "html") {
				out << textBrowser()->toHtml();
			} else {
				out << simTraceCommands;
			}
			file.close();
		} else {
			QMessageBox::information(this, tr("Unable to save file"), file.errorString());
		}
	}
}

void MainFrame::openButtonClicked() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open SIM Tracing Session"), "",
		tr("SIM Tracing Session (*.sts)"));

	if (!fileName.isEmpty()) {
		QFile file(fileName);

		if (file.open(QIODevice::ReadOnly)) {
			if (!clearButtonClicked()) return;

			QTextStream in(&file);
			Tracer fakeTracer;

			QObject::connect(&fakeTracer, &Tracer::apduCommandReceived, this, [this, &fakeTracer](const QString &output, const APDUCommand &command) {
				apduCommandRecieved(&fakeTracer, output, command);
			});

			QObject::connect(&fakeTracer, &Tracer::atrCommandReceived, this, [this, &fakeTracer](const QString &output) {
				atrCommandReceived(&fakeTracer, output);
			});

			QObject::connect(&fakeTracer, &Tracer::simTraceCommandReceived, this, [this, &fakeTracer](const QString &input) {
				simTraceCommandReceived(&fakeTracer, input);
			});

			fakeTracer.createSplitter();

			while (!in.atEnd()) {
				QString input = in.readLine();
				int inputLength = input.length();
				if (inputLength == 0) continue;
				size_t bufferLength = inputLength / 2;
				uint8_t *buffer = (uint8_t *)malloc(bufferLength);
				for (int i = 0; i + 1 < inputLength; i += 2) {
					uint8_t hex = 0;
					QChar first = input[i].toLower();
					QChar second = input[i + 1].toLower();
					if (first.isDigit()) {
						hex = first.digitValue() * 16;
					} else {
						hex += (first.toLatin1() - 'a' + 10) * 16;
					}

					if (second.isDigit()) {
						hex += second.digitValue();
					} else {
						hex += second.toLatin1() - 'a' + 10;
					}
					*(buffer + i/2) = hex;
				}
				fakeTracer.processInput(buffer, bufferLength);
				free(buffer);
			}
			
			fakeTracer.deleteSplitter();
			QObject::disconnect(&fakeTracer);
			file.close();
		} else {
			QMessageBox::information(this, tr("Unable to open file"), file.errorString());
		}
	}
}

bool MainFrame::clearButtonClicked() {
	if (textBrowser()->toPlainText().isEmpty()) return true;
	QMessageBox messageBox;
	QMessageBox::StandardButton reply = messageBox.question(this, tr("Clear Trace Session"), tr("Are you sure you want to discard the current Trace session?"),
		QMessageBox::Discard | QMessageBox::Cancel);

	if (reply == QMessageBox::Discard) {
		stopButtonClicked();
		reset();
		return true;
	}

	return false;
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
			textBrowser()->setTextColor(Qt::black);
			textBrowser()->insertPlainText(output + "\n");
		}
	}
}

void MainFrame::reset() {
	textBrowser()->clear();
	commands.clear();
	simTraceCommands.clear();
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
}

void MainFrame::apduCommandRecieved(Tracer *tracer, const QString &output, const APDUCommand &command) {
	commands.append(std::make_tuple(output, command));
	if (command.fileType() & filter) updateTextBrowser(output, command); // only display if current filter allows
}

void MainFrame::atrCommandReceived(Tracer *tracer, const QString &output) {
	commands.append(std::make_tuple(output, std::nullopt));
	moveCursorToEnd();
	textBrowser()->setTextColor(Qt::black);
	textBrowser()->insertPlainText(output + "\n");
}

void MainFrame::simTraceCommandReceived(Tracer *tracer, const QString &input) {
	simTraceCommands.append(input + "\n");
}

void MainFrame::updateTextBrowser(const QString &output, const APDUCommand &command) {
	moveCursorToEnd();
	int offset = 26; // apdu and timestamp
	textBrowser()->setTextColor(Qt::black);
	textBrowser()->insertPlainText(output.left(offset));
	textBrowser()->setTextColor(headerColor());
	textBrowser()->insertPlainText(output.mid(offset, 15));
	offset += 15; // header and space after
	textBrowser()->setTextColor(command.type() == APDUCommand::Type::Command ? dataColor() : responseColor());
	textBrowser()->insertPlainText(output.mid(offset, command.data().size() * 3));
	offset += command.data().size() * 3; // data (two hexadecimal characters per data) and spacings between and after each character
	textBrowser()->setTextColor(statusCodeColor());
	textBrowser()->insertPlainText(output.mid(offset, 7));
	textBrowser()->insertPlainText("\n");
}

void MainFrame::moveCursorToEnd() {
	auto textCursor = textBrowser()->textCursor();
	textCursor.movePosition(QTextCursor::End);
	textBrowser()->setTextCursor(textCursor);
}