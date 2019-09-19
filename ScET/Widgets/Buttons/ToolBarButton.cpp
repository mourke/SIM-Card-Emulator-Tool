#include "ToolBarButton.h"

ToolBarButton::ToolBarButton(QWidget *parent) : QToolButton(parent) {
	setStyleSheet("QToolButton { border: none; background-color: transparent; }  QToolButton:hover { background-color: rgba(0, 0, 0, 25); } QToolButton:pressed { background-color: rgba(0, 0, 0, 51); }");
}

ToolBarButton::~ToolBarButton() {
}
