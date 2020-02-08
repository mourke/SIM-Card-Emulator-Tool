#include "Segment.h"
#include <QtWidgets>
#include "Colors.h"

Segment::Segment(QWidget *parent) : QAbstractButton(parent), m_selectedIndicatorColor(accentColor()) {
	setCheckable(true);
	QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(setSelected(bool)));
}

void Segment::setSelected(const bool &selected) {
	setChecked(selected);
	if (selected) emit this->selected();
}

void Segment::paintEvent(QPaintEvent *paintEvent) {
	QPainter painter(this);

    QColor textColor;

	if (isSelected()) {
        textColor = selectedTextColor();
		painter.fillRect(0, height() - SELECTED_INDICATOR_HEIGHT, width(), SELECTED_INDICATOR_HEIGHT, m_selectedIndicatorColor);
	} else if (isHighlighted()) {
		textColor = highlightedTextColor();
    } else if (isPressed()) {
		textColor = pressedTextColor();
    } else {
		textColor = this->textColor();
    }

	painter.setFont(font());

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
	QFontMetrics metrics(font());
    int height = metrics.height() + spacing + SELECTED_INDICATOR_HEIGHT;
    setMinimumSize(metrics.horizontalAdvance(text()) + 1, height);
}
