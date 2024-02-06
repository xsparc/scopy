#ifndef SCOPY_DETAILSVIEW_H
#define SCOPY_DETAILSVIEW_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include "iiostandarditem.h"
#include <gui/subsection_separator.hpp>

namespace scopy::iiodebugplugin {
class DetailsView : public QWidget
{
	Q_OBJECT
public:
	explicit DetailsView(QWidget *parent);

	void setupUi();
	void setTitle(QString title);
	void setIIOStandardItem(IIOStandardItem *item);

Q_SIGNALS:
	void addToWatchlist(IIOStandardItem *item);
	void removeFromWatchlist(IIOStandardItem *item);

private:
	IIOStandardItem *m_currentItem;
	QLabel *m_titleLabel;
	QPushButton *m_watchlistButton;
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
