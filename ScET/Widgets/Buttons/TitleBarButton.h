#ifndef TITLEBARBUTTON_H
#define TITLEBARBUTTON_H

#include <QToolButton>

class TitleBarButton : public QToolButton {
	Q_OBJECT

	Q_PROPERTY(Type type READ type WRITE setType)

public:

	enum Type { Minimize, Maximize, Restore, Close };
	Q_ENUM(Type)

	Type type() const { return m_type; }
	void setType(const Type &type);

	
	TitleBarButton(QWidget *parent, Type type);
	~TitleBarButton();

private:

	Type m_type;
};

#endif // TITLEBARBUTTON_H
