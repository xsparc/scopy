#include "channel_control.h"

adiscope::ChannelControl::ChannelControl(const QString& deviceName, const QString& deviceType, QWidget *parent) :
        gui::GenericMenu(parent),
        ui(new Ui::ChannelControl) {

        this->ui->setupUi(this);
        initInteractiveMenu();

        this->ui->deviceName->setText(deviceName);
        this->ui->deviceType->setText(deviceType);

        this->ui->configModes->addItem("high-side");
        this->ui->configModes->addItem("high-side 2x inrush");
        this->ui->configModes->addItem("simple push-pull");
        this->ui->configModes->addItem("active clamp push-pull");

        this->ui->histogramCh1->setOn(QLabel("high"));
        this->ui->histogramCh1->setOff(QLabel("low"));

        if (deviceType == "input") {
                this->ui->configModes->setDisabled(true);
                this->ui->histogramCh1->setDisabled(true);
        }

        this->ui->scaleChannel->setOrientation(Qt::Horizontal);
        this->ui->scaleChannel->setScalePosition(QwtThermo::LeadingScale);
        this->ui->scaleChannel->setOriginMode(QwtThermo::OriginCustom);
        this->ui->scaleChannel->setMinimumSize(100, 50);
        this->ui->scaleChannel->setAutoScaler(true);
        this->ui->scaleChannel->setScale(0, 1);
        this->ui->scaleChannel->setScaleStepSize(1);
        this->ui->scaleChannel->setOrientation(Qt::Horizontal);
        this->ui->scaleChannel->setScalePosition(QwtThermo::LeadingScale);
        this->ui->scaleChannel->setOriginMode(QwtThermo::OriginCustom);
        this->ui->scaleChannel->setStyleSheet("font-size:16px;");

        this->ui->sismograph->setPlotAxisXTitle(""); // clear title
        this->ui->sismograph->setAxisScale(0, 0, 1, 1); // y axis
        this->ui->sismograph->updateYScale(30, 0);

        this->connectSignalsAndSlots();
}

adiscope::ChannelControl::~ChannelControl() {
        delete ui;
};

void adiscope::ChannelControl::connectSignalsAndSlots() {

}

Ui::ChannelControl *adiscope::ChannelControl::getUi() const {
        return ui;
}

void adiscope::ChannelControl::updateTimeScale(double newMin, double newMax) {
        this->ui->sismograph->updateYScale(newMax, newMin);
}
