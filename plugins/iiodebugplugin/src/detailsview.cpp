#include "detailsview.h"
#include <QVBoxLayout>

using namespace scopy::iiodebugplugin;

DetailsView::DetailsView(QWidget *parent)
	: QWidget(parent)
	, m_titleLabel(new QLabel("Select an IIO item.", this))
	, m_watchlistButton(new QPushButton("Add to watchlist", this))
	, m_attrSeparator(new gui::SubsectionSeparator(this))
	, m_detailsSeparator(new gui::SubsectionSeparator(this))
	, m_scrollArea(new QScrollArea(this))
	, m_scrollAreaContents(new QWidget(this))
{
	setupUi();
}

void DetailsView::setupUi()
{
	setLayout(new QVBoxLayout(this));

	m_titleLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	m_titleLabel->setStyleSheet("color: white;");
	m_titleLabel->setAlignment(Qt::AlignCenter);

	m_scrollAreaContents->setLayout(new QVBoxLayout(m_scrollAreaContents));
	m_scrollAreaContents->setObjectName("DetailsViewScrollAreaContents");
	m_scrollAreaContents->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	m_scrollArea->setWidget(m_scrollAreaContents);
	m_scrollArea->setWidgetResizable(true);

	m_attrSeparator->setLabel("Attributes");
	m_attrSeparator->getContentWidget()->layout()->addWidget(m_scrollArea);
	m_attrSeparator->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

	m_detailsSeparator->setLabel("Device info");
	m_detailsSeparator->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	QObject::connect(m_watchlistButton, &QPushButton::clicked, this, [this]() {
		if(m_currentItem->isWatched()) {
			Q_EMIT removeFromWatchlist(m_currentItem);
			m_watchlistButton->setText("Add to watchlist");
			m_currentItem->setWatched(false);
		} else {
			Q_EMIT addToWatchlist(m_currentItem); // this button might move or change
			m_watchlistButton->setText("Remove from watchlist");
			m_currentItem->setWatched(true);
		}
	});

	layout()->addWidget(m_titleLabel);
	layout()->addWidget(m_watchlistButton);
	layout()->addWidget(m_attrSeparator);
	layout()->addWidget(m_detailsSeparator);
}

void DetailsView::setTitle(QString title)
{
	int dataIndex = title.indexOf('(');
	if(dataIndex != -1) {
		title.remove(dataIndex, title.length() - dataIndex);
	}
	m_titleLabel->setText(title);
}

void DetailsView::setIIOStandardItem(IIOStandardItem *item)
{
	m_currentItem = item;
	// FIXME: this whole system of displaying stuff is kind of wrong, it is supposed to just demonstrate how this
	// looks
	setTitle(item->fullTitle());
	clearWidgets();
	auto iioWidgets = item->getIIOWidgets();
	for(auto widget : iioWidgets) {
		widget->show();
		m_currentWidgets.append(widget);
		m_scrollAreaContents->layout()->addWidget(widget);
	}

	auto details = item->details();
	for(const auto &detail : details) {
		auto label = new QLabel(detail);
		label->show();
		m_detailsList.append(label);
		m_detailsSeparator->getContentWidget()->layout()->addWidget(label);
	}

	if(item->isWatched()) {
		m_watchlistButton->setText("Remove from watchlist");
	} else {
		m_watchlistButton->setText("Add to watchlist");
	}
}

void DetailsView::clearWidgets()
{
	QLayoutItem *child;
	while((child = m_scrollAreaContents->layout()->takeAt(0)) != nullptr) {
		child->widget()->hide();
		m_scrollAreaContents->layout()->removeWidget(child->widget());
	}
	m_currentWidgets.clear();

	while((child = m_detailsSeparator->getContentWidget()->layout()->takeAt(0)) != nullptr) {
		child->widget()->hide();
		m_detailsSeparator->getContentWidget()->layout()->removeWidget(child->widget());
	}
	m_detailsList.clear();
}
