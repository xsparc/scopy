#ifndef SCOPY_WATCHLISTVIEW_H
#define SCOPY_WATCHLISTVIEW_H

#include <QWidget>
#include "iiostandarditem.h"

namespace scopy::iiodebugplugin {
class WatchListView : public QWidget
{
	Q_OBJECT
public:
	explicit WatchListView(QWidget *parent = nullptr);
	void setupUi();

public
	Q_SLOT : void addToWatchlist(IIOStandardItem *item);
	void removeFromWatchlist(IIOStandardItem *item);

Q_SIGNALS:
	void selectedItem(IIOStandardItem *item);
};
} // namespace scopy::iiodebugplugin

#endif // SCOPY_WATCHLISTVIEW_H
