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

	this->m_toolView->addFixedCentralWidget(this->faultsPage,0,0,-1,-1);

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

	QObject::connect(this->timer, &QTimer::timeout, this, &SwiotFaults::pollFaults);
	QObject::connect(this->thread, &QThread::started, this, [&](){
		qDebug(CAT_NEWINSTRUMENT) << "Faults reader thread started";
		this->timer->start(POLLING_INTERVAL);
	});
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
		qCritical(CAT_NEWINSTRUMENT) << "No device was found";
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
		qCritical(CAT_NEWINSTRUMENT) << "No device was found";
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

gui::ToolView *SwiotFaults::getMToolView() const {
	return m_toolView;
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
	this->getAd74413rFaultsNumeric();
	this->getMax14906FaultsNumeric();
	this->faultsPage->update(this->ad74413r_numeric, this->max14906_numeric);
}
