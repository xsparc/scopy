#ifndef DATAMONITORTOOL_HPP
#define DATAMONITORTOOL_HPP

#include "datamonitortoolsettings.hpp"

#include <QPushButton>
#include <QWidget>
#include <tooltemplate.h>
#include <toolbuttons.h>
#include <QMap>
#include <QScrollArea>

#include <src/dataacquisition/dataacquisitionmanager.hpp>

#include <src/datamonitor/datamonitorcontroller.hpp>

namespace scopy {

class CollapsableMenuControlButton;

class FlexGridLayout;
namespace datamonitor {

class DataMonitorTool : public QWidget
{
	Q_OBJECT
public:
	explicit DataMonitorTool(QWidget *parent = nullptr);

	RunBtn *getRunButton() const;

	void initDataMonitor();

private:
	ToolTemplate *tool;
	QPushButton *openLatMenuBtn;
	GearBtn *settingsButton;
	InfoBtn *infoBtn;
	RunBtn *runBtn;
	QPushButton *clearBtn;

	FlexGridLayout *m_flexGridLayout;
	QScrollArea *m_scrollArea;

	QButtonGroup *grp;

	DataMonitorToolSettings *generalSettings;
	DataAcquisitionManager *m_dataAcquisitionManager;
	QMap<int, DataMonitorController *> *m_monitorControllers;
	int activeMonitor = -1;

	// TODO remove
	void addMonitor();
};
} // namespace datamonitor
} // namespace scopy
#endif // DATAMONITORTOOL_HPP
