#include "MainFrame.h"
#include <QtWidgets>
#include "TitleBar.h"
#include "AccentColor.h"

MainFrame::MainFrame(QWidget *parent) : QFrame(parent) {
	ui.setupUi(this);
	QString styleSheet = "MainFrame { border: 1px solid %1; }";
	setStyleSheet(styleSheet.arg(accentColor().name(QColor::HexArgb)));
	setWindowFlags(Qt::FramelessWindowHint);
}

void MainFrame::mousePressEvent(QMouseEvent *mouseEvent) {
	oldMousePosition = mouseEvent->globalPos();
	updateMouseInformation(mouseEvent);
}

void MainFrame::mouseMoveEvent(QMouseEvent *mouseEvent) {
	if (isMousePressed) {
		QRect geometry = this->geometry();
		QPoint mousePosition = mouseEvent->globalPos();

		int offsetX = oldMousePosition.x() - mousePosition.x();
		int offsetY = oldMousePosition.y() - mousePosition.y();

		if (isMouseAtLeft) {
			geometry.setX(geometry.x() - offsetX);
		} else if (isMouseAtRight) {
			geometry.setRight(geometry.right() - offsetX);
		}

		if (isMouseAtBottom) {
			geometry.setBottom(geometry.bottom() - offsetY);
		} else if (isMouseAtTop) {
			geometry.setTop(geometry.top() - offsetY);
		}

		setGeometry(geometry);
		oldMousePosition = mousePosition;
	} else {
		updateMouseInformation(mouseEvent);

		if (isMouseAtTop || isMouseAtBottom) {
			if (isMouseAtLeft) {
				setCursor(isMouseAtTop ? Qt::SizeFDiagCursor : Qt::SizeBDiagCursor);
			} else if (isMouseAtRight) {
				setCursor(isMouseAtTop ? Qt::SizeBDiagCursor : Qt::SizeFDiagCursor);
			} else {
				setCursor(Qt::SizeVerCursor);
			}
		} else if (isMouseAtLeft || isMouseAtRight) {
			setCursor(Qt::SizeHorCursor);
		} else {
			setCursor(Qt::ArrowCursor);
		}
	}
}

void MainFrame::updateMouseInformation(QMouseEvent *mouseEvent) {
	int x = mouseEvent->x();
	int y = mouseEvent->y();
	QRect frame = rect();
	int threshold = 5;

	isMousePressed = mouseEvent->button() == Qt::LeftButton;
	isMouseAtTop = y <= threshold;
	isMouseAtBottom = frame.height() - y <= threshold;
	isMouseAtLeft = x <= threshold;
	isMouseAtRight = frame.width() - x <= threshold;
}

void MainFrame::mouseReleaseEvent(QMouseEvent *mouseEvent) {
	isMousePressed = false;
}