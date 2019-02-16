#ifndef SEGMENT_H
#define SEGMENT_H

#include <QAbstractButton>

class Segment : public QAbstractButton {
	Q_OBJECT

	Q_PROPERTY(bool isPressed READ isPressed)
	Q_PROPERTY(bool isHighlighted READ isHighlighted)
	Q_PROPERTY(bool isSelected READ isSelected WRITE setSelected)

	Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
	Q_PROPERTY(QColor highlightedTextColor READ highlightedTextColor WRITE setHighlightedTextColor)
	Q_PROPERTY(QColor pressedTextColor READ pressedTextColor WRITE setPressedTextColor)
	Q_PROPERTY(QColor selectedTextColor READ selectedTextColor WRITE setSelectedTextColor)

	Q_PROPERTY(QFont textFont READ textFont WRITE  setTextFont)

public:
	Segment(QWidget *parent);
	~Segment();


	bool isPressed() const { return isDown(); }
	bool isHighlighted() const { return highlighted; }
	bool isSelected() const { return isChecked(); }
	

	QColor textColor() const { return m_textColor; }
	QColor highlightedTextColor() const { return m_highlightedTextColor; }
	QColor pressedTextColor() const { return m_pressedTextColor; }
	QColor selectedTextColor() const { return m_selectedTextColor; }

	void setTextColor(const QColor &color) { m_textColor = color; }	
	void setHighlightedTextColor(const QColor &color) { m_highlightedTextColor = color; }
	void setPressedTextColor(const QColor &color) { m_pressedTextColor = color; }
	void setSelectedTextColor(const QColor &color) { m_selectedTextColor = color; }

	QFont textFont() const { return m_textFont; }
	void setTextFont(const QFont &textFont) { m_textFont = textFont; }

	void sizeToFit();

Q_SIGNALS:
	void segmentSelected(Segment *);

public Q_SLOTS:
	void setSelected(const bool &selected);

protected:
	void paintEvent(QPaintEvent *paintEvent) override;
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void mousePressEvent(QMouseEvent *mouseEvent) override;

private:
	bool highlighted = false;
	QFont m_textFont = QFont("Segoe UI", 11);
	QColor m_textColor = QColor(0, 0, 0, 145);
	QColor m_highlightedTextColor, m_pressedTextColor, m_selectedTextColor = Qt::black;
	const int selectedIndicatorHeight = 2;

	using QAbstractButton::setCheckable;
	using QAbstractButton::isCheckable;
	using QAbstractButton::isChecked;
	using QAbstractButton::setDown;
	using QAbstractButton::isDown;
};

#endif // SEGMENT_H
