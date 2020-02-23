#include "MainWindow.h"
#include <QtWidgets>
#include "SegmentedControl.h"
#include "Tracer/TracerManager.h"
#include "Buttons/TitleBarButton.h"
#include "Tracer/Tracer.h"
#include "Tracer/APDUCommand.h"
#include "Errors.h"
#include "Colors.h"
#include "FileManager.h"
#include "UpdateManager.h"
#include "AboutDialog.h"
#include "Segment.h"
#include "ApplicationLayerDelegate.h"
#include <QStandardItemModel>
#include <QVariant>
#include "FontSizes.h"

#if defined(Q_OS_WIN)
#include "singleapplication.h"
#elif defined(Q_OS_MAC)
#include "OpenFileApplication.h"
#endif

MainWindow::MainWindow(QWidget *parent) : FramelessMainWindow(parent) {
	ui.setupUi(this);
    restoreState(); // restore after ui updated

#if defined(Q_OS_WIN)
    if (isMaximized()) {
        ui.maximizeButton->setType(TitleBarButton::Type::Restore);
    }
#elif defined(Q_OS_MAC)
    delete ui.titleBar;

    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    QMenu *editMenu = menuBar()->addMenu(tr("Edit"));
    QMenu *helpMenu = menuBar()->addMenu(tr("Help"));

    QAction *clearAction = editMenu->addAction(tr("Clear Tracing Session"));
    QAction *selectAllAction = editMenu->addAction(tr("Select All"));
    QAction *userManualAction = helpMenu->addAction(tr("User Manual"));
    helpMenu->addSeparator();
    QAction *contactAction = helpMenu->addAction(tr("Contact Support"));
    QAction *openAction = fileMenu->addAction(tr("Open Tracing Session"));
    fileMenu->addSeparator();
    QAction *saveAction = fileMenu->addAction(tr("Save Tracing Session"));
    QAction *aboutAction = fileMenu->addAction(tr("About ScET"));
    QAction *updatesAction = fileMenu->addAction(tr("Check for Updates"));

    aboutAction->setMenuRole(QAction::MenuRole::AboutRole);
    updatesAction->setMenuRole(QAction::MenuRole::ApplicationSpecificRole);
    openAction->setShortcut(QKeySequence::Open);
    saveAction->setShortcut(QKeySequence::Save);
    selectAllAction->setShortcut(QKeySequence::SelectAll);

    connect(clearAction, SIGNAL(triggered()), this, SLOT(clearButtonClicked()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openButtonClicked()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveButtonClicked()));
    connect(selectAllAction, &QAction::triggered, this, [this]() {
        this->textBrowser()->selectAll();
    });
    connect(updatesAction, &QAction::triggered, this, [this]() {
        this->checkForUpdates(UpdateManager::CheckFrequency::Immediately);
    });
    connect(contactAction, SIGNAL(triggered()), this, SLOT(contactSupport()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(userManualAction, SIGNAL(triggered()), this, SLOT(openUserManual()));
#endif

    QFont font;
    QWidget *controls[] = {ui.segmentedControl, ui.SIMToolkitCheckBox, ui.authenticationCheckBox, ui.fileIOCheckBox, ui.startButton, ui.clearButton, ui.stopButton, ui.saveButton, ui.openButton};
    for (QWidget *control : controls) {
        font = control->font();
        font.setPointSize(FONT_SIZE_CONTROL);
        control->setFont(font);
    }

    font = textBrowser()->font();
    font.setPointSize(FONT_SIZE_MEDIUM);
    textBrowser()->setFont(font);

    font = ui.titleLabel->font();
    font.setPointSize(FONT_SIZE_LARGE);
    ui.titleLabel->setFont(font);

    font = ui.subtitleLabel->font();
    font.setPointSize(FONT_SIZE_MEDIUM);
    ui.subtitleLabel->setFont(font);

	ui.segmentedControl->addSegment(tr("Protocol Layer"));
	ui.segmentedControl->addSegment(tr("Application Layer"));

    for (Segment *segment : ui.segmentedControl->segments()) {
        segment->setSelectedIndicatorColor(brandColor());
	}

	QObject::connect(ui.segmentedControl, &SegmentedControl::selectedSegmentIndexChanged, this, [this](segmented_index_t selectedSegmentIndex) {
		this->updateCurrentPageWidget();
    });

    auto palette = listView()->palette();
    palette.setColor(QPalette::ColorRole::Base, palette.alternateBase().color());
    listView()->setPalette(palette);

    setBackgroundRole(QPalette::ColorRole::Base);
    ui.bottomBar->setBackgroundRole(QPalette::ColorRole::Window);
	ui.mainToolBar->setBackgroundRole(QPalette::ColorRole::Base);
    ui.hotpluggingWidget->setBackgroundRole(QPalette::ColorRole::Base); // make background white/black instead off off-gray

	TracerManager &manager = TracerManager::sharedManager();
	auto tracer = manager.findTracer();

	if (tracer.has_value()) {
        tracerConnected(*tracer);
	}
	updateCurrentPageWidget();

	QObject::connect(&manager, SIGNAL(tracerConnected(Tracer *)), this, SLOT(tracerConnected(Tracer *)));
	QObject::connect(&manager, SIGNAL(tracerDisconnected(Tracer *)), this, SLOT(tracerDisconnected(Tracer *)));
	QObject::connect(&manager, SIGNAL(tracerStartedSniffing(Tracer *)), this, SLOT(tracerStartedSniffing(Tracer *)));
	QObject::connect(&manager, SIGNAL(tracerStoppedSniffing(Tracer *, libusb_transfer_status)), this, SLOT(tracerStoppedSniffing(Tracer *, libusb_transfer_status)));
	QObject::connect(&manager, SIGNAL(traceStartedMidSession(Tracer *)), this, SLOT(traceStartedMidSession(Tracer *)));
	QObject::connect(&manager, SIGNAL(apduCommandRecieved(Tracer *, const QString &, const APDUCommand &)), this, SLOT(apduCommandRecieved(Tracer *, const QString &, const APDUCommand &)));
	QObject::connect(&manager, SIGNAL(atrCommandReceived(Tracer *, const QString &)), this, SLOT(atrCommandReceived(Tracer *, const QString &)));
	QObject::connect(&manager, SIGNAL(simTraceCommandReceived(Tracer *, const QString &)), this, SLOT(simTraceCommandReceived(Tracer *, const QString &)));

#if defined(Q_OS_WIN)
    SingleApplication *application = qobject_cast<SingleApplication *>(SingleApplication::instance());
	QObject::connect(application, &SingleApplication::receivedMessage, this, [this](quint32 instanceId, QByteArray message) {
        auto arguments = QString::fromUtf8(message).split("»");
		applicationReceivedArguments(arguments);
	});

	auto arguments = QApplication::arguments();
	applicationReceivedArguments(arguments);
#elif defined(Q_OS_MAC)
    OpenFileApplication *application = qobject_cast<OpenFileApplication *>(OpenFileApplication::instance());
    QObject::connect(application, SIGNAL(openFile(const QString &)), this, SLOT(openFile(const QString &)));
#endif

    QObject::connect(application, &QApplication::applicationStateChanged, [this](Qt::ApplicationState state) {
        if (state == Qt::ApplicationState::ApplicationActive) {
            checkForUpdates(UpdateManager::CheckFrequency::Daily);
        }
    });

	QObject::connect(textBrowser(), &QWidget::customContextMenuRequested, this, [this](const QPoint &position) {
		QMenu *contextMenu = this->textBrowser()->createStandardContextMenu();
		QAction restore(tr("Clear"), this);
		restore.setDisabled(textBrowser()->toPlainText().isEmpty());
		connect(&restore, SIGNAL(triggered()), this, SLOT(clearButtonClicked()));
		contextMenu->addAction(&restore);
		contextMenu->exec(this->textBrowser()->mapToGlobal(position));
		delete contextMenu;
	});

	listView()->setItemDelegate(new ApplicationLayerDelegate(this));
	listViewModel = new QStandardItemModel();
	listView()->setModel(listViewModel);
	QObject::connect(listView()->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(listViewItemClicked(const QItemSelection &)));
	QObject::connect(textBrowser(), SIGNAL(selectionChanged()), this, SLOT(textBrowserTextSelected()));
}

#if defined(Q_OS_WIN)

void MainWindow::applicationReceivedArguments(QStringList arguments) {
	if (arguments.size() > 1) {
		QString fileName = arguments.at(1);
		fileName.replace("'", "");
		openFile(fileName);
	}
}

#endif

void MainWindow::openUserManual() {
    QString tempPath = QDir::fromNativeSeparators(QDir::tempPath());
    QString fileName = "User Manual.pdf";
    QString filePath = tempPath + "/" + fileName;
    QFile::copy(":/" + fileName, filePath);
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void MainWindow::contactSupport() {
    QDesktopServices::openUrl(QUrl("mailto:support@cardcentric.com"));
}

void MainWindow::checkForUpdates(UpdateManager::CheckFrequency frequency) {
#if defined(Q_OS_WIN)
    UpdateManager::sharedManager().checkVersion(frequency, [](std::optional<QString> version) -> UpdateManager::UpdateAction {
		if (!version.has_value()) return UpdateManager::UpdateAction::DoNothing;

		QMessageBox messageBox;
		messageBox.setIcon(QMessageBox::Icon::Information);
		messageBox.setWindowTitle(tr("Updates Found"));
        messageBox.setText(QString(tr("Version %1 is now available to download")).arg(*version));
		auto okButton = messageBox.addButton(QMessageBox::Ok);
		auto skipButton = messageBox.addButton(tr("Skip"), QMessageBox::ButtonRole::NoRole);
		auto updateButton = messageBox.addButton(tr("Update"), QMessageBox::ButtonRole::YesRole);

		messageBox.exec();

		if (messageBox.clickedButton() == okButton) {
			return UpdateManager::UpdateAction::DoNothing;
		} else if (messageBox.clickedButton() == skipButton) {
			return UpdateManager::UpdateAction::Skip;
        } else if (messageBox.clickedButton() == updateButton) {
			return UpdateManager::UpdateAction::Update;
        } else {
            throw std::exception(); // RFU
        }
	});
#elif defined(Q_OS_MAC)
    UpdateManager::sharedManager().checkForUpdates(frequency);
#endif
}

void MainWindow::showSettingsContextMenu() {
	QMenu contextMenu(this);
	contextMenu.setStyleSheet("QMenu::separator { margin: 0px 0px 0px -30px; } QMenu::item { padding: 6px 7px 6px 15px; color: black; } QMenu::item:selected { background-color: rgba(0, 0, 0, 25); }");
	contextMenu.setWindowFlags(contextMenu.windowFlags() | Qt::NoDropShadowWindowHint);

    QAction update(tr("Check for Updates"), &contextMenu);
    QAction support(tr("Contact Support"), &contextMenu);
    QAction about(tr("About ScET..."), &contextMenu);
    QAction userManual(tr("User Manual..."), &contextMenu);

    connect(&userManual, SIGNAL(triggered()), this, SLOT(openUserManual()));
	connect(&update, &QAction::triggered, this, [this]() {
		this->checkForUpdates(UpdateManager::CheckFrequency::Immediately);
	});
    connect(&support, SIGNAL(triggered()), this, SLOT(contactSupport()));
    connect(&about, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

	contextMenu.addAction(&update);
	contextMenu.addAction(&support);
	contextMenu.addSeparator();
	contextMenu.addAction(&about);
    contextMenu.addSeparator();
    contextMenu.addAction(&userManual);

	QPoint position = ui.settingsButton->pos();
	position.setY(position.y() + ui.settingsButton->geometry().height()); // account for height
	position.setX(position.x() + 1); // account for border
	contextMenu.exec(mapToGlobal(position));
}

void MainWindow::showAboutDialog() {
    AboutDialog().exec();
}

void MainWindow::startButtonClicked() {
	assert(tracer.has_value()); // logic error somewhere if this fails.

    libusb_error error = TracerManager::sharedManager().startSniffing(*tracer);

	if (error != LIBUSB_SUCCESS) {
		QMessageBox messageBox;
		messageBox.critical(this, tr("Error Starting Trace Session"), libUSBErrorToString(error));
	}
}

void MainWindow::stopButtonClicked() {
    if (tracer.has_value() && (*tracer)->isSniffing()) {
        TracerManager::sharedManager().stopSniffing(*tracer);
	}
}

void MainWindow::saveButtonClicked() {
	const QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save SIM Tracing Session"), "",
        "HTML Document (*.html);;SIM Tracing Session (*.sts);;ETSI Test Script (*.ldr)");

	if (!fileName.isEmpty()) {
		const QString extension = QFileInfo(fileName).suffix();
		QString contents;

		if (extension == "html") {
			contents = textBrowser()->toHtml();
		} else if (extension == "sts") {
			contents = simTraceCommands;
		} else if (extension == "ldr") {
			contents = commandsToLDR();
		} else {
			assert(false); // RFU
		}
		
		try {
			FileManager::sharedManager().saveFile(fileName, contents);
		} catch (const std::exception &exception) {
			QMessageBox::information(this, tr("Unable to save file"), exception.what());
		}
	}
}

void MainWindow::openButtonClicked() {
	const QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open SIM Tracing Session"), "",
        "SIM Tracing Session (*.sts)");

	if (!fileName.isEmpty()) {
		openFile(fileName);
	}
}

void MainWindow::openFile(const QString &fileName) {
	if (!clearButtonClicked()) return; // user cancelled request
	try {
		stopButtonClicked();
		auto data = FileManager::sharedManager().openFile(fileName);
        auto bufferLength = static_cast<int>(data.size());

		if (bufferLength == 0) return; // file empty, do nothing

		auto buffer = &data[0]; // convert to c buffer

		if (!tracer.has_value()) {
			Tracer *tracer = new Tracer(); // simulate a tracer to process the input
			TracerManager::sharedManager().manageTracer(tracer);
			tracer->processInput(buffer, bufferLength);
			TracerManager::sharedManager().stopManagingTracer(tracer);
		} else {
            (*tracer)->processInput(buffer, bufferLength);
		}

		textBrowserInsertAtStart();
		updateCurrentPageWidget();
	}
	catch (const std::exception &exception) {
		QMessageBox::information(this, tr("Unable to open file"), exception.what());
	}
}

QString MainWindow::commandsToLDR() {
	QString ldr;

	auto appendString = [&ldr](QString string) {
		static const int maxCharacterPerLine = 16;
		int charactersPerLine = 0;
		for (auto substring : string.split(" ", QString::SplitBehavior::SkipEmptyParts)) {
			if (charactersPerLine == maxCharacterPerLine ||
				substring.contains("(") ||
				substring.contains("[")) {
				ldr += " \\\n\t";
				charactersPerLine = 0;
			} else {
				ldr += " ";
			}
			ldr += substring;
			charactersPerLine++;
		}
	};

	for (int i = 0; i < commands.size(); ++i) {
		auto tuple = commands.at(i);
		auto command = std::get<1>(tuple);
		if (command.has_value()) {
            auto c = *command;
			if (c->instructionCode() == APDUCommand::InstructionCode::TerminalProfile) {
				ldr += "INI\t";
				QString string;
				for (uint8_t character : c->data()) {
					string += QString::asprintf(" %02X", character);
				}
				appendString(string);
			} else {
				ldr += "CMD\t";
				if (i + 1 < commands.size()) {
					auto nextCommand = std::get<1>(commands.at(i + 1));
					if (nextCommand.has_value()) {
                        auto nc = *nextCommand;
						if (nc->instructionCode() == APDUCommand::InstructionCode::GetResponse) {
							QString string = "[";
                            for (unsigned long int i = 0; i < nc->data().size(); ++i) {
								uint8_t character = nc->data()[i];
								string += QString::asprintf("%02X", character);
								if (i + 1 == nc->data().size()) {
									string += "] (";
								} else {
									string += " ";
								}
							}
							
							auto parts = c->protocolString().split("(");
							parts.insert(1, string);
							appendString(parts.join(""));
							++i; // skip next one
							goto trailer;
						}
					}
				}
				appendString(c->protocolString());
			}
		} else { // ATR
			ldr += "RST";
		}
	trailer:
		ldr += "\n\n";
	}

	return ldr;
}

bool MainWindow::clearButtonClicked() {
	if (textBrowser()->toPlainText().isEmpty()) return true;
	QMessageBox messageBox;
	QMessageBox::StandardButton reply = messageBox.question(this, tr("Clear Trace Session"), tr("Are you sure you want to discard the current Trace session?"),
		QMessageBox::Discard | QMessageBox::Cancel);

	if (reply == QMessageBox::Discard) {
		stopButtonClicked();
		clearCurrentTrace();
		return true;
	}

	return false;
}

void MainWindow::checkboxStateChanged(int rawValue) {
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

	for (int i = 0; i < commands.size(); ++i) {
		auto tuple = commands.at(i);
		QString output = std::get<0>(tuple);
		auto command = std::get<1>(tuple);
		if (command.has_value()) {
            if ((*command)->type() & filter) {
                updateTextBrowser(output,  *command);
				listView()->setRowHidden(i, false);
			} else {
				listView()->setRowHidden(i, true);
			}
		} else { // ATR
            textBrowser()->setTextColor(palette().text().color());
			textBrowser()->insertPlainText(output + "\n");
		}
	}

	textBrowserInsertAtStart(); // Scroll to top
}

void MainWindow::clearCurrentTrace() {
	textBrowser()->clear();
	listViewModel->clear();

	for (auto tuple : commands) {
		auto command = std::get<1>(tuple);
		if (command.has_value()) {
            delete *command;
		}
	}

	commands.clear();
	simTraceCommands.clear();
	updateCurrentPageWidget();
}

void MainWindow::tracerConnected(Tracer *tracer) {
	this->tracer = std::make_optional(tracer);
	ui.startButton->setEnabled(true);
	updateCurrentPageWidget();
}

void MainWindow::tracerDisconnected(Tracer *tracer) {
	this->tracer.reset();
	ui.startButton->setDisabled(true);
	updateCurrentPageWidget();
}

void MainWindow::tracerStartedSniffing(Tracer *tracer) {
	ui.startButton->setDisabled(true);
	ui.stopButton->setEnabled(true);
}

void MainWindow::tracerStoppedSniffing(Tracer *tracer, libusb_transfer_status errorCode) {
	ui.startButton->setEnabled(tracer->isConnected());
	ui.stopButton->setDisabled(true);
	if (errorCode == LIBUSB_TRANSFER_STALL || errorCode == LIBUSB_TRANSFER_NO_DEVICE) {
		QMessageBox messageBox;
		messageBox.critical(this, tr("Sniffer Disconnected"), tr("The Sniffer was disconnected during a trace session. It is advised that the trace be stopped before unplugging the SIM card Sniffer."));
	}
}

void MainWindow::traceStartedMidSession(Tracer *tracer) {
	QMessageBox messageBox;
	messageBox.warning(this, tr("Trace Started Mid Session"), tr("The SIM card about to be sniffed has already been making requests for which ScET has not been listening. You may find yourself at an arbitrary point in the trace which may result in unexpected values."));
}

void MainWindow::apduCommandRecieved(Tracer *tracer, const QString &output, const APDUCommand &c) {
	const APDUCommand *command = new APDUCommand(c);
	commands.append(std::make_tuple(output, std::make_optional(command)));

	if (command->applicationMap().has_value()) {
		QStandardItem *item = new QStandardItem();
		item->setText(command->instructionCodeString());
        item->setData(*command->applicationMap());
		item->setEditable(false);
		listViewModel->appendRow(item);
		if (!(command->type() & filter)) {
			listView()->setRowHidden(listViewModel->rowCount(), true);
		}
	}

	if (command->type() & filter) { // only display if current filter allows
		updateTextBrowser(output, command);
	}
}

void MainWindow::atrCommandReceived(Tracer *tracer, const QString &output) {
	commands.append(std::make_tuple(output, std::nullopt));
	textBrowserInsertAtEnd();
    textBrowser()->setTextColor(palette().text().color());
	textBrowser()->insertPlainText(output + "\n");
}

void MainWindow::simTraceCommandReceived(Tracer *tracer, const QString &input) {
	simTraceCommands.append(input + "\n");
}

void MainWindow::updateTextBrowser(const QString &output, const APDUCommand *command) {
	textBrowserInsertAtEnd(); // append data to end. this is done in case the user highlights anything while this method is being called. when anything is highlighed, it changes the cursor's position to there and that's where the data will be inserted. 
    int offset, length; // the offset and length of the section of the APDU command in the output string
	offset = 26; // apdu and timestamp
    textBrowser()->setTextColor(palette().text().color());
	textBrowser()->insertPlainText(output.left(offset));
	textBrowser()->setTextColor(headerColor());
	length = 15; // header (CLA, INS, P1, P2, Lc. Each item 2 digits long. 5 * 2 = 10) and the space before and after each (5).
	textBrowser()->insertPlainText(output.mid(offset, length));
	offset += length; // header and space after
	textBrowser()->setTextColor(command->instructionCode() == APDUCommand::InstructionCode::GetResponse ? responseColor() : dataColor());
    length = static_cast<int>(command->data().size()) * 3; // data.size * 2 (two digits for every element) and the space before and after each (data.size)
	textBrowser()->insertPlainText(output.mid(offset, length));
	offset += length;
	textBrowser()->setTextColor(statusCodeColor());
	length = 7; // status bytes (SW1 and SW2. Each one two digits long. 2 * 2 = 4), the space in between (1) and the two brackets either side (2)
	textBrowser()->insertPlainText(output.mid(offset, length));
	textBrowser()->insertPlainText("\n");
}

void MainWindow::textBrowserTextSelected() {
	if (ui.stackedWidget->currentWidget() != ui.protocolPage) return;
	QTextCursor cursor = textBrowser()->textCursor();
	cursor.select(QTextCursor::BlockUnderCursor); // select the whole command even if it spans multiple lines
	QString line = cursor.selectedText().trimmed();
	if (line.isEmpty()) return;
	int index = 0;
	for (auto tuple : commands) {
		QString output = std::get<0>(tuple);
		auto command = std::get<1>(tuple);
        if (!(command.has_value() && (*command)->applicationMap().has_value())) { continue; }
		if (line == output) {
			listView()->setCurrentIndex(listViewModel->index(index, 0));
			break;
		}
		index++;
	}
}

void MainWindow::listViewItemClicked(const QItemSelection &item) {
	if (ui.stackedWidget->currentWidget() != ui.applicationPage) return; // stop infinite loop when programatic selected of list view item occurs
	auto indexes = item.indexes();
	if (indexes.isEmpty()) return;
	QModelIndex currentIndex = indexes.first();
	int index = 0;
	QTextCursor cursor(textBrowser()->document());
	for (auto tuple : commands) {
		auto command = std::get<1>(tuple);
		if (command.has_value() && 
            ((*command)->applicationMap().has_value())) {
            if (index == currentIndex.row()) {
				break;
			}
			index++;
		}
		cursor.movePosition(QTextCursor::NextBlock);
	}
	
	cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);	
	textBrowser()->setTextCursor(cursor);
}

void MainWindow::textBrowserInsertAtEnd() {
	auto textCursor = textBrowser()->textCursor();
	textCursor.movePosition(QTextCursor::End);
	textBrowser()->setTextCursor(textCursor);
}

void MainWindow::textBrowserInsertAtStart() {
	auto textCursor = textBrowser()->textCursor();
	textCursor.movePosition(QTextCursor::Start);
	textBrowser()->setTextCursor(textCursor);
}

void MainWindow::showMaxRestore() {
	isMaximized() ? showNormal() : showMaximized();
}

#if defined(Q_OS_WIN)

void MainWindow::changeEvent(QEvent *event) {
	if (event->type() == QEvent::WindowStateChange) {
		QWindowStateChangeEvent *stateChangeEvent = static_cast<QWindowStateChangeEvent *>(event);

		if (stateChangeEvent->oldState() == Qt::WindowNoState && this->windowState() == Qt::WindowMaximized) {
			ui.maximizeButton->setType(TitleBarButton::Type::Restore);
		} else if (stateChangeEvent->oldState() == Qt::WindowMaximized && this->windowState() == Qt::WindowNoState) {
			ui.maximizeButton->setType(TitleBarButton::Type::Maximize);
		}
	}
	QWidget::changeEvent(event);
}



bool MainWindow::shouldMoveWindow() {
	QWidget *action = QApplication::widgetAt(QCursor::pos());
	return action == ui.titleBar;
}

#endif

void MainWindow::updateCurrentPageWidget() {
	if (tracer.has_value() || !textBrowser()->toPlainText().isEmpty()) {
		if (ui.segmentedControl->selectedSegmentIndex() == 0) {
			ui.stackedWidget->setCurrentWidget(ui.protocolPage);
		} else {
			ui.stackedWidget->setCurrentWidget(ui.applicationPage);
			auto currentIndex = listView()->currentIndex();
			listView()->scrollTo(currentIndex, QAbstractItemView::PositionAtTop);
		}
	} else {
		ui.stackedWidget->setCurrentWidget(ui.noDevicePage);
	}
}
