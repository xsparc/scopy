#ifndef SCOPY_DETAILSVIEW_H
#define SCOPY_DETAILSVIEW_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include "iiostandarditem.h"
#include <gui/subsection_separator.hpp>

namespace scopy::iiodebugplugin {
class DetailsView : public QWidget
{
	Q_OBJECT
public:
	explicit DetailsView(QWidget *parent);

	void setTitle(QString title);
	void setIIOStandardItem(IIOStandardItem *item);

private:
	QLabel *m_titleLabel;
	gui::SubsectionSeparator *m_attrSeparator;
	gui::SubsectionSeparator *m_detailsSeparator;
	QScrollArea *m_scrollArea;
	QWidget *m_scrollAreaContents;
	QList<IIOWidget *> m_currentWidgets;
	QList<QLabel *> m_detailsList;

	void clearWidgets();
};
} // namespace scopy::iiodebugplugin

#endif // SCOPY_DETAILSVIEW_H
