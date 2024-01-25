#ifndef SCOPY_IIOSTANDARDITEM_H
#define SCOPY_IIOSTANDARDITEM_H

#include <QWidget>
#include <QStandardItem>
#include "iiostandarditemmodel.h"
#include "iiostandarditemtypes.h"
#include "iiostandarditemview.h"

#include <iio.h>
#include <iio-widgets/iiowidgetfactory.h>
#include <iio-widgets/iiowidget.h>

namespace scopy::iiodebugplugin {
class IIOStandardItem : public QStandardItem
{
public:
	explicit IIOStandardItem(QList<IIOWidget *> widgets, const QString& text);

	~IIOStandardItem() override;
	QList<IIOWidget *> getIIOWidgets();
	QStringList getDetails();

private:
	QList<IIOWidget *> m_iioWidgets;
	QStringList m_details;
};
} // namespace scopy::iiodebugplugin

#endif // SCOPY_IIOSTANDARDITEM_H
