#include "TitleBar.h"
#include <QtWidgets>
#include "Buttons/TitleBarButton.h"


TitleBar::TitleBar(QWidget *parent) {
	// Don't let this widget inherit the parent's backround color
	setAutoFillBackground(true);
	// Use a brush with a Highlight color role to render the background 
	//setBackgroundRole(QPalette::Highlight);

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
	hbox->setMargin(0);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	windowIsMaximized = false;

	connect(closeButton, SIGNAL(clicked()), parent, SLOT(close()));
	connect(minimizeButton, SIGNAL(clicked()), this, SLOT(showSmall()));
	connect(maximizeButton, SIGNAL(clicked()), this, SLOT(showMaxRestore()));
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
	if (windowIsMaximized) {
		parentWidget()->showNormal();
		windowIsMaximized = !windowIsMaximized;
		maximizeButton->setType(TitleBarButton::Type::Maximize);
	} else {
		parentWidget()->showMaximized();
		windowIsMaximized = !windowIsMaximized;
		maximizeButton->setType(TitleBarButton::Type::Restore);
	}
}

void TitleBar::mousePressEvent(QMouseEvent *mouseEvent) {
	clickPosition = mapToParent(mouseEvent->pos());
}

void TitleBar::mouseMoveEvent(QMouseEvent *mouseEvent) {
	qDebug() << "Called mouse move event";
	if (windowIsMaximized) return;
	parentWidget()->move(mouseEvent->globalPos() - clickPosition);
}
