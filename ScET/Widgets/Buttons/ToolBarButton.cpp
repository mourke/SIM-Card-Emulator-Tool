#include "ToolBarButton.h"

ToolBarButton::ToolBarButton(QWidget *parent) : QPushButton(parent) {
	setStyleSheet("QPushButton { border: none; background-color: transparent; }  QPushButton:hover { background-color: rgba(0, 0, 0, 25); } QPushButton:pressed { background-color: rgba(0, 0, 0, 51); }");
}

ToolBarButton::~ToolBarButton() {
}
