#ifndef TITLEBARBUTTON_H
#define TITLEBARBUTTON_H

#include <QPushButton>

/** Custom window control button class.
  *
  * @b Description
  * This class provides the "Close", "Maximize"/"Restore" and
  * "Minimize" functionality for Windows 10 on all platforms.
  *
  * @b Note
  *	Tool tips will need to be localised by the user as the platform's 
  * localisations are not available.
  *
  * Also if the type is "Maximize", upon pressing the type will manually
  * need to be set to "Restore".
  *
  */
class TitleBarButton : public QPushButton {
	Q_OBJECT

	Q_PROPERTY(Type type READ type WRITE setType)

public:
	enum Type { Minimize, Maximize, Restore, Close };
	Q_ENUM(Type)

	/**
     * The type of the button
     *
     * The button's type indicates its highlight color as well as 
	 * its image and tool tip.
     *
     * @retval The button's type.
     */
	Type type() const { return m_type; }

	/**
	 * Sets the type of the button
	 *
	 * If the new type is the same as the current type, nothing 
	 * will be changed.
	 *
	 * @param type	The new type of the button.
	 */
	void setType(const Type &type);


    TitleBarButton(QWidget *parent);

protected:
	bool event(QEvent *event) override;

private:
	Type m_type;

	// The use of these methods is prohibited. Setting the "type" property
	// should suffice, otherwise use a different class.
	using QAbstractButton::setText;
	using QAbstractButton::setIcon;
};

#endif // TITLEBARBUTTON_H
