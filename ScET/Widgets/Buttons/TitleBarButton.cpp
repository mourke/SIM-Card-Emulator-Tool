#include "TitleBarButton.h"
#include <QtWidgets>
#include "AccentColor.h"

TitleBarButton::TitleBarButton(QWidget *parent) : QPushButton(parent) {

}

void TitleBarButton::setType(const Type &type) {
	m_type = type;

	QColor accentColorSelected = accentColor();
	accentColorSelected.setAlpha(145);
	QString styleSheet;

	if (type == Type::Close) {
		styleSheet = "QPushButton { border: none; background-color: transparent; }  QPushButton:hover { background-color: rgb(232, 17, 35); } QPushButton:pressed { background-color: rgba(232, 17, 35, 145); }";
	} else {
		styleSheet = "QPushButton { border: none; background-color: transparent; }  QPushButton:hover { background-color: %1; } QPushButton:pressed { background-color: %2; }";
	}

	setStyleSheet(styleSheet.arg(accentColor().name(QColor::HexArgb), accentColorSelected.name(QColor::HexArgb)));

	switch (type) {
	case Type::Close:
		setIcon(QIcon(":/Windows-New/Close"));
		break;
	case Type::Maximize:
		setIcon(QIcon(":/Windows-New/Maximize"));
		break;
	case Type::Restore:
		setIcon(QIcon(":/Windows-New/Restore"));
		break;
	case Type::Minimize:
		setIcon(QIcon(":/Windows-New/Minimize"));
		break;
	}
}


TitleBarButton::~TitleBarButton() {

}


