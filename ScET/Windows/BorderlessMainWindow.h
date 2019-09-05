#ifndef BORDERLESS_MAIN_WINDOW_H
#define BORDERLESS_MAIN_WINDOW_H

#include <QMainWindow>

class QMouseEvent;

class BorderlessMainWindow : public QMainWindow {
	Q_OBJECT

public:
	BorderlessMainWindow(QWidget *parent = Q_NULLPTR);

private:
	void closeEvent(QCloseEvent *closeEvent) override;
	bool eventFilter(QObject *watched, QEvent *event);

#ifdef Q_OS_WIN
	void showEvent(QShowEvent *event) override;
	bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
#endif // Q_OS_WIN

protected:
	void restoreState();
	void saveState();

	virtual bool shouldMoveWindow();
};

#endif // BORDERLESS_WINDOW_FRAME_H