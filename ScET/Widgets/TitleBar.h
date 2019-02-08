#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class TitleBarButton;
class MainFrame;

class TitleBar : public QWidget {
	Q_OBJECT

public:
	TitleBar(QWidget *parent);

public slots:
	void showSmall();
	void showMaxRestore();

protected:
	void contextMenuEvent(QContextMenuEvent *event);
	void mousePressEvent(QMouseEvent *mouseEvent);
	void mouseDoubleClickEvent(QMouseEvent *mouseEvent);
	void mouseMoveEvent(QMouseEvent *mouseEvent);
	void mouseReleaseEvent(QMouseEvent *mouseEvent);
	void enterEvent(QEvent *event) override;

private:
	TitleBarButton* minimizeButton();
	TitleBarButton* maximizeButton();
	TitleBarButton* closeButton();
	MainFrame* mainFrame();
	bool shouldMoveWindow = false;
	QPoint clickPosition;
};

#endif // TITLEBAR_H
