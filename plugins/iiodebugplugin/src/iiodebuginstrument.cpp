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

	m_treeView = new QTreeView(bottom_container);
	m_iioModel = new IIOModel(m_context, m_treeView);
	//	auto *iio_filter_proxy = new IIOFilterProxy(tree_view);
	m_searchBar = new SearchBar(m_iioModel->getEntries(), this);
	m_detailsView = new DetailsView(details_container);

	StyleHelper::BackgroundPage(details_container, "DetailsContainer");
	StyleHelper::BackgroundPage(watch_list, "WatchListContainer");
	StyleHelper::BackgroundPage(tree_view_container, "TreeViewContainer");
	//	StyleHelper::BackgroundPage(search_bar_container, "SearchBarContainer");
	//	right_container->setStyleSheet("background-color: red");

	//	QObject::connect(search_bar->getLineEdit(), &QLineEdit::textChanged, [&](const QString &text) {
	//		iio_filter_proxy->setFilterPath(text);
	//	});
	//	iio_filter_proxy->setSourceModel(iio_model->getModel());

	tree_view_container->setMaximumWidth(400);
	m_treeView->setStyleSheet("color: white;");
	m_treeView->setModel(m_iioModel->getModel());
	QObject::connect(m_treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
			 [this](const QItemSelection &selected, const QItemSelection &deselected) {
				 // ugly asl? yes. does it work? also yes.
				 auto selectedIndex = selected.indexes()[0];
				 qWarning(CAT_IIODEBUGINSTRUMENT) << "received" << selectedIndex.data().toString();
				 qWarning(CAT_IIODEBUGINSTRUMENT) << "check" << m_iioModel->getModel()->index(0, 0);
				 auto modelItem = m_iioModel->getModel()->itemFromIndex(selectedIndex);
				 auto iioItem = dynamic_cast<IIOStandardItem *>(modelItem);

				 if(iioItem) {
					 qWarning(CAT_IIODEBUGINSTRUMENT) << "received 2";
					 m_detailsView->setIIOStandardItem(iioItem);
				 }
			 });

	m_treeView->expand(m_iioModel->getModel()->index(0, 0));

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
