#ifndef DATAMONITORCONTROLLER_H
#define DATAMONITORCONTROLLER_H

#include <QObject>

#include <src/dataacquisition/datamonitorhandler.hpp>

#include "datamonitormodel.hpp"
#include "datamonitorsettings.hpp"
#include "datamonitorview.hpp"

namespace scopy {
namespace datamonitor {

class DataMonitorController : public QObject
{
	Q_OBJECT
public:
	DataMonitorController(QObject *parent = nullptr);

	~DataMonitorController();

	DataMonitorView *getDataMonitorView();
	DataMonitorSettings *getDataMonitorSettings() const;

	void setMonitors(DataMonitorHandler *dataMonitorHanlder);

private:
	DataMonitorView *dataMonView;
	DataMonitorHandler *m_dataMonitorHanlder;
	DataMonitorSettings *dataMonitorSettings = nullptr;
};
} // namespace datamonitor
} // namespace scopy
#endif // DATAMONITORCONTROLLER_H
