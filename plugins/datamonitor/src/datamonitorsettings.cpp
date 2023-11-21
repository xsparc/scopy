#include "datamonitorsettings.hpp"

#include <menusectionwidget.h>

using namespace scopy;
using namespace datamonitor;

DataMonitorSettings::DataMonitorSettings(QWidget *parent)
	: QWidget{parent}
{
	layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(10);
	setLayout(layout);
}

DataMonitorSettings::~DataMonitorSettings() {}

void DataMonitorSettings::init(QString title, QColor color)
{
	header = new MenuHeaderWidget(title, QPen(color), this);
	layout->addWidget(header);

	MenuSectionWidget *scaleSection = new MenuSectionWidget(this);
	layout->addWidget(scaleSection);

	scaleSWitch = new MenuOnOffSwitch("Scale", scaleSection, false);
	scaleSWitch->onOffswitch()->setChecked(true);
	scaleSection->contentLayout()->addWidget(scaleSWitch);

	MenuSectionWidget *historySection = new MenuSectionWidget(this);
	layout->addWidget(historySection);

	historySwitch = new MenuOnOffSwitch("History", historySection, false);
	historySwitch->onOffswitch()->setChecked(true);
	historySection->contentLayout()->addWidget(historySwitch);

	historySize = new MenuComboWidget("History Size", historySection);
	historySize->combo()->addItem(QString("1s"));
	historySize->combo()->addItem(QString("10s"));
	historySize->combo()->addItem(QString("60s"));
	historySize->combo()->setCurrentIndex(1);

	historySection->contentLayout()->addWidget(historySize);

	historyStyle = new MenuComboWidget("History Style", historySection);
	historyStyle->combo()->addItem(QString("Solid Line"));
	historyStyle->combo()->addItem(QString("Dot Line"));
	historyStyle->combo()->addItem(QString("Dash Line"));
	historyStyle->combo()->addItem(QString("Dash-Dot Line"));
	historyStyle->combo()->addItem(QString("Dash-Dot-Dot Line"));
	historyStyle->combo()->setCurrentIndex(0);

	historySection->contentLayout()->addWidget(historyStyle);

	MenuSectionWidget *peakHolderSection = new MenuSectionWidget(this);
	layout->addWidget(peakHolderSection);

	peakHolderSwitch = new MenuOnOffSwitch("Peak Holder ", peakHolderSection, false);
	peakHolderSwitch->onOffswitch()->setChecked(true);
	peakHolderReset = new QPushButton("Reset", peakHolderSection);

	peakHolderSection->contentLayout()->addWidget(peakHolderSwitch);
	peakHolderSection->contentLayout()->addWidget(peakHolderReset);

	QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Expanding);
	layout->addItem(spacer);

	connect(scaleSWitch->onOffswitch(), &QAbstractButton::toggled, this, &DataMonitorSettings::toggleScale);
	connect(historySwitch->onOffswitch(), &QAbstractButton::toggled, this, &DataMonitorSettings::toggleHistory);
	connect(peakHolderReset, &QPushButton::clicked, this, &DataMonitorSettings::resetPeakHolder);
	connect(peakHolderSwitch->onOffswitch(), &QAbstractButton::toggled, this,
		&DataMonitorSettings::togglePeakHolder);
	connect(historySize->combo(), QOverload<int>::of(&QComboBox::currentIndexChanged), this,
		&DataMonitorSettings::historySizeIndexChanged);
	connect(historyStyle->combo(), QOverload<int>::of(&QComboBox::currentIndexChanged), this,
		&DataMonitorSettings::changeLineStyle);
}

void DataMonitorSettings::scaleToggle(bool toggled) { scaleSWitch->onOffswitch()->setChecked(toggled); }

void DataMonitorSettings::peakHolderToggle(bool toggled) { peakHolderSwitch->onOffswitch()->setChecked(toggled); }

void DataMonitorSettings::historyToggle(bool toggled) { historySwitch->onOffswitch()->setChecked(toggled); }

void DataMonitorSettings::peakHolderResetClicked() { peakHolderReset->click(); }

void DataMonitorSettings::historySizeChanged(int size)
{
	historySize->combo()->setCurrentIndex(size);
	setHistorySize(size);
}

void DataMonitorSettings::changeLineStyle(int index)
{
	//	historyStyle->combo()->setCurrentIndex(index);
	Q_EMIT lineStyleChanged(lineStyleFromIdx(index));
}
double DataMonitorSettings::numSamplesFromIdx(int idx)
{
	switch(idx) {
	case 0:
		return 1.0;
	case 1:
		return 10.0;
	case 2:
		return 60.0;
	default:
		throw std::runtime_error("Invalid INDEX");
	}
}

Qt::PenStyle DataMonitorSettings::lineStyleFromIdx(int idx)
{
	switch(idx) {
	case 0:
		return Qt::PenStyle::SolidLine;
	case 1:
		return Qt::PenStyle::DotLine;
	case 2:
		return Qt::PenStyle::DashLine;
	case 3:
		return Qt::PenStyle::DashDotLine;
	case 4:
		return Qt::PenStyle::DashDotDotLine;
	}
	return Qt::PenStyle::SolidLine;
}

void DataMonitorSettings::setHistorySize(int idx) { Q_EMIT changeHistorySize(numSamplesFromIdx(idx)); }
