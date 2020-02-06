#include "SegmentedControl.h"
#include <QtWidgets>
#include "Segment.h"

SegmentedControl::SegmentedControl(QWidget *parent) : QWidget(parent) {
	QHBoxLayout *layout = new QHBoxLayout();

	layout->setContentsMargins(10, 10, 10, 2);
	layout->setSpacing(24);
	
	setLayout(layout);
}

SegmentedControl::~SegmentedControl() {
	removeAllSegments();
}

void SegmentedControl::setSelectedSegmentIndex(segmented_index_t selectedSegmentIndex) {
	if (!isSegmentIndexValid(selectedSegmentIndex) || selectedSegmentIndex == m_selectedSegmentIndex) return;
	m_segments[m_selectedSegmentIndex]->setSelected(false);
	m_selectedSegmentIndex = selectedSegmentIndex;
	m_segments[selectedSegmentIndex]->setSelected(true);
	emit selectedSegmentIndexChanged(selectedSegmentIndex);
}

void SegmentedControl::addSegment(const QString &text) {
	insertSegment(m_segments.count(), text);
}

void SegmentedControl::addSegment(const QIcon &icon) {
	insertSegment(m_segments.count(), icon);
}

void SegmentedControl::insertSegment(segmented_index_t index, const QIcon &icon) {
	insertSegment(index, QVariant(icon));
}

void SegmentedControl::insertSegment(segmented_index_t index, const QString &text) {
	insertSegment(index, QVariant(text));
}

void SegmentedControl::insertSegment(segmented_index_t index, const QVariant &value) {
    if (index > static_cast<segmented_index_t>(m_segments.count())) throw std::out_of_range("Segments can only be added one from the end of the array.");
	
	Segment *segment = new Segment(this);

	QObject::connect(segment, &Segment::selected, this, [this, segment] {
        auto index = static_cast<segmented_index_t>(this->m_segments.indexOf(segment));
		this->setSelectedSegmentIndex(index);
	});

	if (value.type() == QVariant::Type::Icon) {
		QIcon icon = value.value<QIcon>();
		segment->setIcon(icon);
	} else if (value.type() == QVariant::Type::String) {
		QString text = value.toString();
		segment->setText(text);
		segment->setFont(font());
	}

	segment->sizeToFit();

    if (index == static_cast<segmented_index_t>(m_segments.count())) {
		m_segments.append(segment);
		layout()->addWidget(segment);
	} else {
        for (auto i = static_cast<segmented_index_t>(m_segments.count()); i > index; --i) {
            if (i == static_cast<segmented_index_t>(m_segments.count())) {
				m_segments.append(segment);
				continue;
			}
			m_segments[i] = m_segments[i - 1];
		}
		m_segments[index] = segment;
		invalidateSize();
	}

	segment->setSelected(m_selectedSegmentIndex == index);
}

void SegmentedControl::removeSegment(segmented_index_t index) {
	if (!isSegmentIndexValid(index)) return;
	Segment *segment = m_segments[index];
	removeSegment(segment);
}

void SegmentedControl::removeSegment(Segment *segment) {
	if (!m_segments.contains(segment)) return;

    auto index = static_cast<segmented_index_t>(m_segments.indexOf(segment));
	if (index == m_selectedSegmentIndex) {
		setSelectedSegmentIndex(0);
	}

	m_segments.removeOne(segment);
	layout()->removeWidget(segment);
	segment->disconnect();
    delete segment;
}

void SegmentedControl::removeAllSegments() {
	for (Segment *segment : m_segments) {
		removeSegment(segment);
	}
}

void SegmentedControl::setSegmentWidth(int width, segmented_index_t index) {
	if (!isSegmentIndexValid(index)) return;
	m_segments[index]->setMinimumWidth(width);
}

int SegmentedControl::widthForSegment(segmented_index_t index) const {
	if (!isSegmentIndexValid(index)) return 0;
	return m_segments[index]->geometry().width();
}

void SegmentedControl::invalidateSize() {
	for (int i = 0; i < layout()->count(); ++i) {
		QWidget *widget = layout()->itemAt(i)->widget();
		if (widget != Q_NULLPTR) {
			layout()->removeWidget(widget);
		}
	}

	for (Segment *segment : m_segments) {
		layout()->addWidget(segment);
	}
}

bool SegmentedControl::isSegmentIndexValid(segmented_index_t index) const {
    if (index < static_cast<segmented_index_t>(m_segments.count())) {
		return true;
	} else {
		throw std::out_of_range("The segment index passed was out of range.");
	}
}
