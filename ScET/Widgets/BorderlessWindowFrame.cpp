#include "BorderlessWindowFrame.h"
#include <QtWidgets>
#include <QApplication>
#include "Colors.h"

BorderlessWindowFrame::BorderlessWindowFrame(QWidget *parent) : QFrame(parent) {
	QApplication::instance()->installEventFilter(this);
	QString styleSheet = "BorderlessWindowFrame { border: 1px solid %1; }";
	setStyleSheet(styleSheet.arg(accentColor().name(QColor::HexArgb)));
	setWindowFlags(Qt::FramelessWindowHint);

	restoreState();
}

void BorderlessWindowFrame::mousePressEvent(QMouseEvent *mouseEvent) {
	oldMousePosition = mouseEvent->globalPos();
	updateMouseInformation(mouseEvent);
}

void BorderlessWindowFrame::restoreState() {
	QSettings settings;

	settings.beginGroup("BorderlessWindowFrame");

	restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());
	move(settings.value("pos", pos()).toPoint());
	resize(settings.value("size", size()).toSize());

	if (settings.value("maximized", isMaximized()).toBool()) {
		showMaximized();
	}

	settings.endGroup();
}

void BorderlessWindowFrame::saveState() {
	QSettings settings;

	settings.beginGroup("BorderlessWindowFrame");

	settings.setValue("geometry", saveGeometry());
	settings.setValue("maximized", isMaximized());

	if (!isMaximized()) {
		settings.setValue("pos", pos());
		settings.setValue("size", size());
	}

	settings.endGroup();
}

void BorderlessWindowFrame::closeEvent(QCloseEvent *closeEvent) {
	saveState();
}


void BorderlessWindowFrame::mouseMoveEvent(QMouseEvent *mouseEvent) {
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

void BorderlessWindowFrame::updateMouseInformation(QMouseEvent *mouseEvent) {
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

void BorderlessWindowFrame::mouseReleaseEvent(QMouseEvent *mouseEvent) {
	isMousePressed = false;
}

bool BorderlessWindowFrame::eventFilter(QObject *watched, QEvent *event) {
	if (watched->parent() == this && event->type() == QEvent::Type::Enter) {
		setCursor(Qt::ArrowCursor);
	}
	return QObject::eventFilter(watched, event);
}