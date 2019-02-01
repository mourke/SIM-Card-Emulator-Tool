#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class QMouseEvent;
class TitleBarButton;
class QLabel;

class TitleBar : public QWidget {
	Q_OBJECT

	Q_PROPERTY(QString text READ text WRITE setText)
public:
	TitleBar(QWidget *parent);

	QString text() const;
	void setText(const QString &text);

public slots:
	void showSmall();
	void showMaxRestore();

protected:
	void mousePressEvent(QMouseEvent *mouseEvent);
	void mouseMoveEvent(QMouseEvent *mouseEvent);

private:
	TitleBarButton *minimizeButton;
	TitleBarButton *maximizeButton;
	TitleBarButton *closeButton;
	bool windowIsMaximized;
	QPoint clickPosition;
	QLabel *titleLabel;
};

#endif // TITLEBAR_H
