#ifndef NEWINSTRUMENT_HPP
#define NEWINSTRUMENT_HPP

#include <filter.hpp>
//#include "src/tool_launcher.hpp"
#include "channel_control.h"
#include "ui_newinstrument.h"
#include "max14906settingstab.h"
#include "gui/tool_view.hpp"
#include "gui/customcolqgridlayout.hpp"
#include <tool.hpp>
#include "newinstrument_logic.hpp"

namespace adiscope {
    class ChannelControl;
    class NewInstrumentLogic;

    class NewInstrument : public Tool {
    Q_OBJECT

        Ui::NewInstrument *ui;
        Max14906SettingsTab *max14906SettingsTab;
        adiscope::gui::ToolView* m_toolView{};
        CustomColQGridLayout* m_customColGrid{};
        adiscope::gui::GenericMenu* m_generalSettingsMenu{};
        adiscope::gui::ChannelManager* m_monitorChannelManager{};

        QTimer *qTimer{};
    public:

        gui::ToolView *getMToolView() const;
    private:
        std::vector<ChannelControl*> channelControls;

    protected:
        NewInstrumentLogic *newInstrumentLogic;

    public:
        NewInstrument(struct iio_context *ctx, Filter *filt,
                      ToolMenuItem *toolMenuItem,
                      QJSEngine *engine, ToolLauncher *parent);

        ~NewInstrument() override;


    Q_SIGNALS:

        void showTool();

    private Q_SLOTS:
        void runButtonToggled();
        void singleButtonToggled();

        void timerChanged();
    private:
        void setupDynamicUi(ToolLauncher *parent);
        void initMonitorToolView();
        adiscope::gui::GenericMenu* createGeneralSettings(const QString& title, QColor* color);
        void connectSignalsAndSlots();
    };
}
#endif // NEWINSTRUMENT_HPP
