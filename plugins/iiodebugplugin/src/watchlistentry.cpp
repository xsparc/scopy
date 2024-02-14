#include "watchlistentry.h"
#include <iio-widgets/datastrategy/datastrategyinterface.h>
#include <QHBoxLayout>

using namespace scopy::iiodebugplugin;

WatchListEntry::WatchListEntry(IIOStandardItem *item, QWidget *parent)
	: QPushButton(parent)
	, m_item(item)
{
	setupUi();
	m_name = item->getTitle();

	auto widgets = item->getIIOWidgets();
	qWarning() << "size: " << widgets.size();
	if(widgets.size() == 1) {
		auto widget = widgets[0];
		m_value = widget->getDataStrategy()->data();
		setText(m_name + " (" + m_value + ")");
		QObject::connect(dynamic_cast<QWidget *>(widget->getDataStrategy()), SIGNAL(sendData(QString, QString)),
				 this, SLOT(setNewData(QString, QString)));
	} else {
		m_value = "";
		setText(m_name);
	}
}

QString WatchListEntry::getName() { return m_name; }

void WatchListEntry::setName(QString name) { m_name = name; }

QString WatchListEntry::getValue() { return m_value; }

void WatchListEntry::setValue(QString value) { m_value = value; }

void WatchListEntry::setNewData(QString data, QString optionalData)
{
	Q_UNUSED(optionalData)

	qWarning() << data;
	m_value = data;
	setText(m_name + " (" + m_value + ")");
}

void WatchListEntry::setupUi()
{
	// implement if needed
}
