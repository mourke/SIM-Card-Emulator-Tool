#ifndef APPLICATION_LAYER_DELEGATE
#define APPLICATION_LAYER_DELEGATE

#include <QStyledItemDelegate>
#include <optional>

class MainWindow;
class QStandardItem;

class ApplicationLayerDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
    ApplicationLayerDelegate(MainWindow *parent);

protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    int draw(std::optional<QPainter *> painter, const QStyleOptionViewItem &option, const QStandardItem *item) const;
};

#endif // APPLICATION_LAYER_DELEGATE
