#include "datamonitorcontroller.hpp"

using namespace scopy;
using namespace datamonitor;

DataMonitorController::DataMonitorController(QObject *parent)
{
	dataMonView = new DataMonitorView();

	// Generate settings
	dataMonitorSettings = new DataMonitorSettings();
	dataMonitorSettings->init(dataMonView->getTitle(), StyleHelper::getColor("ScopyBlue"));

	// peak holder settings
	connect(dataMonitorSettings, &DataMonitorSettings::togglePeakHolder, dataMonView,
		&DataMonitorView::togglePeakHolder);

	connect(dataMonitorSettings, &DataMonitorSettings::mainMonitorChanged, dataMonView, [=](QString monitorTitle) {
		// TODO disconnect old monitor connect to new monitor

		dataMonView->configureMonitor(m_dataMonitorHanlder->getMonitor(monitorTitle));

		disconnect(dataMonitorSettings, &DataMonitorSettings::resetPeakHolder,
			   m_dataMonitorHanlder->getMonitor(monitorTitle), &DataMonitorModel::resetMinMax);

		connect(dataMonitorSettings, &DataMonitorSettings::resetPeakHolder,
			m_dataMonitorHanlder->getMonitor(monitorTitle), &DataMonitorModel::resetMinMax);
	});

	connect(dataMonView, &DataMonitorView::titleChanged, dataMonitorSettings, &DataMonitorSettings::updateTitle);

	// plot settings

	connect(dataMonitorSettings, &DataMonitorSettings::togglePlot, dataMonView, &DataMonitorView::togglePlot);
	connect(dataMonitorSettings, &DataMonitorSettings::lineStyleIndexChanged, dataMonView->getMonitorPlot(),
		[=](int style) {
			dataMonView->getMonitorPlot()->changeCurveStyle(dataMonView->getMainMonitor(), style);
		});

	connect(dataMonitorSettings, &DataMonitorSettings::monitorToggled, dataMonView->getMonitorPlot(),
		[=](bool toggled, QString monitorTitle) {
			if(dataMonView->getMonitorPlot()->hasMonitor(monitorTitle)) {
				dataMonView->getMonitorPlot()->toggleMonitor(toggled, monitorTitle);
			} else {
				if(dataMonView->getMonitorPlot()->firstMonitor()) {
					dataMonitorSettings->updateMainMonitor(monitorTitle);
					dataMonView->configureMonitor(m_dataMonitorHanlder->getMonitor(monitorTitle));

					connect(dataMonitorSettings, &DataMonitorSettings::resetPeakHolder,
						m_dataMonitorHanlder->getMonitor(monitorTitle),
						&DataMonitorModel::resetMinMax);

					dataMonView->getMonitorPlot()->setFirstMonitor(false);
				}

				dataMonView->getMonitorPlot()->addMonitor(
					m_dataMonitorHanlder->getMonitor(monitorTitle));
			}
		});
}

DataMonitorController::~DataMonitorController()
{
	delete dataMonView;
	if(dataMonitorSettings)
		delete dataMonitorSettings;
}

DataMonitorView *DataMonitorController::getDataMonitorView() { return dataMonView; }

DataMonitorSettings *DataMonitorController::getDataMonitorSettings() const { return dataMonitorSettings; }

void DataMonitorController::setMonitors(DataMonitorHandler *dataMonitorHanlder)
{
	m_dataMonitorHanlder = dataMonitorHanlder;
}
