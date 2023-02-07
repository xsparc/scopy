#include "newinstrument.hpp"
#include "gui/tool_view_builder.hpp"
#include "gui/customcolqgridlayout.hpp"

using namespace adiscope;

NewInstrument::NewInstrument(struct iio_context *ctx, Filter *filt,
                             ToolMenuItem *toolMenuItem,
                             QJSEngine *engine, ToolLauncher *parent) :
        Tool(ctx, toolMenuItem, nullptr, "NewInstrument", parent),
        ui(new Ui::NewInstrument),
        max14906SettingsTab(new Max14906SettingsTab(this)),
        qTimer(new QTimer(this)),
        newInstrumentLogic(new NewInstrumentLogic(ctx)) {
        run_button = nullptr;

        int channel_num = this->newInstrumentLogic->getChannelCount();

        for (int i = 0; i < channel_num - 1; ++i) { // -1 because of the fault channel
                auto *channel_control = new ChannelControl(
                        this->newInstrumentLogic->getChannelName(i),
                        this->newInstrumentLogic->getChannelType(i),
                        this
                );
                channel_control->updateTimeScale(0, this->max14906SettingsTab->getSpinButton()->value());
                this->channelControls.push_back(channel_control);
        }

        this->qTimer->setInterval(1000); // poll once every second
        this->qTimer->setSingleShot(true);

        this->saveOnExit = false; // FIXME: might need to change once it gets preferences

        this->setupDynamicUi(parent);
        this->initMonitorToolView();
        this->connectSignalsAndSlots();
}

void NewInstrument::setupDynamicUi(ToolLauncher *parent) {
        adiscope::gui::ToolViewRecipe recipe;
        recipe.helpBtnUrl = "";
        recipe.hasRunBtn = true;
        recipe.hasSingleBtn = true;
        recipe.hasPairSettingsBtn = true;
        recipe.hasPrintBtn = false;
        recipe.hasChannels = false;
        recipe.channelsPosition = adiscope::gui::ChannelsPositionEnum::HORIZONTAL;

        this->m_monitorChannelManager = new adiscope::gui::ChannelManager(recipe.channelsPosition);
        m_monitorChannelManager->setChannelIdVisible(false);

        m_toolView = adiscope::gui::ToolViewBuilder(recipe, this->m_monitorChannelManager, parent).build();

        this->m_generalSettingsMenu = this->createGeneralSettings("General settings", new QColor("#4a64ff"));
        this->m_toolView->setGeneralSettingsMenu(this->m_generalSettingsMenu, true);

        this->m_customColGrid = new CustomColQGridLayout(100, this);

        this->m_toolView->addFixedCentralWidget(m_customColGrid, 0, 0, 0, 0); // TODO: make this dockable ??

        setCentralWidget(getMToolView());
        this->m_toolView->getGeneralSettingsBtn()->setChecked(true);
}

void NewInstrument::connectSignalsAndSlots() {
        connect(m_toolView->getRunBtn(), &QPushButton::toggled, this, &NewInstrument::runButtonToggled);
        connect(m_toolView->getSingleBtn(), &QPushButton::toggled, this, &NewInstrument::singleButtonToggled);

        connect(this->max14906SettingsTab->getSpinButton(), &PositionSpinButton::valueChanged, this, &NewInstrument::timerChanged);
}

NewInstrument::~NewInstrument() {
        this->m_toolView->getRunBtn()->setChecked(false);

        delete ui;
}

gui::ToolView *NewInstrument::getMToolView() const {
        return m_toolView;
}

void NewInstrument::runButtonToggled() {

}

void NewInstrument::singleButtonToggled() {
        if (m_toolView->getRunBtn()->isChecked()) {
                this->m_toolView->getRunBtn()->setChecked(false);
        }

//        for (auto & channelControl : this->channelControls) {
//
//        }

        qTimer->start(0);
}

void NewInstrument::timerChanged() {
        for (auto & channelControl : this->channelControls) {
                channelControl->updateTimeScale(0, this->max14906SettingsTab->getSpinButton()->value());
        }
}

adiscope::gui::GenericMenu* NewInstrument::createGeneralSettings(const QString& title, QColor* color) {
        auto generalSettingsMenu = new adiscope::gui::GenericMenu(this);
        generalSettingsMenu->initInteractiveMenu();
        generalSettingsMenu->setMenuHeader(title, color, false);

        this->max14906SettingsTab = new Max14906SettingsTab();
        auto *settingsWidgetSeparator = new adiscope::gui::SubsectionSeparator("MAX14906", false, this);
        settingsWidgetSeparator->setContent(this->max14906SettingsTab);
        generalSettingsMenu->insertSection(settingsWidgetSeparator);


        return generalSettingsMenu;
}

void NewInstrument::initMonitorToolView() {
        for (int i = 0; i < this->channelControls.size(); ++i) {
                this->m_customColGrid->addQWidgetToList(this->channelControls[i]);
                this->m_customColGrid->addWidget(i);
        }

        setUpdatesEnabled(true);
}
