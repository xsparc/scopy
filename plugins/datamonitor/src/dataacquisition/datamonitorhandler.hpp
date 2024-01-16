#ifndef DATAMONITORHANDLER_HPP
#define DATAMONITORHANDLER_HPP

#include <QMap>

#include <src/datamonitor/datamonitormodel.hpp>

namespace scopy {
namespace datamonitor {

class DataMonitorHandler
{
public:
	DataMonitorHandler();

	void addMonitor(DataMonitorModel *dataMonitorModel);
	DataMonitorModel *getMonitor(QString monitorTitle);
	void removeMonitor(QString monitorTitle);
	void clearMonitorsData();
	QList<QString> getMonitors();

private:
	QMap<QString, DataMonitorModel *> *dataMonitorMap;
};
} // namespace datamonitor
} // namespace scopy
#endif // DATAMONITORHANDLER_HPP
