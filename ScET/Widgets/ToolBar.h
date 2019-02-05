#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>

class ToolBar : public QWidget {
	Q_OBJECT

public:
	ToolBar(QWidget *parent);
	~ToolBar();

private:
	void enterEvent(QEvent *event) override;
};

#endif // TOOLBAR_H
