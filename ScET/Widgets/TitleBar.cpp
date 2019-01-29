#include "TitleBar.h"
#include <QtWidgets>
#include "Buttons/TitleBarButton.h"


TitleBar::TitleBar(QWidget *parent) {
	// Don't let this widget inherit the parent's backround color
	setAutoFillBackground(true);
	// Use a brush with a Highlight color role to render the background 
	//setBackgroundRole(QPalette::Highlight);

	minimize = new TitleBarButton(this, TitleBarButton::Type::Minimize);
	maximize = new TitleBarButton(this, TitleBarButton::Type::Maximize);
	close = new TitleBarButton(this, TitleBarButton::Type::Close);

	titleLabel = new QLabel(this);
	

	QHBoxLayout *hbox = new QHBoxLayout(this);

	hbox->addWidget(titleLabel);
	hbox->addWidget(minimize);
	hbox->addWidget(maximize);
	hbox->addWidget(close);

	hbox->insertStretch(1, 500);
	hbox->setSpacing(0);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	windowIsMaximized = false;

	connect(close, SIGNAL(clicked()), parent, SLOT(close()));
	connect(minimize, SIGNAL(clicked()), this, SLOT(showSmall()));
	connect(maximize, SIGNAL(clicked()), this, SLOT(showMaxRestore()));
}

QString TitleBar::text() const {
	return titleLabel->text();
}

void TitleBar::setText(const QString &text) {
	titleLabel->setText(text);
	if (QWidget *parent = dynamic_cast<QWidget *>(this->parent())) {
		parent->setWindowTitle(text);
	}
}

void TitleBar::showSmall() {
	parentWidget()->showMinimized();
}

void TitleBar::showMaxRestore() {
	if (windowIsMaximized) {
		parentWidget()->showNormal();
		windowIsMaximized = !windowIsMaximized;
		maximize->setType(TitleBarButton::Type::Maximize);
	} else {
		parentWidget()->showMaximized();
		windowIsMaximized = !windowIsMaximized;
		maximize->setType(TitleBarButton::Type::Restore);
	}
}

void TitleBar::mousePressEvent(QMouseEvent *mouseEvent) {
	startPosition = mouseEvent->globalPos();
	clickPosition = mapToParent(mouseEvent->pos());
}

void TitleBar::mouseMoveEvent(QMouseEvent *mouseEvent) {
	if (windowIsMaximized) return;
	parentWidget()->move(mouseEvent->globalPos() - clickPosition);
}
