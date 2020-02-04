#include "ApplicationLayerDelegate.h"
#include "Windows/MainWindow.h"
#include "Tracer/APDUCommand.h"
#include <QVariant>
#include <QStandardItemModel>
#include <QDebug>
#include <QPainter>
#include <QStyleOptionViewItem>
#include "Colors.h"

ApplicationLayerDelegate::ApplicationLayerDelegate(MainWindow *parent) : QStyledItemDelegate(parent) {

}

int draw(std::optional<QPainter *> painter, const QStyleOptionViewItem &option, const QStandardItem *item) {
	const auto applicationMap = item->data().toMap();

	static const QFont boldFont("Courier New", 11, QFont::Bold);
	static const QFont font("Courier New", 11, QFont::Normal);
	static const QFontMetrics boldMetrics = QFontMetrics(boldFont);
	static const QFontMetrics metrics = QFontMetrics(font);

	int x = option.rect.x() + 5;
	int y = option.rect.y() + boldMetrics.height() - 2;

	if (painter.has_value()) {
        auto p = *painter;
		p->fillRect(option.rect, Qt::white);
		p->setPen(headerColor());
		p->setFont(boldFont);
		p->drawText(x, y, item->text());
		p->setPen(Qt::black);
	}

	y += 5;

	for (auto i = applicationMap.begin(); i != applicationMap.end(); ++i) {
		QString key = i.key() + ":";
		int keyFlags = Qt::TextFlag::TextWordWrap | Qt::AlignmentFlag::AlignTop | Qt::AlignmentFlag::AlignRight; // when the value is multiple lines, align the key to the top.
		int keyX = x + 20;
		int keyWidth = 200;
		int keyHeight = boldMetrics.boundingRect(QRect(0, 0, keyWidth, 0), keyFlags, key).height();

		QString value = i.value().toString();
		int valueFlags = Qt::TextFlag::TextWrapAnywhere | Qt::AlignmentFlag::AlignBottom;
		int valueX = keyX + keyWidth + 3;
		int valueWidth = option.rect.width() - valueX - 10;
		int valueHeight = std::max(keyHeight, metrics.boundingRect(QRect(0, 0, valueWidth, 0), valueFlags, value).height());

		if (painter.has_value()) {
            auto p = *painter;
			p->setFont(boldFont);
			p->drawText(QRect(keyX, y, keyWidth, keyHeight), keyFlags, key);
			p->setFont(font);
			p->drawText(QRect(valueX, y, valueWidth, valueHeight), valueFlags, value);
		}

		y += valueHeight;
	}

	return y - option.rect.y();
}

void ApplicationLayerDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	auto model = qobject_cast<const QStandardItemModel *>(index.model());
	const auto item = model->itemFromIndex(index);
	
	draw(std::make_optional(painter), option, item);
}

QSize ApplicationLayerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
	auto model = qobject_cast<const QStandardItemModel *>(index.model());
	const auto item = model->itemFromIndex(index);
	int height = draw(std::nullopt, option, item);
	QSize hint = QSize(option.rect.width(), height);

	return hint;
}
