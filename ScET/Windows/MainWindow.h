#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class MainFrame;
class ToolBar;
class QTextBrowser;

class MainWindow : public QObject {
	Q_OBJECT

	Q_PROPERTY(MainFrame frame READ frame)
	Q_PROPERTY(QTextBrowser centralWidget READ centralWidget)
	Q_PROPERTY(ToolBar mainToolBar READ mainToolBar)

public:
	MainFrame * frame() const { return m_frame; }
	QTextBrowser * centralWidget() const;
	ToolBar * mainToolBar() const;

	void show() const;
	MainWindow(QObject *parent = Q_NULLPTR);

private:
	MainFrame *m_frame;
};

#endif // MAINWINDOW_H