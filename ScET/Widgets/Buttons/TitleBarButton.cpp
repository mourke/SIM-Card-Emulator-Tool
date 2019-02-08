#include "TitleBarButton.h"
#include <QtWidgets>
#include "AccentColor.h"

TitleBarButton::TitleBarButton(QWidget *parent) : QPushButton(parent) {

}

void TitleBarButton::setType(const Type &type) {
	m_type = type;

	if (type == Type::Close) {
		setStyleSheet("QPushButton { border: none; background-color: transparent; }  QPushButton:hover { background-color: rgb(232, 17, 35); } QPushButton:pressed { background-color: rgba(232, 17, 35, 145); }");
	} else {
		QColor accentColorSelected = accentColor();
		accentColorSelected.setAlpha(145);
		QString styleSheet = "QPushButton { border: none; background-color: transparent; }  QPushButton:hover { background-color: %1; } QPushButton:pressed { background-color: %2; }";
		setStyleSheet(styleSheet.arg(accentColor().name(QColor::HexArgb), accentColorSelected.name(QColor::HexArgb)));
	}
	

	switch (type) {
	case Type::Close:
		setToolTip("Close");
		setIcon(QIcon(":/Windows-New/Close"));
		break;
	case Type::Maximize:
		setToolTip("Maximize");
		setIcon(QIcon(":/Windows-New/Maximize"));
		break;
	case Type::Restore:
		setToolTip("Restore");
		setIcon(QIcon(":/Windows-New/Restore"));
		break;
	case Type::Minimize:
		setToolTip("Minimize");
		setIcon(QIcon(":/Windows-New/Minimize"));
		break;
	}
}

bool TitleBarButton::event(QEvent *event) {
	
	if (event->type() == QEvent::HoverEnter) {
		QPixmap pixmap = icon().pixmap(iconSize());
		QImage tmp = pixmap.toImage();
		QColor color = Qt::GlobalColor::white;

		for (int y = 0; y < tmp.height(); y++) {
			for (int x = 0; x < tmp.width(); x++) {
				color.setAlpha(tmp.pixelColor(x, y).alpha());
				tmp.setPixelColor(x, y, color);
			}
		}

		pixmap = QPixmap::fromImage(tmp);
		setIcon(QIcon(pixmap));
	} else if (event->type() == QEvent::HoverLeave) {
		setType(m_type); // reset the icon
	}
	
	return QPushButton::event(event);
}


TitleBarButton::~TitleBarButton() {

}


