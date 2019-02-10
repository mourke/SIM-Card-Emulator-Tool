#ifndef SEGMENT_H
#define SEGMENT_H

#include <QAbstractButton>

class Segment : public QAbstractButton {
	Q_OBJECT

public:
	Segment(QWidget *parent);
	~Segment();
};

#endif // SEGMENT_H
