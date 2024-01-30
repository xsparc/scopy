#include "detailsview.h"
#include <QVBoxLayout>

using namespace scopy::iiodebugplugin;

DetailsView::DetailsView(QWidget *parent)
	: QWidget(parent)
	, m_titleLabel(new QLabel("Select an IIO item.", this))
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
//	setStyleSheet("border: 2px solid green");

	m_titleLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	m_titleLabel->setStyleSheet("color: white;");
	m_titleLabel->setAlignment(Qt::AlignCenter);

	m_scrollAreaContents->setLayout(new QVBoxLayout(m_scrollAreaContents));
//	m_scrollAreaContents->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	m_scrollArea->setWidget(m_scrollAreaContents);
	m_scrollArea->setWidgetResizable(true);
//	m_scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

	m_attrSeparator->setLabel("Attributes");
	m_attrSeparator->getContentWidget()->layout()->addWidget(m_scrollArea);
	m_attrSeparator->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

	m_detailsSeparator->setLabel("Device info");
	m_detailsSeparator->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
//	m_detailsSeparator->setStyleSheet("border: 2px solid red");

	layout()->addWidget(m_titleLabel);
	layout()->addWidget(m_attrSeparator);
	layout()->addWidget(m_detailsSeparator);
//	layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::MinimumExpanding));
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
	// FIXME: this whole system of displaying stuff is wrong, it is supposed to just demonstrate how this looks
	setTitle(item->text());
	clearWidgets();
	auto iioWidgets = item->getIIOWidgets();
	for(auto widget : iioWidgets) {
		widget->show();
		m_currentWidgets.append(widget);
		m_scrollAreaContents->layout()->addWidget(widget);
	}

	auto details = item->getDetails();
	for(const auto &detail : details) {
		auto label = new QLabel(detail);
		label->show();
		m_detailsList.append(label);
		m_detailsSeparator->getContentWidget()->layout()->addWidget(label);
	}
}

void DetailsView::clearWidgets()
{
	// FIXME: This is functional mess
	for(auto widget : m_currentWidgets) {
		widget->hide();
	}
	m_currentWidgets.clear();

	for(auto detail : m_detailsList) {
		detail->hide();
	}
	m_detailsList.clear();
}
