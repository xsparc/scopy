#include "datamonitorhandler.hpp"

using namespace scopy;
using namespace datamonitor;
DataMonitorHandler::DataMonitorHandler() { dataMonitorMap = new QMap<QString, DataMonitorModel *>(); }

void DataMonitorHandler::addMonitor(DataMonitorModel *dataMonitorModel)
{
	dataMonitorMap->insert(dataMonitorModel->getTitle(), dataMonitorModel);
}

DataMonitorModel *DataMonitorHandler::getMonitor(QString monitorTitle) { return dataMonitorMap->value(monitorTitle); }

void DataMonitorHandler::removeMonitor(QString monitorTitle) { dataMonitorMap->remove(monitorTitle); }

void DataMonitorHandler::clearMonitorsData()
{
	foreach(auto modelKey, dataMonitorMap->keys()) {
		dataMonitorMap->value(modelKey)->clearMonitorData();
	}
}

QList<QString> DataMonitorHandler::getMonitors() { return dataMonitorMap->keys(); }
