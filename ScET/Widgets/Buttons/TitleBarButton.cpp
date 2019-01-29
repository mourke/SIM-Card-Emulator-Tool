#include "TitleBarButton.h"
#include <QtWidgets>

TitleBarButton::TitleBarButton(QWidget *parent, Type type) : QToolButton(parent) {
	setType(type);

	
}

void TitleBarButton::setType(const Type &type) {
	m_type = type;

	if (type == Type::Close) {
		setStyleSheet("QToolButton { background-color: transparent; }  QToolButton:hover { background-color: rgb(232, 17, 35); } QToolButton:pressed { background-color: rgb(227, 101, 113); }");
	} else {
		setStyleSheet("QToolButton { background-color: transparent; }  QToolButton:hover { background-color: rgb(220, 220, 220); } QToolButton:pressed { background-color: rgb(178, 180, 184); }");
	}

	switch (type) {
	case Type::Close:
		setText("Close");
		setIcon(QIcon(":/Windows-New/Close"));
		break;
	case Type::Maximize:
		setText("Maximize");
		setIcon(QIcon(":/Windows-New/Maximize"));
		break;
	case Type::Restore:
		setText("Restore");
		setIcon(QIcon(":/Windows-New/Restore"));
		break;
	case Type::Minimize:
		setText("Minimize");
		setIcon(QIcon(":/Windows-New/Minimize"));
		break;
	}
}

TitleBarButton::~TitleBarButton() {

}


