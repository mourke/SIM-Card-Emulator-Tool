#include "TitleBarButton.h"
#include <QtWidgets>

TitleBarButton::TitleBarButton(QWidget *parent, Type type) : QToolButton(parent) {
	setType(type);

	
}

void TitleBarButton::setType(const Type &type) {
	m_type = type;

	if (type == Type::Close) {
		setStyleSheet("QToolButton { border: none; background-color: transparent; }  QToolButton:hover { background-color: rgb(232, 17, 35); } QToolButton:pressed { background-color: rgba(232, 17, 35, 70); }");
	} else {
		setStyleSheet("QToolButton { border: none; background-color: transparent; }  QToolButton:hover { background-color: rgba(0, 0, 0, 20); } QToolButton:pressed { background-color: rgba(0, 0, 0, 30); }");
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


