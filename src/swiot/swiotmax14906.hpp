#ifndef MAX14906TOOL_HPP
#define MAX14906TOOL_HPP

#include "src/filter.hpp"
#include "src/swiot/swiotdigitalchannel.h"
#include "ui_swiotmax14906.h"
#include "swiot/swiotmax14906settingstab.h"
#include "gui/tool_view.hpp"
#include "gui/customcolqgridlayout.hpp"
#include "src/tool.hpp"
#include "swiot/swiotmax14906controller.hpp"
#include "gui/tool_view_builder.hpp"
#include "gui/customcolqgridlayout.hpp"
#include "src/swiot/swiotmax14906readerthread.h"

#define POLLING_INTERVAL 1000

namespace adiscope {
class DigitalChannel;
class Max14906ToolController;

class Max14906Tool : public Tool {
	Q_OBJECT
public:
	Max14906Tool(struct iio_context *ctx, Filter *filt,
		     ToolMenuItem *toolMenuItem,
		     QJSEngine *engine, ToolLauncher *parent);

	~Max14906Tool() override;

	gui::ToolView *getMToolView() const;

private Q_SLOTS:
	void runButtonToggled();
	void singleButtonToggled();

	void timerChanged(double value);

private:
	void initChannels();
	void setupDynamicUi(ToolLauncher *parent);
	void initMonitorToolView();
	adiscope::gui::GenericMenu* createGeneralSettings(const QString& title, QColor* color);
	void connectSignalsAndSlots();

	Max14906ToolController *max14906ToolController;
	Ui::Max14906Tool *ui;
	Max14906SettingsTab *m_max14906SettingsTab;
	adiscope::gui::ToolView* m_toolView;
	CustomColQGridLayout* m_customColGrid;
	adiscope::gui::GenericMenu* m_generalSettingsMenu;
	adiscope::gui::ChannelManager* m_monitorChannelManager;
	QTimer *m_qTimer;

	MaxReaderThread *m_readerThread;
	QMap<int, DigitalChannel*> m_channelControls;

	adiscope::gui::SubsectionSeparator *settingsWidgetSeparator;
};
}
#endif // MAX14906TOOL_HPP
