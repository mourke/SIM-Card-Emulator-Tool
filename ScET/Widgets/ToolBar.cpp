#include "ToolBar.h"

ToolBar::ToolBar(QWidget *parent) : QWidget(parent) {
}

void ToolBar::enterEvent(QEvent *event) {
	setCursor(Qt::ArrowCursor); // make sure resize cursor isn't selected
}

ToolBar::~ToolBar() {
}
