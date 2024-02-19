#ifndef IIODEBUGINSTRUMENT_H
#define IIODEBUGINSTRUMENT_H

#include "scopy-iiodebugplugin_export.h"
#include "iiomodel.h"
#include "detailsview.h"
#include "searchbar.h"
#include "iiosortfilterproxymodel.h"
#include "watchlistview.h"

#include <iio.h>
#include <QWidget>
#include <QTreeView>

namespace scopy::iiodebugplugin {
class SCOPY_IIODEBUGPLUGIN_EXPORT IIODebugInstrument : public QWidget
{
	Q_OBJECT
public:
	IIODebugInstrument(struct iio_context *context, QString uri, QWidget *parent = nullptr);
	~IIODebugInstrument();

private Q_SLOTS:
	void applySelection(const QItemSelection &selected, const QItemSelection &deselected);
	void filterAndExpand(const QString &text);

private:
	void setupUi();
	void connectSignalsAndSlots();

	// Recursive function to find an item in the source model
	IIOStandardItem *findItemRecursive(QStandardItem *currentItem, QStandardItem *targetItem);
	void recursiveExpandItems(QStandardItem *item, const QString &text);
	void recursiveExpandItem(QStandardItem *item, QStandardItem *searchItem);

	struct iio_context *m_context;
	QString m_uri;
	QTreeView *m_treeView;
	IIOModel *m_iioModel;
	DetailsView *m_detailsView;
	SearchBar *m_searchBar;
	IIOSortFilterProxyModel *m_proxyModel;
	WatchListView *m_watchListView;
};
} // namespace scopy::iiodebugplugin
#endif // IIODEBUGINSTRUMENT_H
