#ifndef SEGMENTEDCONTROL_H
#define SEGMENTEDCONTROL_H

#include <QWidget>

class Segment;

typedef unsigned long long segmented_index_t;

/** A horizontal control made of multiple segments, 
  * each segment functioning as a discrete button.
  *
  * @b Description
  * A segmented control can display a title or an
  * image. The SegmentedControl object automatically 
  * resizes segments to fit proportionally within their 
  * superview unless they have a specific width set. 
  *
  */
class SegmentedControl : public QWidget {
	Q_OBJECT

	Q_PROPERTY(QList<Segment *> segments READ segments)
	Q_PROPERTY(segmented_index_t selectedSegmentIndex READ selectedSegmentIndex WRITE setSelectedSegmentIndex)

public:
	SegmentedControl(QWidget *parent);
	~SegmentedControl();

	/**
	 * The segments currently in the segmented control.
	 *
	 * Segments can be added to this array by calling 
	 * "addSegment" or "insertSegment".
	 *
	 * @retval The segments in the control.
	 */
	const QList<Segment *>& segments() const { return m_segments; }

	/**
	 * The index number identifying the selected segment
	 * (that is, the last segment clicked).
	 *
	 * @retval The index of the segment in the "segments()" array that 
	 * is currently selected.
	 */
	segmented_index_t selectedSegmentIndex() const { return m_selectedSegmentIndex;  }


	/**
	 * Adds a segment to the end of segments array with a given title.
	 *
	 * @param text	The text of the new segment.
	 */
	void addSegment(const QString &text);

	/**
	 * Inserts a segment at a specific position in the receiver and 
	 * gives it a title as content. If the index is smaller than the
	 * segment count, it will be inserted at that index and the subsequent 
	 * elements moved forward one place. If the index is equal to the count
	 * the segment will be appended to the end of the array.
	 *
	 * @b Note
	 * If the index is greater than the count an out-of-range exception
	 * will be thrown.
	 *
	 * @param index	The index of the new segment
	 * @param text	The text of the new segment.
	 */
	void insertSegment(segmented_index_t index, const QString &text);

	/**
	 * Adds a segment to the end of segments array with a given icon.
	 *
	 * @param icon	The icon of the segment.
	 */
	void addSegment(const QIcon &icon);

	/**
	 * Inserts a segment at a specific position in the receiver and
	 * gives it an image as content. If the index is smaller than the
	 * segment count, it will be inserted at that index and the subsequent
	 * elements moved forward one place. If the index is equal to the count
	 * the segment will be appended to the end of the array.
	 *
	 * @b Note
	 * If the index is greater than the amount of segments an out-of-range 
	 * exception will be thrown.
	 *
	 * @param index	The index of the new segment.
	 * @param icon	The icon of the segment.
	 */
	void insertSegment(segmented_index_t index, const QIcon &icon);

	/**
	 * Removes a segment at a certain index from the control.
	 *
	 * @b Note
	 * If the index is greater than the amount of segments an out-of-range 
	 * exception will be thrown.
	 *
	 * @param index	The index of the segment to be removed.
	 */
	void removeSegment(segmented_index_t index);

	/**
	 * Removes a segment from the control.
	 *
	 * @b Note
	 * If the index is greater than the amount of segments an out-of-range
	 * exception will be thrown.
	 *
	 * @param segment	The segment to be removed.
	 */
	void removeSegment(Segment *segment);

	/**
	 * Removes all the segments from the control.
	 */
	void removeAllSegments();


	/**
	 * Sets the width of a specified segment
	 *
	 * @b Note
	 * If the index is greater than the amount of segments an out-of-range
	 * exception will be thrown.
	 *
	 * @param width	The overriding width of the segment
	 * @param index	The index of the segment whose width is to be changed.
	 */
	void setSegmentWidth(int width, segmented_index_t index);

	/**
	 * The width of a specified segment. This is automatically set to the
	 * segment's text or icon size but can be overridden by the method
	 * "setSegmentWidth".
	 *
	 * @retval The width of the segment.
	 */
    int widthForSegment(segmented_index_t index) const;
signals:
	/**
	 * Emitted when the selected segment index has been changed.
	 *
	 * @param newIndex	The new index that has been selected.
	 */
	void selectedSegmentIndexChanged(segmented_index_t newIndex);

public slots:
	/**
	 * Sets the selected status of the specified segment. Setting this
	 * deselects the previously selected segment.
	 *
	 * @b Note
	 * If the index is greater than the amount of segments an out-of-range
	 * exception will be thrown.
	 *
	 * @param selectedSegmentIndex	The index of the item to be selected.
	 */
	void setSelectedSegmentIndex(segmented_index_t selectedSegmentIndex);

	/**
	 * Sets the selected status of the specified segment. Setting this
	 * deselects the previously selected segment.
	 *
	 * @b Note
	 * If the segment is not in the segmented control, an out-of-range exception
	 * will be thrown.
	 *
	 * @param segment	The segment to be selected.
	 */
    void selectSegment(Segment *segment) { setSelectedSegmentIndex(static_cast<segmented_index_t>(m_segments.indexOf(segment))); }

	/**
	 * Sets the selected status of the specified segment. Setting this
	 * deselects the previously selected segment.
	 *
	 * @b Note
	 * If the index is greater than the amount of segments an out-of-range
	 * exception will be thrown.
	 *
	 * @param index	The index of the item to be selected.
	 */
	void selectSegment(segmented_index_t index) { setSelectedSegmentIndex(index); }

protected:
	QList<Segment *> m_segments;
	segmented_index_t m_selectedSegmentIndex = 0;
	void invalidateSize();

private:
	bool isSegmentIndexValid(segmented_index_t index) const;
    void insertSegment(segmented_index_t index, const QVariant &value);
};

#endif // SEGMENTEDCONTROL_H
