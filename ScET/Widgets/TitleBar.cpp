#include "TitleBar.h"
#include <QtWidgets>
#include "Buttons/TitleBarButton.h"
#include "MainFrame.h"

TitleBar::TitleBar(QWidget *parent): QWidget(parent) {
}

void TitleBar::enterEvent(QEvent *event) {
	setCursor(Qt::ArrowCursor); // make sure resize cursor isn't selected
}

TitleBarButton* TitleBar::minimizeButton() {
	return mainFrame()->ui.minimizeButton;
}
TitleBarButton* TitleBar::maximizeButton() {
	return mainFrame()->ui.maximizeButton;
}
TitleBarButton* TitleBar::closeButton() {
	return mainFrame()->ui.closeButton;
}

MainFrame* TitleBar::mainFrame() {
	return qobject_cast<MainFrame *>(parentWidget());
}

void TitleBar::contextMenuEvent(QContextMenuEvent *event) {
	QMenu contextMenu(this);

	QAction restore(tr("Restore"), this);
	restore.setIcon(QIcon(style()->standardPixmap(QStyle::SP_TitleBarNormalButton)));
	restore.setEnabled(parentWidget()->isMaximized());
	connect(&restore, SIGNAL(triggered()), this, SLOT(showMaxRestore()));
	contextMenu.addAction(&restore);

	QAction minimize(tr("Minimize"), this);
	minimize.setIcon(QIcon(style()->standardPixmap(QStyle::SP_TitleBarMinButton)));
	connect(&minimize, SIGNAL(triggered()), this, SLOT(showSmall()));
	contextMenu.addAction(&minimize);

	QAction maximize(tr("Maximize"), this);
	maximize.setIcon(QIcon(style()->standardPixmap(QStyle::SP_TitleBarMaxButton)));
	maximize.setDisabled(parentWidget()->isMaximized());
	connect(&maximize, SIGNAL(triggered()), this, SLOT(showMaxRestore()));
	contextMenu.addAction(&maximize);

	contextMenu.addSeparator();
	QAction close(tr("Close") + "\tAlt+F4", this);
	close.setIcon(QIcon(style()->standardPixmap(QStyle::SP_TitleBarCloseButton)));
	close.setShortcut(QKeySequence::Quit);
	connect(&close, SIGNAL(triggered()), parent(), SLOT(close()));
	contextMenu.addAction(&close);

	contextMenu.exec(mapToGlobal(event->pos()));
}

void TitleBar::showSmall() {
	parentWidget()->showMinimized();
}

void TitleBar::showMaxRestore() {
	if (parentWidget()->isMaximized()) {
		parentWidget()->showNormal();
		maximizeButton()->setType(TitleBarButton::Type::Maximize);
	} else {
		parentWidget()->showMaximized();
		maximizeButton()->setType(TitleBarButton::Type::Restore);
	}
}

void TitleBar::mousePressEvent(QMouseEvent *mouseEvent) {
	clickPosition = mapToParent(mouseEvent->pos());

	shouldMoveWindow = cursor() == Qt::ArrowCursor && mouseEvent->button() == Qt::LeftButton;
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *mouseEvent) {
	if (mouseEvent->button() == Qt::LeftButton) showMaxRestore();
}

void TitleBar::mouseMoveEvent(QMouseEvent *mouseEvent) {
	if (!shouldMoveWindow) return;

	if (parentWidget()->isMaximized()) {
		showMaxRestore();
	}

	QScreen *screen = QGuiApplication::primaryScreen();
	QCursor cursor;
	QPoint cursorPosition = cursor.pos();

	QPoint newWindowPosition = mouseEvent->globalPos() - clickPosition;
	int maxScreenHeight = screen->availableGeometry().height();

	// Stop window falling off bottom of screen
	if (cursorPosition.y() >= maxScreenHeight) {
		cursor.setPos(cursorPosition.x(), maxScreenHeight);
		newWindowPosition.setY(parentWidget()->geometry().y());
	} 

	parentWidget()->move(newWindowPosition);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *mouseEvent) {
	shouldMoveWindow = false;
}
