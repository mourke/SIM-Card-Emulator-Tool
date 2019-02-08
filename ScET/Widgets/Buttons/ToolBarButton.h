#ifndef TOOLBARBUTTON_H
#define	TOOLBARBUTTON_H

#include <QPushButton>

class ToolBarButton : public QPushButton {
	Q_OBJECT

public:
	ToolBarButton(QWidget *parent);
	~ToolBarButton();
};

#endif // TOOLBARBUTTON_H
