#include "BorderlessMainWindow.h"
#include <QtWidgets>
#ifdef Q_OS_WIN
#include <Windows.h>
#include <windowsx.h>
#endif //Q_OS_WIN

BorderlessMainWindow::BorderlessMainWindow(QWidget *parent) : QMainWindow(parent) {
	QApplication::instance()->installEventFilter(this);
	restoreState();
}

void BorderlessMainWindow::restoreState() {
	QSettings settings;

	settings.beginGroup("BorderlessMainWindow");

	restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());
	move(settings.value("pos", pos()).toPoint());
	resize(settings.value("size", size()).toSize());

	if (settings.value("maximized", isMaximized()).toBool()) {
		showMaximized();
	}

	settings.endGroup();
}

void BorderlessMainWindow::saveState() {
	QSettings settings;

	settings.beginGroup("BorderlessMainWindow");

	settings.setValue("geometry", saveGeometry());
	settings.setValue("maximized", isMaximized());

	if (!isMaximized()) {
		settings.setValue("pos", pos());
		settings.setValue("size", size());
	}

	settings.endGroup();
}

void BorderlessMainWindow::closeEvent(QCloseEvent *closeEvent) {
	saveState();
}


#ifdef Q_OS_WIN
void BorderlessMainWindow::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);
	if (isVisible()) {
		SetWindowLong((HWND)winId(), GWL_STYLE,
			GetWindowLong((HWND)winId(), GWL_STYLE) & ~WS_CAPTION);
		SetWindowPos(reinterpret_cast<HWND>(winId()), NULL, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE |
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW);
	}
}

bool BorderlessMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
	MSG *msg = static_cast<MSG *>(message);
	switch (msg->message) {
	case WM_NCCALCSIZE: {
		//this kills the window frame and title bar we added with
		//WS_THICKFRAME and WS_CAPTION
		*result = 0;
		return true;
		break;
	}
	case WM_NCHITTEST: {
		*result = 0;
		const LONG border_width = 8; //in pixels
		RECT winrect;
		GetWindowRect((HWND)winId(), &winrect);

		long x = GET_X_LPARAM(msg->lParam);
		long y = GET_Y_LPARAM(msg->lParam);

		bool resizeWidth = minimumWidth() != maximumWidth();
		bool resizeHeight = minimumHeight() != maximumHeight();
		if (resizeWidth) {
			//left border
			if (x >= winrect.left && x < winrect.left + border_width) {
				*result = HTLEFT;
			}
			//right border
			if (x < winrect.right && x >= winrect.right - border_width) {
				*result = HTRIGHT;
			}
		}
		if (resizeHeight) {
			//bottom border
			if (y < winrect.bottom && y >= winrect.bottom - border_width) {
				*result = HTBOTTOM;
			}
			//top border
			if (y >= winrect.top && y < winrect.top + border_width) {
				*result = HTTOP;
			}
		}
		if (resizeWidth && resizeHeight) {
			//bottom left corner
			if (x >= winrect.left && x < winrect.left + border_width &&
				y < winrect.bottom && y >= winrect.bottom - border_width) {
				*result = HTBOTTOMLEFT;
			}
			//bottom right corner
			if (x < winrect.right && x >= winrect.right - border_width &&
				y < winrect.bottom && y >= winrect.bottom - border_width) {
				*result = HTBOTTOMRIGHT;
			}
			//top left corner
			if (x >= winrect.left && x < winrect.left + border_width &&
				y >= winrect.top && y < winrect.top + border_width) {
				*result = HTTOPLEFT;
			}
			//top right corner
			if (x < winrect.right && x >= winrect.right - border_width &&
				y >= winrect.top && y < winrect.top + border_width) {
				*result = HTTOPRIGHT;
			}
		}

		if (*result != 0)
			return true;

		
		if (shouldMoveWindow()) {
			*result = HTCAPTION;
			return true;
		}
		break;
	} //end case WM_NCHITTEST
	}

	return QWidget::nativeEvent(eventType, message, result);
}

#endif //Q_OS_WIN

bool BorderlessMainWindow::shouldMoveWindow() {
	QWidget *action = QApplication::widgetAt(QCursor::pos());
	return action == this;
}

bool BorderlessMainWindow::eventFilter(QObject *watched, QEvent *event) {
	if (watched->parent() == this && event->type() == QEvent::Type::Enter) {
		setCursor(Qt::ArrowCursor);
	}
	return QObject::eventFilter(watched, event);
}