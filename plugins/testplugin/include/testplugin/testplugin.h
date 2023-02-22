#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#define SCOPY_PLUGIN_NAME TestPlugin
#define SCOPY_PLUGIN_PRIO 1

#include <QObject>
#include <pluginbase/plugin.h>
#include <pluginbase/pluginbase.h>
#include "scopytestplugin_export.h"

namespace adiscope {
class SCOPYTESTPLUGIN_EXPORT TestPlugin : public QObject, public PluginBase
{
	Q_OBJECT
	SCOPY_PLUGIN
public:
	bool compatible(QString uri) override;	
	bool load(QString uri) override;
	void unload() override;

	bool connectDev() override;
	bool disconnectDev() override;
};
}

#endif // TESTPLUGIN_H
