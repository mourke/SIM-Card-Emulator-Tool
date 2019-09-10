#include "FramelessMainWindow.h"
#include <QtWidgets>
#include "Colors.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#pragma comment (lib,"Dwmapi.lib") 
#endif //Q_OS_WIN

FramelessMainWindow::FramelessMainWindow(QWidget *parent) : QMainWindow(parent) {
	QApplication::instance()->installEventFilter(this);
	restoreState();
	QString styleSheet = "FramelessMainWindow { border: 1px solid %1; }";
	setStyleSheet(styleSheet.arg(accentColor().name(QColor::HexArgb)));
	setContentsMargins(1, 1, 1, 1); // account for border width
	setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMaximizeButtonHint);
}


void FramelessMainWindow::restoreState() {
	QSettings settings;

	settings.beginGroup("FramelessMainWindow");

	restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());
	move(settings.value("pos", pos()).toPoint());
	resize(settings.value("size", size()).toSize());

	if (settings.value("maximized", isMaximized()).toBool()) {
		showMaximized();
	}

	settings.endGroup();
}

void FramelessMainWindow::saveState() {
	QSettings settings;

	settings.beginGroup("FramelessMainWindow");

	settings.setValue("geometry", saveGeometry());
	settings.setValue("maximized", isMaximized());

	if (!isMaximized()) {
		settings.setValue("pos", pos());
		settings.setValue("size", size());
	}

	settings.endGroup();
}

void FramelessMainWindow::closeEvent(QCloseEvent *closeEvent) {
	saveState();
}


#ifdef Q_OS_WIN
void FramelessMainWindow::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);
	HWND hwnd = (HWND)this->winId();
	DWORD style = GetWindowLong(hwnd, GWL_STYLE);
	SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
	const MARGINS shadow = { 1, 1, 1, 1 };
	DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
}

bool FramelessMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
	MSG *msg = static_cast<MSG *>(message);
	switch (msg->message) {
	case WM_NCCALCSIZE: {
		NCCALCSIZE_PARAMS &params = *reinterpret_cast<NCCALCSIZE_PARAMS *>(msg->lParam);
		//this kills the window frame and title bar we added with
		//WS_THICKFRAME and WS_CAPTION
		*result = WVR_REDRAW;
		return true;
	}
	case WM_NCHITTEST: {
		*result = 0;
		const LONG border_width = 5; //in pixels
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

		if (*result != 0) {
			return true;
		}

		
		if (shouldMoveWindow()) {
			*result = HTCAPTION;
			return true;
		}
		break;
	}
	case WM_GETMINMAXINFO: {
		if (IsZoomed(msg->hwnd)) {
			RECT frame = {0, 0, 0, 0};
			AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);

			double pixelRatio = this->devicePixelRatioF() + 0.5;

			insets.setLeft(abs(frame.left) / pixelRatio);
			insets.setTop(abs(frame.bottom) / pixelRatio);
			insets.setRight(abs(frame.right) / pixelRatio);
			insets.setBottom(abs(frame.bottom) / pixelRatio);

			QMainWindow::setContentsMargins(insets + m_contentsMargins); // add inset padding when in fullscreen
		} else {
			QMainWindow::setContentsMargins(m_contentsMargins); // restore to default padding
		}
		return false;
	}
	}

	return QWidget::nativeEvent(eventType, message, result);
}

void FramelessMainWindow::setContentsMargins(const QMargins &contentsMargins) {
	if (m_contentsMargins == contentsMargins) return;
	QMainWindow::setContentsMargins(contentsMargins + insets);
	m_contentsMargins = contentsMargins;
}

void FramelessMainWindow::setContentsMargins(int left, int top, int right, int bottom) {
	if (m_contentsMargins == QMargins(left, top, right, bottom)) return;
	QMainWindow::setContentsMargins(left + insets.left(), top + insets.top(), right + insets.right(), bottom + insets.bottom());

	m_contentsMargins.setLeft(left);
	m_contentsMargins.setTop(top);
	m_contentsMargins.setRight(right);
	m_contentsMargins.setBottom(bottom);
}

QMargins FramelessMainWindow::contentsMargins() const {
	QMargins margins = QMainWindow::contentsMargins();
	margins -= insets;
	return margins;
}

void FramelessMainWindow::getContentsMargins(int *left, int *top, int *right, int *bottom) const {
	QMainWindow::getContentsMargins(left, top, right, bottom);
	if (!(left && top && right && bottom)) return;

	if (isMaximized()) {
		*left -= insets.left();
		*top -= insets.top();
		*right -= insets.right();
		*bottom -= insets.bottom();
	}
}

QRect FramelessMainWindow::contentsRect() const {
	QRect rect = QMainWindow::contentsRect();
	int width = rect.width();
	int height = rect.height();
	rect.setLeft(rect.left() - insets.left());
	rect.setTop(rect.top() - insets.top());
	rect.setWidth(width);
	rect.setHeight(height);
	return rect;
}

void FramelessMainWindow::showFullScreen() {
	if (isMaximized()) {
		QMainWindow::setContentsMargins(m_contentsMargins);
		insets = QMargins();
	}
	QMainWindow::showFullScreen();
}

#endif //Q_OS_WIN

bool FramelessMainWindow::shouldMoveWindow() {
	QWidget *action = QApplication::widgetAt(QCursor::pos());
	return action == this;
}

bool FramelessMainWindow::eventFilter(QObject *watched, QEvent *event) {
	if (watched->parent() == this && event->type() == QEvent::Type::Enter) {
		setCursor(Qt::ArrowCursor);
	}
	return QObject::eventFilter(watched, event);
}