#include "Segment.h"
#include <QtWidgets>
#include "Colors.h"

Segment::Segment(QWidget *parent) : QAbstractButton(parent) {
	setCheckable(true);
	QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(setSelected(bool)));
}

void Segment::setSelected(const bool &selected) {
	setChecked(selected);
	if (selected) emit this->selected();
}

Segment::~Segment() {
}

void Segment::paintEvent(QPaintEvent *paintEvent) {
	QPainter painter(this);

	QColor textColor;

#ifdef Q_OS_MAC
	QColor fillColor;
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	
	if (position == Position::Default) {
		path.addRect(QRect(0, 0, width(), height()));
	} else {
		path.addRoundedRect(QRect(0, 0, width(), height()), CORNER_RADIUS, CORNER_RADIUS);
		
		if (position == Position::Start) {
			path.addRect(QRect(width() - CORNER_RADIUS, 0, CORNER_RADIUS, CORNER_RADIUS)); // Mask top right corner
			path.addRect(QRect(width() - CORNER_RADIUS, height() - CORNER_RADIUS, CORNER_RADIUS, CORNER_RADIUS)); // Mask bottom right corner
		} else if (position == Position::End) {
			path.addRect(QRect(0, 0, CORNER_RADIUS, CORNER_RADIUS)); // Mask top left corner
			path.addRect(QRect(0, height() - CORNER_RADIUS, CORNER_RADIUS, CORNER_RADIUS)); // Mask bottom left corner
		}
	}
	
#endif
	

	if (isSelected()) {
		textColor = selectedTextColor();
#if defined(Q_OS_MAC)
		fillColor = accentColor();
#elif defined(Q_OS_WIN)
		painter.fillRect(0, height() - SELECTED_INDICATOR_HEIGHT, width(), SELECTED_INDICATOR_HEIGHT, accentColor());
#endif
	} else if (isHighlighted()) {
#ifdef Q_OS_MAC
		fillColor = Qt::white;
#endif
		textColor = highlightedTextColor();
	} else if (isPressed()) {
#ifdef Q_OS_MAC
		fillColor = QColor(0, 0, 0, 80);
#endif
		textColor = pressedTextColor();
	} else {
#ifdef Q_OS_MAC
		fillColor = Qt::white;
#endif
		textColor = this->textColor();
	}

#ifdef Q_OS_MAC
	painter.fillPath(path.simplified(), fillColor);
#endif
	painter.setFont(m_textFont);

	if (!text().isEmpty()) {
		painter.setPen(textColor);
		painter.drawText(rect(), text());
	} else {
		QSize size = iconSize();
		painter.drawPixmap(width() - size.width() / 2, height() - size.height() / 2, icon().pixmap(size));
	}
}

void Segment::mousePressEvent(QMouseEvent *mouseEvent) {
	if (!isSelected()) { // the user should not be able to deselect the segment, this should only be done by its controller.
		QAbstractButton::mousePressEvent(mouseEvent); 
	}
}

void Segment::enterEvent(QEvent *event) {
	highlighted = true;
}

void Segment::leaveEvent(QEvent *event) {
	highlighted = false;
}

void Segment::sizeToFit() {
	static const int spacing = 12;
	QFontMetrics metrics(m_textFont);
	setMinimumSize(metrics.width(text()), metrics.height() + spacing + SELECTED_INDICATOR_HEIGHT);
}
