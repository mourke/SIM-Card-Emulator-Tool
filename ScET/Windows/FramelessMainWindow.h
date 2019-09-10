#ifndef FRAMELESS_MAIN_WINDOW_H
#define FRAMELESS_MAIN_WINDOW_H

#include <QMainWindow>

class QMouseEvent;

class FramelessMainWindow : public QMainWindow {
	Q_OBJECT

public:
	FramelessMainWindow(QWidget *parent = Q_NULLPTR);

private:
	void closeEvent(QCloseEvent *closeEvent) override;
	bool eventFilter(QObject *watched, QEvent *event);

#ifdef Q_OS_WIN
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
#endif // Q_OS_WIN

protected:
	void restoreState();
	void saveState();

	virtual bool shouldMoveWindow();
};

#endif // FRAMELESS_MAIN_WINDOW_H