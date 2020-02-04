#ifndef TOOL_BAR_BUTTON_H
#define	TOOL_BAR_BUTTON_H

#include <QToolButton>

/** A Tool Bar buttons class.
 * 
 * @b Description
 * These buttons are specially designed to behave as buttons
 * commonly found in applications' tool bars. For example in
 * Windows 10 these buttons are 48x48 px and and have a clear
 * background until hovered over.
 *
 */
class ToolBarButton : public QToolButton {
	Q_OBJECT

public:
    ToolBarButton(QWidget *parent);
};

#endif // TOOLBARBUTTON_H
