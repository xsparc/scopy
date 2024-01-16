#include "datamonitorsettings.hpp"

#include <menusectionwidget.h>
#include <QScrollArea>

#include <src/datamonitorstylehelper.hpp>

using namespace scopy;
using namespace datamonitor;

DataMonitorSettings::DataMonitorSettings(QWidget *parent)
	: QWidget{parent}
{
	mainLayout = new QVBoxLayout(this);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(10);
	setLayout(mainLayout);
}

DataMonitorSettings::~DataMonitorSettings() {}

void DataMonitorSettings::init(QString title, QColor color)
{
	header = new MenuHeaderWidget(title, QPen(color), this);
	mainLayout->addWidget(header);

	settingsBody = new QWidget(this);
	layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(10);
	settingsBody->setLayout(layout);

	mainLayout->addLayout(layout);

	QScrollArea *scrollArea = new QScrollArea(this);
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(settingsBody);
	mainLayout->addWidget(scrollArea);

	MenuSectionWidget *scaleSection = new MenuSectionWidget(this);
	layout->addWidget(scaleSection);

	scaleSWitch = new MenuOnOffSwitch("Scale", scaleSection, false);
	scaleSWitch->onOffswitch()->setChecked(true);
	scaleSection->contentLayout()->addWidget(scaleSWitch);

	MenuSectionWidget *plotSection = new MenuSectionWidget(this);
	layout->addWidget(plotSection);

	plotSwitch = new MenuOnOffSwitch("Plot", plotSection, false);
	plotSwitch->onOffswitch()->setChecked(true);
	plotSection->contentLayout()->addWidget(plotSwitch);

	plotSize = new MenuComboWidget("Plot Size", plotSection);
	plotSize->combo()->addItem(QString("1s"));
	plotSize->combo()->addItem(QString("10s"));
	plotSize->combo()->addItem(QString("60s"));
	plotSize->combo()->setCurrentIndex(1);

	plotSection->contentLayout()->addWidget(plotSize);

	plotStyle = new MenuCombo("Plot Style", plotSection);
	auto cbStyle = plotStyle->combo();
	cbStyle->addItem("Lines", PlotChannel::PCS_LINES);
	cbStyle->addItem("Dots", PlotChannel::PCS_DOTS);
	cbStyle->addItem("Steps", PlotChannel::PCS_STEPS);
	cbStyle->addItem("Sticks", PlotChannel::PCS_STICKS);
	cbStyle->addItem("Smooth", PlotChannel::PCS_SMOOTH);
	StyleHelper::MenuComboBox(cbStyle, "");
	cbStyle->setCurrentIndex(0);

	plotSection->contentLayout()->addWidget(plotStyle);

	MenuSectionWidget *peakHolderSection = new MenuSectionWidget(this);
	peakHolderSection->contentLayout()->setSpacing(10);
	layout->addWidget(peakHolderSection);

	peakHolderSwitch = new MenuOnOffSwitch("Peak Holder ", peakHolderSection, false);
	peakHolderSwitch->onOffswitch()->setChecked(true);
	peakHolderReset = new QPushButton("Reset", peakHolderSection);

	peakHolderSection->contentLayout()->addWidget(peakHolderSwitch);
	peakHolderSection->contentLayout()->addWidget(peakHolderReset);

	connect(scaleSWitch->onOffswitch(), &QAbstractButton::toggled, this, &DataMonitorSettings::toggleScale);
	connect(plotSwitch->onOffswitch(), &QAbstractButton::toggled, this, &DataMonitorSettings::togglePlot);
	connect(peakHolderReset, &QPushButton::clicked, this, &DataMonitorSettings::resetPeakHolder);
	connect(peakHolderSwitch->onOffswitch(), &QAbstractButton::toggled, this,
		&DataMonitorSettings::togglePeakHolder);
	connect(plotSize->combo(), QOverload<int>::of(&QComboBox::currentIndexChanged), this,
		&DataMonitorSettings::plotSizeIndexChanged);
	connect(cbStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
		&DataMonitorSettings::changeLineStyle);

	MenuSectionWidget *removeMonitorSection = new MenuSectionWidget(this);
	layout->addWidget(removeMonitorSection);

	deleteMonitor = new QPushButton("Remove monitor");
	removeMonitorSection->contentLayout()->addWidget(deleteMonitor);
	connect(deleteMonitor, &QPushButton::clicked, this, &DataMonitorSettings::removeMonitor);

	QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Expanding);
	layout->addItem(spacer);

	DataMonitorStyleHelper::DataMonitorSettingsStyle(this);
}

void DataMonitorSettings::scaleToggle(bool toggled) { scaleSWitch->onOffswitch()->setChecked(toggled); }

void DataMonitorSettings::peakHolderToggle(bool toggled) { peakHolderSwitch->onOffswitch()->setChecked(toggled); }

void DataMonitorSettings::plotToggle(bool toggled) { plotSwitch->onOffswitch()->setChecked(toggled); }

void DataMonitorSettings::peakHolderResetClicked() { peakHolderReset->click(); }

void DataMonitorSettings::plotSizeChanged(int size)
{
	plotSize->combo()->setCurrentIndex(size);
	setPlotSize(size);
}

void DataMonitorSettings::changeLineStyle(int index) { Q_EMIT lineStyleIndexChanged(index); }

void DataMonitorSettings::addMonitorsList(QList<QString> monitoList)
{
	monitorsCheckboxList = new QList<QPair<QString, QCheckBox *>>();
	activeMonitors = new QList<QString>;

	QWidget *selectMonitorsWidgets = new QWidget(this);
	QVBoxLayout *selectMonitorsLayout = new QVBoxLayout(selectMonitorsWidgets);
	selectMonitorsWidgets->setLayout(selectMonitorsLayout);

	mainMonitorCombo = new MenuComboWidget("Main Monitor", selectMonitorsWidgets);
	selectMonitorsLayout->addWidget(mainMonitorCombo);

	layout->addWidget(selectMonitorsWidgets);

	QWidget *activeMonitorsWidget = new QWidget(selectMonitorsWidgets);
	QVBoxLayout *activeMonitorsLayout = new QVBoxLayout(activeMonitorsWidget);
	activeMonitorsWidget->setLayout(activeMonitorsLayout);

	layout->addWidget(activeMonitorsWidget);

	selectMonitorsLayout->addWidget(new QLabel("Toggle monitors: "));
	selectMonitorsLayout->addWidget(activeMonitorsWidget);

	for(int i = 0; i < monitoList.length(); i++) {
		QCheckBox *select = new QCheckBox(activeMonitorsWidget);
		activeMonitorsLayout->addWidget(select);
		select->setText(monitoList.at(i));

		monitorsCheckboxList->push_back(qMakePair(monitoList.at(i), select));

		connect(select, &QCheckBox::toggled, this, [=](bool toggled) {
			if(toggled) {
				activeMonitors->append(monitoList.at(i));
			} else {
				activeMonitors->removeAt(activeMonitors->indexOf(monitoList.at(i)));
			}
			mainMonitorCombo->combo()->blockSignals(true);
			mainMonitorCombo->combo()->clear();
			mainMonitorCombo->combo()->addItems(*activeMonitors);
			mainMonitorCombo->combo()->blockSignals(false);

			Q_EMIT monitorToggled(toggled, monitoList.at(i));
		});
	}

	mainMonitorCombo->combo()->setCurrentIndex(0);

	connect(mainMonitorCombo->combo(), &QComboBox::currentTextChanged, this,
		&DataMonitorSettings::mainMonitorChanged);
}

void DataMonitorSettings::updateTitle(QString title) { header->label()->setText(title); }

void DataMonitorSettings::updateMainMonitor(QString monitorTitle)
{
	mainMonitorCombo->combo()->setCurrentText(monitorTitle);
}

QList<QString> *DataMonitorSettings::getActiveMonitors() { return activeMonitors; }
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

void DataMonitorSettings::setPlotSize(int idx) { Q_EMIT changeHistorySize(numSamplesFromIdx(idx)); }
