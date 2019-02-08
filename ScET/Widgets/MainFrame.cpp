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

		bool isMinWidth = geometry.width() <= minimumWidth();
		bool isMinHeight = geometry.height() <= minimumHeight();


		if (isMouseAtLeft) {
			if ((isMinWidth && offsetX <= 0)) return;
			geometry.setX(geometry.x() - offsetX);
		} else if (isMouseAtRight) {
			if ((isMinWidth && offsetX >= 0)) return;
			geometry.setRight(geometry.right() - offsetX);
		}

		if (isMouseAtBottom) {
			if ((isMinHeight && offsetY >= 0)) return;
			geometry.setBottom(geometry.bottom() - offsetY);
		} else if (isMouseAtTop) {
			if ((isMinHeight && offsetY <= 0)) return;
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