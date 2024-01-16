#ifndef DATAACQUISITIONMANAGER_HPP
#define DATAACQUISITIONMANAGER_HPP

#include "datamonitorhandler.hpp"
#include "ireadstrategy.hpp"

#include <QElapsedTimer>
#include <QMap>
#include <QObject>
#include <QTimer>

namespace scopy {
namespace datamonitor {

class DataAcquisitionManager : public QObject
{
	Q_OBJECT
public:
	explicit DataAcquisitionManager(QObject *parent = nullptr);

	// will read data once
	void readData();
	void startTimer();
	void stopTimer();
	void setTimer(QTimer *timer);
	void setTimerInterval(int timerInterval);

	void setReadStrategy(IReadStrategy *newReadStrategy);

	DataMonitorHandler *getDataMonitorHanlder() const;

	void updateActiveMonitors(bool toggled, QString monitorTitle);

signals:
	void requestUpdateActiveMonitors();
	void requestRead();

private:
	DataMonitorHandler *dataMonitorHanlder;
	QTimer *m_timer;
	IReadStrategy *readStrategy;

	// active monitors ??? moved to another object ???
	QList<QString> *activeMonitors;
	QMap<QString, int> *m_activeMonitorsMap;
};
} // namespace datamonitor
} // namespace scopy
#endif // DATAACQUISITIONMANAGER_HPP
