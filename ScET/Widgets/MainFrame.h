#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include "ui_MainFrame.h"

class TitleBar;
class ToolBar;
class QMouseEvent;

class MainFrame : public QFrame {
	Q_OBJECT

	Q_PROPERTY(QTextBrowser textBrowser READ textBrowser)
	Q_PROPERTY(TitleBar titleBar READ titleBar)
	Q_PROPERTY(ToolBar mainToolBar READ mainToolBar)

public:

	MainFrame(QWidget *parent = Q_NULLPTR);

	QTextBrowser* textBrowser() const { return ui.textBrowser; }
	ToolBar* mainToolBar() const { return ui.mainToolBar; }
	TitleBar* titleBar() const { return ui.titleBar;  }

private:
	Ui::MainFrame ui;
	
	QPoint oldMousePosition;
	bool isMousePressed = false;
	bool isMouseAtTop = false;
	bool isMouseAtBottom = false;
	bool isMouseAtLeft = false;
	bool isMouseAtRight = false;

	
	void updateMouseInformation(QMouseEvent *mouseEvent);
	void mousePressEvent(QMouseEvent *mouseEvent) override;
	void mouseMoveEvent(QMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QMouseEvent *mouseEvent) override;

	friend TitleBar;
};

#endif // FRAME_H