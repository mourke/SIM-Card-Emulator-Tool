#include "TitleBar.h"
#include <QtWidgets>
#include "Buttons/TitleBarButton.h"

TitleBar::TitleBar(QWidget *parent): QWidget(parent) {
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