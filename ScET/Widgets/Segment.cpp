#include "Segment.h"
#include <QtWidgets>
#include "AccentColor.h"

Segment::Segment(QWidget *parent) : QAbstractButton(parent) {
	setCheckable(true);
	QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(setSelected(bool)));
}

void Segment::setSelected(const bool &selected) {
	setChecked(selected);
	if (selected) emit segmentSelected(this);
}

Segment::~Segment() {
}

void Segment::paintEvent(QPaintEvent *paintEvent) {
	QPainter painter(this);

	QColor textColor;

	if (isSelected()) {
		textColor = selectedTextColor();
		painter.fillRect(0, height() - selectedIndicatorHeight, width(), selectedIndicatorHeight, accentColor());
	} else if (isHighlighted()) {
		textColor = highlightedTextColor();
	} else if (isPressed()) {
		textColor = pressedTextColor();
	} else {
		textColor = this->textColor();
	}

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
	setMinimumSize(metrics.width(text()), metrics.height() + spacing + selectedIndicatorHeight);
}
