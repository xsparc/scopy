#include "swiotfaults.hpp"
#include "gui/tool_view_builder.hpp"
#include "gui/channel_manager.hpp"
#include "logging_categories.h"

#define POLLING_INTERVAL 1000
#define FAULT_CHANNEL_NAME "voltage"

using namespace adiscope;

SwiotFaults::SwiotFaults(struct iio_context *ctx, Filter *filt,
                         ToolMenuItem *toolMenuItem,
                         QJSEngine *engine, ToolLauncher *parent) :
        Tool(ctx, toolMenuItem, nullptr, "SwiotFaults", parent),
        ui(new Ui::SwiotFaults),
        timer(new QTimer(this)),
        thread(new QThread(this)),
        faultsPage(new adiscope::gui::FaultsPage(this)) {
        qInfo(CAT_NEWINSTRUMENT) << "Initialising SWIOT faults page";

        ui->setupUi(this);
        run_button = nullptr;

        this->ad74413r_faults_count = 16;
        this->max14906_faults_count = 32;

        this->setupDynamicUi(parent);
        this->connectSignalsAndSlots();

        this->ad74413rSetup();
        this->max14906Setup();
}

SwiotFaults::~SwiotFaults() {
        if (this->thread->isRunning()) {
                this->thread->quit();
                this->thread->wait();
        }
        delete thread;
        delete ui;
}

void SwiotFaults::setupDynamicUi(ToolLauncher *parent) {
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

        this->m_toolView = adiscope::gui::ToolViewBuilder(recipe, this->m_monitorChannelManager, parent).build();

        this->m_generalSettingsMenu = this->createGeneralSettings("General settings", new QColor("#4a64ff"));
        this->m_toolView->setGeneralSettingsMenu(this->m_generalSettingsMenu, true);

        this->m_customColGrid = new CustomColQGridLayout(100, this);
        this->m_customColGrid->addQWidgetToList(this->faultsPage);
        this->m_customColGrid->addWidget(0);

        this->m_toolView->addFixedCentralWidget(m_customColGrid, 0, 0, 0, 0);

        setCentralWidget(getMToolView());
        this->m_toolView->getGeneralSettingsBtn()->setChecked(true);
}

adiscope::gui::GenericMenu *SwiotFaults::createGeneralSettings(const QString &title, QColor *color) {
        auto generalSettingsMenu = new adiscope::gui::GenericMenu(this);
        generalSettingsMenu->initInteractiveMenu();
        generalSettingsMenu->setMenuHeader(title, color, false);

        auto *testLabel = new QLabel("coming soon");
        auto *settingsWidgetSeparator = new adiscope::gui::SubsectionSeparator("MAX14906", false, this);
        settingsWidgetSeparator->setContent(testLabel);
        generalSettingsMenu->insertSection(settingsWidgetSeparator);

        return generalSettingsMenu;
}

void SwiotFaults::connectSignalsAndSlots() {
        QObject::connect(this->m_toolView->getRunBtn(), &QPushButton::toggled, this,
                         &SwiotFaults::runButtonClicked);
        QObject::connect(this->m_toolView->getSingleBtn(), &QPushButton::clicked, this,
                         &SwiotFaults::singleButtonClicked);

        QObject::connect(this->faultsPage->getAdResetButton(), &QPushButton::clicked, this,
                         &SwiotFaults::resetStoredAd74413r);
        QObject::connect(this->faultsPage->getMaxResetButton(), &QPushButton::clicked, this,
                         &SwiotFaults::resetStoredMax14906);

        QObject::connect(this->timer, &QTimer::timeout, this, &SwiotFaults::pollFaults);
        QObject::connect(this->thread, &QThread::started, this, [&](){
                qDebug(CAT_NEWINSTRUMENT) << "Faults reader thread started";
                this->timer->start(POLLING_INTERVAL);
        });
}

void SwiotFaults::ad74413rSetup() {
        this->getAd74413rFaultsNumeric();
        this->setAd74413rFaults();
        this->populateAd74413rExplanations();
}

void SwiotFaults::max14906Setup() {
        this->getMax14906FaultsNumeric();
        this->setMax14906Faults();
        this->populateMax14906Explanations();
}

void SwiotFaults::getAd74413rFaultsNumeric() {
        iio_device *dev = nullptr;
        unsigned int devCount = iio_context_get_devices_count(ctx);
        for (int i = 0; i < devCount; ++i) {
                iio_device *aux = iio_context_get_device(ctx, i);
                std::string name = iio_device_get_name(aux);
                if (name == "ad74413r") {
                        dev = aux;
                        break;
                }
        }

        if (dev == nullptr) {
                qCritical(CAT_NEWINSTRUMENT) << "Critical: No device was found";
                return;
        }

        iio_channel *chn = iio_device_find_channel(dev, FAULT_CHANNEL_NAME, false);
        if (chn == nullptr) {
                qCritical(CAT_NEWINSTRUMENT) << "Device is not found";
                return;
        }

        char fau[100];
        iio_channel_attr_read(chn, "raw", fau, 100);

        qDebug(CAT_NEWINSTRUMENT) << "ad74413r_numeric: " << fau;
        this->ad74413r_numeric = std::stoi(fau);
}

void SwiotFaults::getMax14906FaultsNumeric() {
        iio_device *dev = nullptr;
        unsigned int devCount = iio_context_get_devices_count(ctx);
        for (int i = 0; i < devCount; ++i) {
                iio_device *aux = iio_context_get_device(ctx, i);
                std::string name = iio_device_get_name(aux);
                if (name == "max14906") {
                        dev = aux;
                        break;
                }
        }

        if (dev == nullptr) {
                qCritical(CAT_NEWINSTRUMENT) << "Critical: No device was found";
                return;
        }

        iio_channel *chn = iio_device_find_channel(dev, FAULT_CHANNEL_NAME, false);
        if (chn == nullptr) {
                qCritical(CAT_NEWINSTRUMENT) << "Device is not found";
                return;
        }

        char fau[100];
        iio_channel_attr_read(chn, "raw", fau, 100);

        qDebug(CAT_NEWINSTRUMENT) << "max14906_numeric: " << fau;
        this->max14906_numeric = std::stoi(fau);
}

void SwiotFaults::setAd74413rFaults() {
        this->faultsPage->setAdNumericText(QString("0x%1").arg(this->ad74413r_numeric, 8, 16, QLatin1Char('0')));

        uint32_t aux = this->ad74413r_numeric;
        for (int i = this->ad74413r_faults_count - 1; i >= 0; --i) {
                bool bit = (bool) (aux & 0b1);
                if (this->faultsPage->getAdFaultsGroup()->getFaults()[i]->isActive() &&
                    bit) { // if we get 2 active signals, we set the stored to 1
                        this->faultsPage->getAdFaultsGroup()->getFaults()[i]->setStored(true);
                }
                this->faultsPage->getAdFaultsGroup()->getFaults()[i]->setActive(bit);
                aux >>= 1;
        }
}

void SwiotFaults::setMax14906Faults() {
        this->faultsPage->setMaxNumericText(QString("0x%1").arg(this->max14906_numeric, 8, 16, QLatin1Char('0')));

        uint32_t aux = this->max14906_numeric;
        for (int i = this->max14906_faults_count - 1; i >= 0; --i) {
                bool bit = (bool) (aux & 0b1);
                if (this->faultsPage->getMaxFaultsGroup()->getFaults()[i]->isActive() &&
                    bit) { // if we get 2 active signals, we set the stored to 1
                        this->faultsPage->getMaxFaultsGroup()->getFaults()[i]->setStored(true);
                }
                this->faultsPage->getMaxFaultsGroup()->getFaults()[i]->setActive(bit);
                aux >>= 1;
        }
}

void SwiotFaults::resetStoredAd74413r() {
        qDebug(CAT_NEWINSTRUMENT) << "Resetting Ad74413r Stored";
        for (auto &ad74413r_fault: this->faultsPage->getAdFaultsGroup()->getFaults()) {
                ad74413r_fault->setStored(false);
        }
}

void SwiotFaults::resetStoredMax14906() {
        qDebug(CAT_NEWINSTRUMENT) << "Resetting Max14906 Stored";
        for (auto &max1490b_fault: this->faultsPage->getMaxFaultsGroup()->getFaults()) {
                max1490b_fault->setStored(false);
        }
}

gui::ToolView *SwiotFaults::getMToolView() const {
        return m_toolView;
}

void SwiotFaults::populateAd74413rExplanations() {
        this->faultsPage->getAdExplanations()->clear();
        QString explanations;
        for (auto &ad74413r_faultWidget: this->faultsPage->getAdFaultsGroup()->getFaults()) {
                if (ad74413r_faultWidget->isStored()) {
                        explanations.append(ad74413r_faultWidget->getFaultExplanation());
                        explanations.append('\n');
                }
        }
        this->faultsPage->getAdExplanations()->setText(explanations);
}

void SwiotFaults::populateMax14906Explanations() {
        this->faultsPage->getMaxExplanations()->clear();
        QString explanations;
        for (auto &ad74413r_faultWidget: this->faultsPage->getMaxFaultsGroup()->getFaults()) {
                if (ad74413r_faultWidget->isStored()) {
                        explanations.append(ad74413r_faultWidget->getFaultExplanation());
                        explanations.append('\n');
                }
        }
        this->faultsPage->getMaxExplanations()->setText(explanations);
}

void SwiotFaults::runButtonClicked() {
        qDebug(CAT_NEWINSTRUMENT) << "Run button clicked";
        this->m_toolView->getSingleBtn()->setChecked(false);
        if (this->m_toolView->getRunBtn()->isChecked()) {
                qDebug(CAT_NEWINSTRUMENT) << "Reader thread started";
                this->thread->start();
        } else {
                if (this->thread->isRunning()) {
                        qDebug(CAT_NEWINSTRUMENT) << "Reader thread stopped";
                        this->thread->quit();
                        this->thread->wait();
                }
                this->timer->stop();
        }
}

void SwiotFaults::singleButtonClicked() {
        qDebug(CAT_NEWINSTRUMENT) << "Single button clicked";
        this->m_toolView->getRunBtn()->setChecked(false);
        this->timer->stop();
        this->pollFaults();
        this->m_toolView->getSingleBtn()->setChecked(false);
}

void SwiotFaults::pollFaults() {
        qDebug(CAT_NEWINSTRUMENT) << "Polling faults...";
        this->ad74413rSetup();
        this->max14906Setup();
}
