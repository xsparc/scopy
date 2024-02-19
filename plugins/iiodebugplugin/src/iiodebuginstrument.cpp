#include <QVBoxLayout>
#include <QLabel>
#include <QLoggingCategory>
#include <QSplitter>

#include <gui/stylehelper.h>

#include "iiodebuginstrument.h"
#include "iiostandarditem.h"

Q_LOGGING_CATEGORY(CAT_IIODEBUGINSTRUMENT, "IIODebugInstrument")
using namespace scopy::iiodebugplugin;

IIODebugInstrument::IIODebugInstrument(struct iio_context *context, QString uri, QWidget *parent)
	: QWidget(parent)
	, m_context(context)
	, m_uri(uri)
{
	setupUi();
	connectSignalsAndSlots();
}

IIODebugInstrument::~IIODebugInstrument() {}

void IIODebugInstrument::setupUi()
{
	auto *search_bar_container = new QWidget(this);
	auto *bottom_container = new QWidget(this);
	auto *tree_view_container = new QWidget(bottom_container);
	auto *right_container = new QWidget(bottom_container);
	auto *details_container = new QWidget(right_container);
	auto *watch_list = new QWidget(right_container);
	auto *vSplitter = new QSplitter(Qt::Vertical, this);
	auto *hSplitter = new QSplitter(Qt::Horizontal, this);

	setLayout(new QVBoxLayout(this));
	bottom_container->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	bottom_container->setLayout(new QHBoxLayout(bottom_container));
	bottom_container->layout()->setContentsMargins(0, 0, 0, 0);

	right_container->setLayout(new QVBoxLayout(right_container));
	right_container->layout()->setContentsMargins(0, 0, 0, 0);
	details_container->setLayout(new QVBoxLayout(details_container));
	watch_list->setLayout(new QVBoxLayout(watch_list));
	tree_view_container->setLayout(new QVBoxLayout(tree_view_container));
	search_bar_container->setLayout(new QVBoxLayout(search_bar_container));
	search_bar_container->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	vSplitter->setSizes({150, 300});
	hSplitter->setSizes({150, 300});

	m_proxyModel = new IIOSortFilterProxyModel(this);
	m_treeView = new QTreeView(bottom_container);
	m_iioModel = new IIOModel(m_context, m_uri, m_treeView);
	m_searchBar = new SearchBar(m_iioModel->getEntries(), this);
	m_detailsView = new DetailsView(details_container);
	m_watchListView = new WatchListView(watch_list);

	// watch_list->setFixedHeight(100);
	watch_list->layout()->setContentsMargins(0, 0, 0, 0);
	watch_list->layout()->addWidget(m_watchListView);

	m_proxyModel->setSourceModel(m_iioModel->getModel());
	m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

	StyleHelper::BackgroundPage(details_container, "DetailsContainer");
	StyleHelper::BackgroundPage(watch_list, "WatchListContainer");
	StyleHelper::BackgroundPage(tree_view_container, "TreeViewContainer");
	StyleHelper::BackgroundPage(search_bar_container, "SearchBarContainer");

	// tree_view_container->setMaximumWidth(400);
	m_treeView->setStyleSheet("color: white;");
	m_treeView->setModel(m_proxyModel);

	// expand the root element, better visual experience
	m_treeView->expand(m_proxyModel->index(0, 0));

	details_container->layout()->addWidget(m_detailsView);
	tree_view_container->layout()->addWidget(m_treeView);
	search_bar_container->layout()->addWidget(m_searchBar);

	layout()->addWidget(search_bar_container);
	layout()->addWidget(bottom_container);
	hSplitter->addWidget(tree_view_container);
	hSplitter->addWidget(right_container);
	bottom_container->layout()->addWidget(hSplitter);
	vSplitter->addWidget(details_container);
	vSplitter->addWidget(watch_list);
	right_container->layout()->addWidget(vSplitter);

	tree_view_container->resize(300, tree_view_container->sizeHint().height());
}

void IIODebugInstrument::connectSignalsAndSlots()
{
	QObject::connect(m_searchBar->getLineEdit(), &QLineEdit::textChanged, this, [this](QString text) {
		// TODO: when the user clears the search bar, it should only open the context item, everything should be
		// collapsed
		filterAndExpand(text);
		if(text.isEmpty()) {
			m_treeView->expand(m_proxyModel->index(0, 0));
		}
	});

	QObject::connect(m_treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
			 &IIODebugInstrument::applySelection);

	QObject::connect(m_detailsView, &DetailsView::addToWatchlist, this, [this](IIOStandardItem *item) {
		qWarning(CAT_IIODEBUGINSTRUMENT) << "sending item to watchlist";
		m_watchListView->addToWatchlist(item);
	});

	QObject::connect(m_detailsView, &DetailsView::removeFromWatchlist, this, [this](IIOStandardItem *item) {
		qWarning(CAT_IIODEBUGINSTRUMENT) << "removing item from watchlist";
		m_watchListView->removeFromWatchlist(item);
	});

	QObject::connect(m_watchListView, &WatchListView::selectedItem, this, [this](IIOStandardItem *item) {
		qWarning(CAT_IIODEBUGINSTRUMENT) << "Expanding item" << item->title();
		// TODO: set the item selection for this
		// TODO: add style to the watchlist infrastructure
		auto sourceModel = qobject_cast<QStandardItemModel *>(m_proxyModel->sourceModel());
		recursiveExpandItem(sourceModel->invisibleRootItem(), item);
		m_detailsView->setIIOStandardItem(item);
	});
}

IIOStandardItem *IIODebugInstrument::findItemRecursive(QStandardItem *currentItem, QStandardItem *targetItem)
{
	if(currentItem == targetItem) {
		return dynamic_cast<IIOStandardItem *>(currentItem);
	}

	// Check children recursively
	for(int i = 0; i < currentItem->rowCount(); ++i) {
		IIOStandardItem *result = findItemRecursive(currentItem->child(i), targetItem);
		if(result) {
			return result;
		}
	}

	return nullptr;
}

void IIODebugInstrument::recursiveExpandItems(QStandardItem *item, const QString &text)
{
	for(int row = 0; row < item->rowCount(); ++row) {
		QStandardItem *childItem = item->child(row);

		// Check if the item's data contains the filter string
		if(childItem && childItem->text().contains(text, Qt::CaseInsensitive)) {
			QModelIndex proxyIndex = m_proxyModel->mapFromSource(childItem->index());

			// Expand all parents from the root to the current element
			QModelIndex parent = proxyIndex.parent();
			while(parent.isValid()) {
				m_treeView->expand(parent);
				parent = parent.parent();
			}
		}

		// Recursively process children
		recursiveExpandItems(childItem, text);
	}
}

void IIODebugInstrument::recursiveExpandItem(QStandardItem *item, QStandardItem *searchItem)
{
	for(int row = 0; row < item->rowCount(); ++row) {
		QStandardItem *childItem = item->child(row);

		if(childItem == searchItem) {
			QModelIndex index = m_proxyModel->mapFromSource(childItem->index());
			QModelIndex parent = index.parent();

			// Recursively expand all parents once the item was found
			while(parent.isValid()) {
				m_treeView->expand(parent);
				parent = parent.parent();
			}

			// Highlight the selected item
			QItemSelectionModel *selectionModel = m_treeView->selectionModel();
			selectionModel->select(index, QItemSelectionModel::ClearAndSelect);
			return;
		}

		recursiveExpandItem(childItem, searchItem);
	}
}

void IIODebugInstrument::applySelection(const QItemSelection &selected, const QItemSelection &deselected)
{
	Q_UNUSED(deselected)

	if(selected.indexes().isEmpty()) {
		qWarning(CAT_IIODEBUGINSTRUMENT) << "Selected index is cannot be found.";
		return;
	}

	QModelIndex proxyIndex = selected.indexes().first();

	// Map the proxy index to the source index
	QModelIndex sourceIndex = m_proxyModel->mapToSource(proxyIndex);

	// Get the source model item from the source index
	QStandardItemModel *sourceModel = qobject_cast<QStandardItemModel *>(m_proxyModel->sourceModel());
	QStandardItem *modelItem = sourceModel->itemFromIndex(sourceIndex);

	// Find the root item from the source model
	QStandardItem *rootItem = sourceModel->invisibleRootItem();

	// Use a recursive function to find the corresponding item in the source model
	IIOStandardItem *iioItem = findItemRecursive(rootItem, modelItem);

	if(iioItem) {
		m_detailsView->setIIOStandardItem(iioItem);
	}
}

void IIODebugInstrument::filterAndExpand(const QString &text)
{
	m_proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive, QRegExp::FixedString));
	m_proxyModel->invalidate(); // Trigger re-filtering

	if(text.isEmpty()) {
		qDebug(CAT_IIODEBUGINSTRUMENT) << "Text is empty, will not recursively expand items.";
		return;
	}

	QStandardItemModel *sourceModel = qobject_cast<QStandardItemModel *>(m_proxyModel->sourceModel());

	if(!sourceModel) {
		qWarning(CAT_IIODEBUGINSTRUMENT) << "Failed to obtain source model.";
		return;
	}

	// Iterate through the items in the source model
	recursiveExpandItems(sourceModel->invisibleRootItem(), text);
}
