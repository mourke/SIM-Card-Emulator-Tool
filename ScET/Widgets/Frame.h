#ifndef FRAME_H
#define FRAME_H

#include <QFrame>

class TitleBar;
class QMouseEvent;

class Frame : public QFrame {
public:

	Frame();

	// Allows you to access the content area of the frame
	// where widgets and layouts can be added
	QWidget *contentWidget() const;

	TitleBar *titleBar() const;

	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	TitleBar *m_titleBar;
	QWidget *m_content;
	QPoint m_old_pos;
	bool m_mouse_down;
	bool left, right, bottom;
};

#endif // FRAME_H