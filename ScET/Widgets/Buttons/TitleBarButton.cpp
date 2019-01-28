#include "TitleBarButton.h"

TitleBarButton::TitleBarButton(QWidget *parent, Type type) : QAbstractButton(parent) {
	setType(type);
}

void TitleBarButton::setType(const Type &type) {
	m_type = type;

	if (type == Type::Close) {
		highlightColor = QColor(232, 17, 35);
		selectedColor = QColor(227, 101, 113);
	} else {
		highlightColor = QColor(220, 220, 220);
		selectedColor = QColor(178, 180, 184);
	}

	switch (type) {
	case Type::Close:
		setText("Close");
		setIcon(QIcon("Close"));
		break;
	case Type::Maximize:
		setText("Maximize");
		setIcon(QIcon("Maximize"));
		break;
	case Type::Restore:
		setText("Restore");
		setIcon(QIcon("Restore"));
		break;
	case Type::Minimize:
		setText("Minimize");
		setIcon(QIcon("Minimize"));
		break;
	}
}

TitleBarButton::~TitleBarButton() {

}
