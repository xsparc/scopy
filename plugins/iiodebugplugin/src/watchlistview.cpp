#include "watchlistview.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLoggingCategory>

using namespace scopy::iiodebugplugin;

WatchListView::WatchListView(QWidget *parent)
	: QWidget(parent)
{
	setupUi();
}

void WatchListView::setupUi() { setLayout(new QVBoxLayout(this)); }

void WatchListView::addToWatchlist(IIOStandardItem *item)
{
	auto pushButton = new QPushButton(item->getTitle());
	QObject::connect(pushButton, &QPushButton::clicked, this, [this, item]() {
		qWarning() << "selected item from watchlist";
		Q_EMIT selectedItem(item);
	});
	layout()->addWidget(pushButton);
}

void WatchListView::removeFromWatchlist(IIOStandardItem *item)
{
	auto count = layout()->count();
	qWarning() << "watchlist item count is" << count;
	for(int i = 0; i < count; ++i) {
		QLayoutItem *currentItem = layout()->itemAt(i);
		QWidget *currentWidget = currentItem->widget();
		auto currentButton = dynamic_cast<QPushButton *>(currentWidget);
		if(!currentButton) {
			continue;
		}

		if(currentButton->text() == item->getTitle()) {
			layout()->removeItem(currentItem);
			currentButton->deleteLater();
			return;
		}
	}
}
