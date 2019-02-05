#ifndef TITLEBARBUTTON_H
#define TITLEBARBUTTON_H

#include <QPushButton>
#include <QtUiPlugin/QDesignerExportWidget>

class /* QDESIGNER_WIDGET_EXPORT */ TitleBarButton : public QPushButton {
	Q_OBJECT

	Q_PROPERTY(Type type READ type WRITE setType)

public:

	enum Type { Minimize, Maximize, Restore, Close };
	Q_ENUM(Type)

	Type type() const { return m_type; }
	void setType(const Type &type);

	
	TitleBarButton(QWidget *parent);
	~TitleBarButton();

private:
	Type m_type;
};

#endif // TITLEBARBUTTON_H
