#ifndef FRAMELESS_MAIN_WINDOW_H
#define FRAMELESS_MAIN_WINDOW_H

#include "RemembersStateMainWindow.h"

class QMouseEvent;

class FramelessMainWindow : public RemembersStateMainWindow {
	Q_OBJECT

public:
	FramelessMainWindow(QWidget *parent = Q_NULLPTR);

#if defined(Q_OS_WIN)

private:
    bool eventFilter(QObject *watched, QEvent *event) override;

	void showEvent(QShowEvent *event) override;
	bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

	void setContentsMargins(const QMargins &contentsMargins);
	void setContentsMargins(int left, int top, int right, int bottom);
	QMargins contentsMargins() const;
	void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
	QRect contentsRect() const;

	void showFullScreen();
	QMargins insets; // the inset margins caused by the resize mouse grabber
	QMargins m_contentsMargins;
    bool rightMousePressedDown = false;

protected:
	virtual bool shouldMoveWindow();

#endif
};

#endif // FRAMELESS_MAIN_WINDOW_H
