#ifndef SEGMENTEDCONTROL_H
#define SEGMENTEDCONTROL_H

#include <QWidget>

class Segment;

typedef unsigned long long segmented_index_t;

class SegmentedControl : public QWidget {
	Q_OBJECT

	Q_PROPERTY(QList<Segment *> segments READ segments)
	Q_PROPERTY(segmented_index_t selectedSegmentIndex READ selectedSegmentIndex WRITE setSelectedSegmentIndex)

public:
	SegmentedControl(QWidget *parent);
	~SegmentedControl();

	const QList<Segment *>& segments() const { return m_segments; }

	segmented_index_t selectedSegmentIndex() const { return m_selectedSegmentIndex;  }
	void setSelectedSegmentIndex(segmented_index_t selectedSegmentIndex);
	void selectSegment(segmented_index_t index) { setSelectedSegmentIndex(index); }

	void addSegment(const QString &text);
	void insertSegment(segmented_index_t index, const QString &text);
	void addSegment(const QIcon &icon);
	void insertSegment(segmented_index_t index, const QIcon &icon);
	void removeSegment(segmented_index_t index);
	void removeSegment(Segment *segment);
	void removeAllSegments();

	void setSegmentWidth(int width, segmented_index_t index);
	int widthForSegment(segmented_index_t index) const;

protected:
	QList<Segment *> m_segments;
	segmented_index_t m_selectedSegmentIndex;
	segmented_index_t highlightedSegmentIndex;
	segmented_index_t pressedSegmentIndex;
	void invalidateSize();

private:
	bool isSegmentIndexValid(segmented_index_t index) const;
};

#endif // SEGMENTEDCONTROL_H
