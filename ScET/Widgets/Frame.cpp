#include "Frame.h"
#include <QtWidgets>
#include "TitleBar.h"


Frame::Frame() {
	setFrameShape(Panel);

	setWindowFlags(Qt::FramelessWindowHint);
	setMouseTracking(true); // We need to recieve all mouse events even when the mouse is not being pressed.

	m_titleBar = new TitleBar(this);
	m_contentWidget = new QWidget(this);

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(m_titleBar);
	vbox->setMargin(0);
	vbox->setSpacing(0);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(m_contentWidget);
	layout->setMargin(5);
	layout->setSpacing(0);
	vbox->addLayout(layout);
}

QWidget* Frame::contentWidget() const { return m_contentWidget; }

TitleBar* Frame::titleBar() const { return m_titleBar; }

void Frame::mousePressEvent(QMouseEvent *mouseEvent) {
	oldMousePosition = mouseEvent->globalPos();
	updateMouseInformation(mouseEvent);
}

void Frame::mouseMoveEvent(QMouseEvent *mouseEvent) {
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
				setCursor(Qt::SizeBDiagCursor);
			} else if (isMouseAtRight) {
				setCursor(Qt::SizeFDiagCursor);
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

void Frame::updateMouseInformation(QMouseEvent *mouseEvent) {
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

void Frame::mouseReleaseEvent(QMouseEvent *mouseEvent) {
	isMousePressed = false;
}