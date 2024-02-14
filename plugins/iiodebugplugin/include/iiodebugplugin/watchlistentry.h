#ifndef WATCHLISTENTRY_H
#define WATCHLISTENTRY_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include "iiostandarditem.h"

namespace scopy::iiodebugplugin {
class WatchListEntry : public QPushButton
{
	Q_OBJECT
public:
	explicit WatchListEntry(IIOStandardItem *item, QWidget *parent = nullptr);

	QString getName();
	void setName(QString name);

	QString getValue();
	void setValue(QString value);

private Q_SLOTS:
	void setNewData(QString data, QString optionalData);

private:
	void setupUi();

	IIOStandardItem *m_item;
	QString m_name;
	QString m_value;
};
} // namespace scopy::iiodebugplugin

#endif // WATCHLISTENTRY_H
