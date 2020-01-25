#ifndef APPLICATION_LAYER_DELEGATE
#define APPLICATION_LAYER_DELEGATE

#include <QStyledItemDelegate >

class MainWindow;

class ApplicationLayerDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	ApplicationLayerDelegate(MainWindow *parent);
	~ApplicationLayerDelegate();

protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // APPLICATION_LAYER_DELEGATE
