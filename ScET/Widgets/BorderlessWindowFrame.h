#ifndef BORDERLESS_WINDOW_FRAME_H
#define BORDERLESS_WINDOW_FRAME_H

#include <QFrame>

class QMouseEvent;

class BorderlessWindowFrame : public QFrame {
	Q_OBJECT

public:
	BorderlessWindowFrame(QWidget *parent = Q_NULLPTR);

private:
	QPoint oldMousePosition;
	bool isMousePressed = false;
	bool isMouseAtTop = false;
	bool isMouseAtBottom = false;
	bool isMouseAtLeft = false;
	bool isMouseAtRight = false;

	void updateMouseInformation(QMouseEvent *mouseEvent);
	void closeEvent(QCloseEvent *closeEvent) override;
	void mousePressEvent(QMouseEvent *mouseEvent) override;
	void mouseMoveEvent(QMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QMouseEvent *mouseEvent) override;

protected:
	void restoreState();
	void saveState();

};

#endif // BORDERLESS_WINDOW_FRAME_H