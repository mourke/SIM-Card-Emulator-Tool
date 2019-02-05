#include "MainWindow.h"
#include "Widgets/MainFrame.h"
#include "Widgets/TitleBar.h"
#include "Widgets/ToolBar.h"
#include <QtWidgets>

MainWindow::MainWindow(QObject *parent) : QObject(parent) {
	m_frame = new MainFrame();
}

void MainWindow::show() const {
	return m_frame->show();
}

QTextBrowser* MainWindow::centralWidget() const {
	return m_frame->textBrowser();
}

ToolBar* MainWindow::mainToolBar() const {
	return m_frame->mainToolBar();
}
