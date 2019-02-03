#include "TitleBar.h"
#include <QtWidgets>
#include "Buttons/TitleBarButton.h"



TitleBar::TitleBar(QWidget *parent) {
	// Don't let this widget inherit the parent's backround color
	setAutoFillBackground(true);
	setBackgroundRole(QPalette::Light);

	minimizeButton = new TitleBarButton(this, TitleBarButton::Type::Minimize);
	maximizeButton = new TitleBarButton(this, TitleBarButton::Type::Maximize);
	closeButton = new TitleBarButton(this, TitleBarButton::Type::Close);

	closeButton->setIconSize(QSize(10, 10));
	closeButton->setMinimumSize(QSize(45, 30));
	maximizeButton->setIconSize(QSize(10, 10));
	maximizeButton->setMinimumSize(QSize(45, 30));
	minimizeButton->setIconSize(QSize(10, 10));
	minimizeButton->setMinimumSize(QSize(45, 30));

	titleLabel = new QLabel(this);
	

	QHBoxLayout *hbox = new QHBoxLayout(this);

	hbox->addWidget(titleLabel);
	hbox->addWidget(minimizeButton);
	hbox->addWidget(maximizeButton);
	hbox->addWidget(closeButton);

	hbox->insertStretch(1, 500);
	hbox->setSpacing(0);
	hbox->setContentsMargins(15, 0, 0, 0);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	connect(closeButton, SIGNAL(clicked()), parent, SLOT(close()));
	connect(minimizeButton, SIGNAL(clicked()), this, SLOT(showSmall()));
	connect(maximizeButton, SIGNAL(clicked()), this, SLOT(showMaxRestore()));

}

void TitleBar::contextMenuEvent(QContextMenuEvent *event) {
	QMenu contextMenu(this);

	QAction restore("Restore", this);
	restore.setIcon(QIcon(style()->standardPixmap(QStyle::SP_TitleBarNormalButton)));
	restore.setEnabled(parentWidget()->isMaximized());
	connect(&restore, SIGNAL(triggered()), this, SLOT(showMaxRestore()));
	contextMenu.addAction(&restore);

	QAction minimize("Minimize", this);
	minimize.setIcon(QIcon(style()->standardPixmap(QStyle::SP_TitleBarMinButton)));
	connect(&minimize, SIGNAL(triggered()), this, SLOT(showSmall()));
	contextMenu.addAction(&minimize);

	QAction maximize("Maximize", this);
	maximize.setIcon(QIcon(style()->standardPixmap(QStyle::SP_TitleBarMaxButton)));
	maximize.setDisabled(parentWidget()->isMaximized());
	connect(&maximize, SIGNAL(triggered()), this, SLOT(showMaxRestore()));
	contextMenu.addAction(&maximize);

	contextMenu.addSeparator();
	QAction close("Close", this);
	close.setIcon(QIcon(style()->standardPixmap(QStyle::SP_TitleBarCloseButton)));
	close.setShortcut(QKeySequence::Quit);
	connect(&close, SIGNAL(triggered()), parent(), SLOT(close()));
	contextMenu.addAction(&close);

	contextMenu.exec(mapToGlobal(event->pos()));
}

QString TitleBar::text() const {
	return titleLabel->text();
}

void TitleBar::setText(const QString &text) {
	titleLabel->setText(text);
	parentWidget()->setWindowTitle(text);
}

void TitleBar::showSmall() {
	parentWidget()->showMinimized();
}

void TitleBar::showMaxRestore() {
	if (parentWidget()->isMaximized()) {
		parentWidget()->showNormal();
		maximizeButton->setType(TitleBarButton::Type::Maximize);
	} else {
		parentWidget()->showMaximized();
		maximizeButton->setType(TitleBarButton::Type::Restore);
	}
}

void TitleBar::mousePressEvent(QMouseEvent *mouseEvent) {
	clickPosition = mapToParent(mouseEvent->pos());

	shouldMoveWindow = cursor() == Qt::ArrowCursor && !parentWidget()->isMaximized() && mouseEvent->button() == Qt::LeftButton;
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *mouseEvent) {
	if (mouseEvent->button() == Qt::LeftButton) showMaxRestore();
}

void TitleBar::mouseMoveEvent(QMouseEvent *mouseEvent) {
	if (!shouldMoveWindow) return;
	parentWidget()->move(mouseEvent->globalPos() - clickPosition);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *mouseEvent) {
	shouldMoveWindow = false;
}
