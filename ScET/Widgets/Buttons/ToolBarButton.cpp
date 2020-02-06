#include "ToolBarButton.h"

ToolBarButton::ToolBarButton(QWidget *parent) : QToolButton(parent) {
    QColor normal = palette().text().color();
    QColor disabled = normal;
    disabled.setAlpha(145);
    QString styleSheet = "QToolButton { color: %1; border: none; background-color: transparent; }  QToolButton:disabled { color: %2; } QToolButton:hover { background-color: rgba(0, 0, 0, 25); } QToolButton:pressed { background-color: rgba(0, 0, 0, 51); }";
    setStyleSheet(styleSheet.arg(normal.name(QColor::HexArgb), disabled.name(QColor::HexArgb)));
}


