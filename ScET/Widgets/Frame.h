#ifndef FRAME_H
#define FRAME_H

#include <QFrame>

class TitleBar;
class QMouseEvent;

class Frame : public QFrame {
	Q_OBJECT

	Q_PROPERTY(QWidget contentWidget READ contentWidget)
	Q_PROPERTY(TitleBar titleBar READ titleBar)

public:

	Frame();

	QWidget *contentWidget() const;
	TitleBar *titleBar() const;

private:
	TitleBar *m_titleBar;
	QWidget *m_contentWidget;
	QPoint oldMousePosition;
	bool isMousePressed;
	bool isMouseAtTop;
	bool isMouseAtBottom;
	bool isMouseAtLeft;
	bool isMouseAtRight;

	
	void updateMouseInformation(QMouseEvent *mouseEvent);
	void mousePressEvent(QMouseEvent *mouseEvent) override;
	void mouseMoveEvent(QMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
};

#endif // FRAME_H