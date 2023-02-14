#ifndef NEWINSTRUMENT_HPP
#define NEWINSTRUMENT_HPP

#include "src/filter.hpp"
#include "src/tool_launcher.hpp"
#include "ui_newinstrument.h"
#include "src/tool.hpp"
#include "src/swiot/faultwidget.hpp"
#include "src/swiot/faultsgroup.hpp"
#include "gui/tool_view.hpp"
#include "gui/customcolqgridlayout.hpp"
#include "gui/generic_menu.hpp"
#include "gui/channel_manager.hpp"
#include "src/swiot/faultspage.hpp"


namespace adiscope {
    class NewInstrument : public Tool {
    Q_OBJECT

    private:
        Ui::NewInstrument *ui;
        QTimer *timer;
        QThread *thread;

        QVector<QPair<QPushButton *, QPushButton *> *> ad74413r_fault_buttons;
        QVector<QPair<QPushButton *, QPushButton *> *> max14906_fault_buttons;

        uint16_t ad74413r_faults_count;
        uint16_t max14906_faults_count;

        uint32_t ad74413r_numeric = 0;
        uint32_t max14906_numeric = 0;

        adiscope::gui::ToolView *m_toolView{};
        CustomColQGridLayout *m_customColGrid{};
        adiscope::gui::GenericMenu *m_generalSettingsMenu{};
        adiscope::gui::ChannelManager *m_monitorChannelManager{};
        adiscope::gui::FaultsPage *faultsPage{};
    public:
        gui::ToolView *getMToolView() const;

    private:
        void connectSignalsAndSlots();

        void setupDynamicUi(ToolLauncher *parent);

        gui::GenericMenu *createGeneralSettings(const QString &title, QColor *color);

    public:
        NewInstrument(struct iio_context *ctx, Filter *filt,
                      ToolMenuItem *toolMenuItem,
                      QJSEngine *engine, ToolLauncher *parent);

        ~NewInstrument() override;

        void ad74413rSetup();
        void max14906Setup();

        void getAd74413rFaultsNumeric();
        void getMax14906FaultsNumeric();

        void setAd74413rFaults();
        void setMax14906Faults();

        void populateAd74413rExplanations();
        void populateMax14906Explanations();

        void pollFaults();

    protected Q_SLOTS:

        void resetStoredAd74413r();
        void resetStoredMax14906();

        void runButtonClicked();
        void singleButtonClicked();
    };
}
#endif // NEWINSTRUMENT_HPP
