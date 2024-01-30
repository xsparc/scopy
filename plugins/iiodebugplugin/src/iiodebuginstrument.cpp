#include <QVBoxLayout>
#include <QLabel>
#include <QLoggingCategory>

#include <gui/stylehelper.h>

#include "iiodebuginstrument.h"
#include "iiostandarditem.h"

Q_LOGGING_CATEGORY(CAT_IIODEBUGINSTRUMENT, "IIODebugInstrument")
using namespace scopy::iiodebugplugin;

IIODebugInstrument::IIODebugInstrument(struct iio_context *context, QWidget *parent)
	: QWidget(parent)
	, m_context(context)
{
	setupUi();
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

	watch_list->setMaximumHeight(100);
	watch_list->layout()->addWidget(new QLabel("var 1"));
	watch_list->layout()->addWidget(new QLabel("var 2"));
	watch_list->layout()->addWidget(new QLabel("var 3"));

	m_proxyModel = new IIOSortFilterProxyModel(this);
	m_treeView = new QTreeView(bottom_container);
	m_iioModel = new IIOModel(m_context, m_treeView);
	m_searchBar = new SearchBar(m_iioModel->getEntries(), this);
	m_detailsView = new DetailsView(details_container);

	m_proxyModel->setSourceModel(m_iioModel->getModel());
	m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

	QObject::connect(m_searchBar->getLineEdit(), &QLineEdit::textChanged, this,
			 &IIODebugInstrument::filterAndExpand);

	StyleHelper::BackgroundPage(details_container, "DetailsContainer");
	StyleHelper::BackgroundPage(watch_list, "WatchListContainer");
	StyleHelper::BackgroundPage(tree_view_container, "TreeViewContainer");
	StyleHelper::BackgroundPage(search_bar_container, "SearchBarContainer");

	tree_view_container->setMaximumWidth(400);
	m_treeView->setStyleSheet("color: white;");
	m_treeView->setModel(m_proxyModel);
	QObject::connect(m_treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
			 &IIODebugInstrument::applySelection);

	// expand the root element, better visual experience
	m_treeView->expand(m_proxyModel->index(0, 0));

	details_container->layout()->addWidget(m_detailsView);
	tree_view_container->layout()->addWidget(m_treeView);
	search_bar_container->layout()->addWidget(m_searchBar);

	layout()->addWidget(search_bar_container);
	layout()->addWidget(bottom_container);
	bottom_container->layout()->addWidget(tree_view_container);
	bottom_container->layout()->addWidget(right_container);
	right_container->layout()->addWidget(details_container);
	right_container->layout()->addWidget(watch_list);
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
			m_treeView->setExpanded(m_proxyModel->mapFromSource(childItem->index()).parent(), true);
		}

		// Recursively process children
		recursiveExpandItems(childItem, text);
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

	if (text.isEmpty()) {
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
