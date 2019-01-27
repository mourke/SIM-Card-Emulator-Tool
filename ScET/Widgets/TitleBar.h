#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class QMouseEvent;
class QToolButton;

class TitleBar : public QWidget {
	Q_OBJECT
public:
	TitleBar(QWidget *parent);

public slots:
	void showSmall();
	void showMaxRestore();

protected:
	void mousePressEvent(QMouseEvent *me);
	void mouseMoveEvent(QMouseEvent *me);

private:
	QToolButton *minimize;
	QToolButton *maximize;
	QToolButton *close;
	QPixmap restorePix, maxPix;
	bool maxNormal;
	QPoint startPos;
	QPoint clickPos;
};

#endif // TITLEBAR_H
