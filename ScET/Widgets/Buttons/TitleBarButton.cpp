#include "TitleBarButton.h"

TitleBarButton::TitleBarButton(QWidget *parent, Type type) : QAbstractButton(parent) {
	setType(type);
}

void TitleBarButton::setType(const Type &type) {
	m_type = type;

	switch (type) {
	case Type::Close:
		highlightColor = QColor(232, 17, 35);
		setText("Close");
		setIcon(QIcon("Close"));
		break;
	case Type::Maximize:
		highlightColor = QColor(220, 220, 220);
		setText("Maximize");
		setIcon(QIcon("Maximize"));
		break;
	case Type::Minimize:
		highlightColor = QColor(220, 220, 220);
		setText("Minimize");
		setIcon(QIcon("Minimize"));
		break;
	}
}

TitleBarButton::~TitleBarButton() {

}
