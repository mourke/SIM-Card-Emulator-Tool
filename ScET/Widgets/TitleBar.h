#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class TitleBar : public QWidget {
	Q_OBJECT

public:
	TitleBar(QWidget *parent = Q_NULLPTR);

protected:
	void contextMenuEvent(QContextMenuEvent *event);
};

#endif // TITLEBAR_H
