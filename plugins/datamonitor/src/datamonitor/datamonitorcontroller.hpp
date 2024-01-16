#ifndef DATAMONITORCONTROLLER_H
#define DATAMONITORCONTROLLER_H

#include <QObject>

#include <src/dataacquisition/datamonitorhandler.hpp>

#include "datamonitormodel.hpp"
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

	void setMonitors(DataMonitorHandler *dataMonitorHanlder);

private:
	DataMonitorView *dataMonView;
	DataMonitorHandler *m_dataMonitorHanlder;
};
} // namespace datamonitor
} // namespace scopy
#endif // DATAMONITORCONTROLLER_H
