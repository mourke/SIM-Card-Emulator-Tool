#include "APDUListWidgetItem.h"
#include <QWidget>
#include <QLabel>
#include <QLayout>
#include "Tracer/APDUCommand.h"
#include "Colors.h"

APDUListWidgetItem::APDUListWidgetItem(const APDUCommand *command, QListWidget *parent, ItemType type) : QListWidgetItem(parent, type), m_command(command) {
	if (!command->applicationMap().has_value()) {
		throw std::invalid_argument("APDUCommand must have a non-empty application map to be added to an APDUListWidgetItem");
	}
	QMap<QString, QString> applicationMap = command->applicationMap().value();
	
	m_widget = new QWidget();
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setSpacing(0); // spacing between header label and each item 
	layout->setContentsMargins(5, 2, 5, 2); // margin between the header label and the widget container
	headerLabel = new QLabel(command->instructionCodeString(), m_widget);
	QSizePolicy policy = headerLabel->sizePolicy();
	policy.setVerticalPolicy(QSizePolicy::Policy::Fixed);
	headerLabel->setSizePolicy(policy);
	headerLabel->setFont(QFont("Courier New", 11, QFont::Bold));
	headerLabel->setStyleSheet(QString("QLabel { color : %1; }").arg(headerColor().name(QColor::HexArgb)));
	layout->addWidget(headerLabel);
	for (auto i = applicationMap.begin(); i != applicationMap.end(); ++i) {
		QWidget *widget = new QWidget(m_widget);
		QLabel *keyLabel = new QLabel(i.key() + ":", widget);
		keyLabel->setWordWrap(true);
		keyLabel->setFont(QFont("Courier New", 11, QFont::Bold));
		keyLabel->setAlignment(Qt::AlignmentFlag::AlignTop | Qt::AlignmentFlag::AlignRight); // when the value is multiple lines, align the key to the top.
		keyLabel->setMinimumWidth(200);
		keyLabel->setMaximumWidth(200);
		QLabel *valueLabel = new QLabel(i.value(), widget);
		valueLabel->setAlignment(Qt::AlignmentFlag::AlignBottom); // when the key is multiple lines, align the value to the bottom as that is where the semi colon will appear.
		valueLabel->setFont(QFont("Courier New", 11, QFont::Normal));
		QHBoxLayout *widgetLayout = new QHBoxLayout();
		widgetLayout->setContentsMargins(20, 0, 10, 0);
		widgetLayout->addWidget(keyLabel);
		widgetLayout->addWidget(valueLabel);
		widget->setLayout(widgetLayout);
		layout->addWidget(widget);
	}
	m_widget->setLayout(layout);
	int parentWidth = parent->size().width();
	QSize sizeHint = QSize(parentWidth, m_widget->heightForWidth(parentWidth));
	this->setSizeHint(sizeHint);
}

APDUListWidgetItem::~APDUListWidgetItem() {
	QLayoutItem *child;
	auto layout = m_widget->layout();
	while ((child = layout->takeAt(0)) != nullptr) {
		delete child->widget();
		delete child;
	}
	delete layout;
}