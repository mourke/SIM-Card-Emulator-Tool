#include "SegmentedControl.h"
#include <QtWidgets>
#include "Segment.h"

SegmentedControl::SegmentedControl(QWidget *parent) : QWidget(parent) {
	QHBoxLayout *layout = new QHBoxLayout();

	layout->setMargin(0);

	setLayout(layout);
}

SegmentedControl::~SegmentedControl() {
}

void SegmentedControl::setSelectedSegmentIndex(segmented_index_t selectedSegmentIndex) {
	if (!isSegmentIndexValid(selectedSegmentIndex)) return;
	m_selectedSegmentIndex = selectedSegmentIndex;
}

void SegmentedControl::addSegment(const QString &text) {
	insertSegment(segments().count(), text);
}

void SegmentedControl::addSegment(const QIcon &icon) {
	insertSegment(segments().count(), icon);
}

void SegmentedControl::insertSegment(segmented_index_t index, const QIcon &icon) {

}

void SegmentedControl::insertSegment(segmented_index_t index, const QString &text) {
	if (index < 0 || index > segments().count()) return; // Segment can only be inserted 1 past the end.
}

void SegmentedControl::removeSegment(segmented_index_t index) {
	if (!isSegmentIndexValid(index)) return;
}

void SegmentedControl::removeSegment(Segment *segment) {
	if (!m_segments.contains(segment)) return;
}

void SegmentedControl::removeAllSegments() {
	for (Segment *segment : segments()) {
		removeSegment(segment);
	}
	invalidateSize();
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

}

bool SegmentedControl::isSegmentIndexValid(segmented_index_t index) const {
	return index >= 0 && index < m_segments.count();
}
