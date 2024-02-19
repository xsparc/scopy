#ifndef SCOPY_IIOSTANDARDITEM_H
#define SCOPY_IIOSTANDARDITEM_H

#include <QWidget>
#include <QStandardItem>

#include <iio.h>
#include <iio-widgets/iiowidgetfactory.h>
#include <iio-widgets/iiowidget.h>

namespace scopy::iiodebugplugin {
class IIOStandardItem : public QStandardItem
{
public:
	explicit IIOStandardItem(QList<IIOWidget *> widgets, const QString &title, const QString &fullTitle);

	~IIOStandardItem() override;
	QList<IIOWidget *> getIIOWidgets();

	QStringList details();
	QString title();
	QString fullTitle();

	bool isWatched();
	void setWatched(bool isWatched);

private:
	QList<IIOWidget *> m_iioWidgets;
	QStringList m_details;
	QString m_title;
	QString m_fullTitle;
	bool m_isWatched;
};
} // namespace scopy::iiodebugplugin

#endif // SCOPY_IIOSTANDARDITEM_H
