#include "dataacquisitionmanager.hpp"

using namespace scopy;
using namespace datamonitor;

DataAcquisitionManager::DataAcquisitionManager(QObject *parent)
	: QObject{parent}
{
	dataMonitorHanlder = new DataMonitorHandler();
	activeMonitors = new QList<QString>();
	m_activeMonitorsMap = new QMap<QString, int>();
}

void DataAcquisitionManager::readData()
{
	// TODO read data using read strategy
	// TODO add a readthread
	//		readerThread->start();

	foreach(QString monKey, m_activeMonitorsMap->keys()) {
		readStrategy->read(monKey);
	}
}

void DataAcquisitionManager::startTimer() { m_timer->start(); }

void DataAcquisitionManager::stopTimer() { m_timer->stop(); }

void DataAcquisitionManager::setTimer(QTimer *timer)
{
	m_timer = timer;
	connect(m_timer, &QTimer::timeout, this, [=]() { readData(); });
}

void DataAcquisitionManager::setTimerInterval(int timerInterval) { m_timer->setInterval(timerInterval); }

void DataAcquisitionManager::setReadStrategy(IReadStrategy *newReadStrategy)
{
	readStrategy = newReadStrategy;

	connect(readStrategy, &IReadStrategy::readDone, this,
		[=](QString title, double value) { dataMonitorHanlder->getMonitor(title)->updateValue(0, value); });
}

DataMonitorHandler *DataAcquisitionManager::getDataMonitorHanlder() const { return dataMonitorHanlder; }

void DataAcquisitionManager::updateActiveMonitors(bool toggled, QString monitorTitle)
{
	if(toggled) {
		// monitor enabled
		if(m_activeMonitorsMap->contains(monitorTitle)) {
			// more than one view is using the monitor
			int value = m_activeMonitorsMap->value(monitorTitle);
			m_activeMonitorsMap->insert(monitorTitle, ++value);

		} else {
			// one view is using the monitor
			m_activeMonitorsMap->insert(monitorTitle, 1);
		}
	} else {
		// monitor disabled
		if(m_activeMonitorsMap->value(monitorTitle) <= 1) {
			// when no view has the monitor enabled remove it from active monitors
			m_activeMonitorsMap->remove(monitorTitle);
		} else {
			// one less view is using the monitor
			int value = m_activeMonitorsMap->value(monitorTitle);
			m_activeMonitorsMap->insert(monitorTitle, --value);
		}
	}
}
