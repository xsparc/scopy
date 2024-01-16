#include "datamonitorcontroller.hpp"

using namespace scopy;
using namespace datamonitor;

DataMonitorController::DataMonitorController(QObject *parent) { dataMonView = new DataMonitorView(); }

DataMonitorController::~DataMonitorController() { delete dataMonView; }

DataMonitorView *DataMonitorController::getDataMonitorView() { return dataMonView; }

void DataMonitorController::setMonitors(DataMonitorHandler *dataMonitorHanlder)
{
	m_dataMonitorHanlder = dataMonitorHanlder;
}
