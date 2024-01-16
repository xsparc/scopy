#ifndef DATAMONITORSTYLEHELPER_HPP
#define DATAMONITORSTYLEHELPER_HPP

#include "datamonitortoolsettings.hpp"

#include <QObject>

#include <src/datamonitor/datamonitorsettings.hpp>
#include <src/datamonitor/datamonitorview.hpp>

namespace scopy {
namespace datamonitor {

class DataMonitorStyleHelper : public QObject
{
	Q_OBJECT
public:
	explicit DataMonitorStyleHelper(QObject *parent = nullptr);

	static void DataMonitorViewStyle(DataMonitorView *dataMonitorView, QColor color);
	static void DataMonitorSettingsStyle(DataMonitorSettings *dataMonitorSettings);
	static void DataMonitorToolSettingsStyle(DataMonitorToolSettings *dataMonitorToolSettings);

signals:
};
} // namespace datamonitor
} // namespace scopy
#endif // DATAMONITORSTYLEHELPER_HPP
