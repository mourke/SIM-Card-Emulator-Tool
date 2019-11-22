#ifndef APDU_LIST_WIDGET_ITEM_H
#define APDU_LIST_WIDGET_ITEM_H

#include <QListWidgetItem>

class APDUCommand;
class QLabel;
class QWidget;

class APDUListWidgetItem : public QListWidgetItem {

public:
	APDUListWidgetItem(const APDUCommand *command, QListWidget *parent = nullptr, ItemType type = Type);
	~APDUListWidgetItem();
	QWidget *widget() const { return m_widget; }
	const APDUCommand * command() const { return m_command; }

private:
	QWidget *m_widget;
	QLabel *headerLabel;
	const APDUCommand *m_command;
};

#endif // APDU_LIST_WIDGET_ITEM_H