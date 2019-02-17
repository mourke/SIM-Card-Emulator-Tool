#ifndef TOOLBARBUTTON_H
#define	TOOLBARBUTTON_H

#include <QPushButton>

/** A Tool Bar buttons class.
 * 
 * @b Description
 * These buttons are specially designed to behave as buttons
 * commonly found in applications' tool bars. For example in
 * Windows 10 these buttons are 48x48 px and and have a clear
 * background until hovered over.
 *
 */
class ToolBarButton : public QPushButton {
	Q_OBJECT

public:
	ToolBarButton(QWidget *parent);
	~ToolBarButton();
};

#endif // TOOLBARBUTTON_H
