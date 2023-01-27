#include "newinstrument.hpp"

using namespace adiscope;

struct swiot_fault {
    bool stored;
    bool active;
    const char *const name;
    const char *const explanation;
};

swiot_fault ad74413r_faults[]{
        {false, false, "VI_ERR_A",
                                           "[vo]Voltage output: short-circuit error. The error condition is debounced for 2 ms before the status bit is set."
                                           "[co]Current output: open circuit error. The error condition is debounced for 2 ms before the status bit is set."
                                           "[cil]Current input, loop powered: short-circuit error. A short to ground is detected if the digital input comparator is enabled as described in Current Input Loop Powered section with a trip point of AVDD/2 and the digital output is inverted via the INV_DIN_COMP_OUT bit in the DIN_CONFIGx registers. The debounce time of this error detect is userprogrammable via the DEBOUNCE_TIME bits in the DIN_CONFIGx registers."
                                           "[cie]Current input, externally powered: short-circuit error. A current source >25 mA is detected if the digital input comparator is enabled as described in Current Input Loop Powered section with a trip point of AVDD/2. The debounce time of this error detect is user-programmable via the DEBOUNCE_TIME bits in the DIN_CONFIGx registers."
        },
        {false, false, "VI_ERR_B",
                                           "[vo]Voltage output: short-circuit error. The error condition is debounced for 2 ms before the status bit is set."
                                           "[co]Current output: open circuit error. The error condition is debounced for 2 ms before the status bit is set"
                                           "[cil]Current input, loop powered: short-circuit error. A short to ground is detected if the digital input comparator is enabled as described in Current Input Loop Powered section with a trip point of AVDD/2 and the digital output is inverted via the INV_DIN_COMP_OUT bit in the DIN_CONFIGx registers. The debounce time of this error detect is userprogrammable via the DEBOUNCE_TIME bits in the DIN_CONFIGx registers."
                                           "[cie]Current input, externally powered: short-circuit error. A current source >25 mA is detected if the digital input comparator is enabled as described in Current Input Loop Powered section with a trip point of AVDD/2. The debounce time of this error detect is user-programmable via the DEBOUNCE_TIME bits in the DIN_CONFIGx registers. "
        },
        {false, false, "VI_ERR_C",
                                           "[vo]Voltage output: short-circuit error. The error condition is debounced for 2 ms before the status bit is set."
                                           "[co]Current output: open circuit error. The error condition is debounced for 2 ms before the status bit is set"
                                           "[cil]Current input, loop powered: short-circuit error. A short to ground is detected if the digital input comparator is enabled as described in Current Input Loop Powered section with a trip point of AVDD/2 and the digital output is inverted via the INV_DIN_COMP_OUT bit in the DIN_CONFIGx registers. The debounce time of this error detect is userprogrammable via the DEBOUNCE_TIME bits in the DIN_CONFIGx registers."
                                           "[cie]Current input, externally powered: short-circuit error. A current source >25 mA is detected if the digital input comparator is enabled as described in Current Input Loop Powered section with a trip point of AVDD/2. The debounce time of this error detect is user-programmable via the DEBOUNCE_TIME bits in the DIN_CONFIGx registers. "
        },
        {false, false, "VI_ERR_D",
                                           "[vo]Voltage output: short-circuit error. The error condition is debounced for 2 ms before the status bit is set."
                                           "[co]Current output: open circuit error. The error condition is debounced for 2 ms before the status bit is set"
                                           "[cil]Current input, loop powered: short-circuit error. A short to ground is detected if the digital input comparator is enabled as described in Current Input Loop Powered section with a trip point of AVDD/2 and the digital output is inverted via the INV_DIN_COMP_OUT bit in the DIN_CONFIGx registers. The debounce time of this error detect is userprogrammable via the DEBOUNCE_TIME bits in the DIN_CONFIGx registers."
                                           "[cie]Current input, externally powered: short-circuit error. A current source >25 mA is detected if the digital input comparator is enabled as described in Current Input Loop Powered section with a trip point of AVDD/2. The debounce time of this error detect is user-programmable via the DEBOUNCE_TIME bits in the DIN_CONFIGx registers. "
        },
        {false, false, "HI_TEMP_ERR",      "High temperature detected. After the die temperature typically reaches 115°C, this bit is asserted."},
        {false, false,  "CHARGE_PUMP_ERR",  "Charge pump error detected. This bit is asserted when the AVSS pin rises above −1.65 V."},
        {false, false, "ALDO5V_ERR",       "ALDO5V Power Supply Monitor Error. This bit is asserted when the ALDO5V pin falls below 4.05 V."},
        {false, false, "AVDD_ERR",         "AVDD Power Supply Monitor Error. This bit is asserted when the AVDD pin falls below 9.26 V."},
        {false, false, "DVCC_ERR",         "DVCC Power Supply Monitor Error. This bit is asserted when the DVCC pin falls below 1.93 V."},
        {false, false, "ALDO1V8_ERR",      "ALDO1V8 Power Supply Monitor Error. This bit is asserted when the ALDO1V8 pin falls below 1.35 V."},
        {false, false, "ADC_CONV_ERR",     "ADC Conversion Error. ADC results may be outside the selected measurement range."},
        {false, false, "ADC_SAT_ERR",      "ADC Saturation Error. ADC may be outside the user selected measurement range."},
        {false, false, "SPI_SCLK_CNT_ERR", "SPI SCLK count error detected. This bit is asserted if an SPI command is applied but 32 SCLKs are not provided."},
        {false, false, "SPI_CRC_ERR",      "SPI CRC error detected. This bit is asserted if an invalid CRC is received."},
        {false, false, "CAL_MEM_ERR",
                                           "Calibration Memory Error. This flag asserts under the following two conditions:\n"
                                           "\tWhen a calibration memory CRC error or an uncorrectable error correcting code (ECC) error is detected on the calibration memory upload. It is not possible to clear this bit if there is a CRC error or uncorrectable ECC error. It is recommended to reset the device and check the supplies in this situation.\n"
                                           "\tWhen there is an attempted SPI access to a register before the calibration memory refresh is complete. Do not address the device until the calibration memory is refreshed. Writing 1 to this bit clears the flag, if the flag is asserted due to this condition."
        },
        {false, false, "RESET_OCCURRED",   "Reset occurred. This bit is asserted after a reset event, which asserts the ALERT pin after the reset. Write a 1 to this bit to clear the flag. Note that a mask bit is not provided for this bit."}
};

swiot_fault max1490b_faults[]{
        {false, false, "OVL1",      "Thermal overload is detected on DOI1"},
        {false, false,  "OVL2",      "Thermal overload is detected on DOI2"},
        {false, false, "OVL3",      "Thermal overload is detected on DOI3"},
        {false, false, "OVL4",      "Thermal overload is detected on DOI4"},
        {false, false, "CL1",       "Current limit detected on DOI1"},
        {false, false, "CL2",       "Current limit detected on DOI2"},
        {false, false,  "CL3",       "Current limit detected on DOI3"},
        {false, false, "CL4",       "Current limit detected on DOI4"},
        {false, false, "OWOff1",    "Open-wire fault is detected on DOI1 when the high-side switch is in the off state"},
        {false, false, "OWOff2",    "Open-wire fault is detected on DOI2 when the high-side switch is in the off state"},
        {false, false, "OWOff3",    "Open-wire fault is detected on DOI3 when the high-side switch is in the off state"},
        {false, false, "OWOff4",    "Open-wire fault is detected on DOI4 when the high-side switch is in the off state"},
        {false, false, "AboveVDD1", "Above-VDD fault is detected on DOI1"},
        {false, false, "AboveVDD2", "Above-VDD fault is detected on DOI2"},
        {false, false, "AboveVDD3", "Above-VDD fault is detected on DOI3"},
        {false, false, "AboveVDD4", "Above-VDD fault is detected on DOI4"},
        {false, false, "SHVDD1",    "Short-to-VDD fault is detected on DOI1 when the high-side switch is in the off state"},
        {false, false, "SHVDD2",    "Short-to-VDD fault is detected on DOI2 when the high-side switch is in the off state"},
        {false, false, "SHVDD3",    "Short-to-VDD fault is detected on DOI3 when the high-side switch is in the off state"},
        {false, false, "SHVDD4",    "Short-to-VDD fault is detected on DOI4 when the high-side switch is in the off state"},
        {false, false,  "VDDOV1",    "VDD1 supply is higher than the VDD_OVTH threshold voltage (43.5V, typ)"},
        {false, false, "VDDOV2",    "VDD2 supply is higher than the VDD_OVTH threshold voltage (43.5V, typ)"},
        {false, false, "VDDOV3",    "VDD3 supply is higher than the VDD_OVTH threshold voltage (43.5V, typ)"},
        {false, false, "VDDOV4",    "VDD4 supply is higher than the VDD_OVTH threshold voltage (43.5V, typ)"},
        {false, false, "VINT_UV",   "Undervoltage on the internal supply for the SPI registers"},
        {false, false, "V5_UVLO",   "V5 undervoltage"},
        {false, false, "VDD_Low",   "Below VDD_GOOD threshold"},
        {false, false, "VDD_Warn",  "Below VDD_WARN threshold"},
        {false, false, "VDD_UVLO",  "VDD supply in UVLO; all DOI_ switches turned off"},
        {false, false,  "ThrmShutd", "The MAX14906 enters thermal shutdown"},
        {false, false, "LossGND",   "The MAX14906 detects loss of GND fault"},
        {false, false,  "WDogErr",   "SPI or SYNCH watchdog timeout is detected"},
};

NewInstrument::NewInstrument(struct iio_context *ctx, Filter *filt,
                             ToolMenuItem *toolMenuItem,
                             QJSEngine *engine, ToolLauncher *parent) :
        Tool(ctx, toolMenuItem, nullptr, "NewInstrument",
             parent),
        ui(new Ui::NewInstrument) {
        ui->setupUi(this);
        run_button = nullptr;

        this->connectSignalsAndSlots();

        // FIXME: testing only
        this->ad74413r_faults_num = 16;
        this->max1490b_faults_num = 32;


        this->ad74413rSetup();
        this->max1490bSetup();
}

NewInstrument::~NewInstrument() {
        delete ui;
}

void NewInstrument::connectSignalsAndSlots() {
        QObject::connect(ui->ad_reset_button, &QPushButton::clicked, this, &NewInstrument::resetStoredAd74413r);
        QObject::connect(ui->max_reset_button, &QPushButton::clicked, this, &NewInstrument::resetStoredMax1490b);
}

void NewInstrument::generateAd74413rButtons() {
        for (int i = 0; i < this->ad74413r_faults_num; ++i) {
                auto a = new QPushButton();
                auto b = new QPushButton();
                a->setStyleSheet(this->buttonStyleSheet);
                b->setStyleSheet(this->buttonStyleSheet);
                auto pair = new QPair<QPushButton*, QPushButton*>;
                pair->first = a;
                pair->second = b;

                this->ad74413r_fault_buttons.append(pair);
        }
}

void NewInstrument::generateMax1490bButtons() {
        for (int i = 0; i < this->max1490b_faults_num; ++i) {
                auto a = new QPushButton();
                auto b = new QPushButton();
                a->setStyleSheet(this->buttonStyleSheet);
                b->setStyleSheet(this->buttonStyleSheet);
                auto pair = new QPair<QPushButton*, QPushButton*>;
                pair->first = a;
                pair->second = b;

                this->max1490b_fault_buttons.append(pair);
        }
}


void NewInstrument::ad74413rSetup() {
        this->getAd74413rFaultsNumeric();
        this->generateAd74413rButtons();
        this->initAd74413rFaults();
        this->setAd74413rFaults();
        this->populateAdExplanations();
}

void NewInstrument::max1490bSetup() {
        this->getMax1490bFaultsNumeric();
        this->generateMax1490bButtons();
        this->initMax1490bFaults();
        this->setMax1490bFaults();
        this->populateMaxExplanations();
}

void NewInstrument::getAd74413rFaultsNumeric() {
        iio_device *dev = iio_context_get_device(ctx, 0);
        qDebug() << "dev name: " << iio_device_get_name(dev);
        iio_channel *chn = iio_device_find_channel(dev, "voltage", false);
        if (chn == nullptr) {
                qDebug() << "Device is in config mode";
                return;
        }
        char fau[100];
        iio_channel_attr_read(chn, "raw", fau, 100);
        qDebug() << "read attr: " << fau;

        uint32_t result = std::stoi(fau);
        this->ui->lineEdit_adNumeric->setText(QString::number(result));

        for (int i = this->ad74413r_faults_num-1; i >= 0; --i) {
                ad74413r_faults[i].stored = (bool)(result & 0b1);
                result >>= 1;
        }
}


void NewInstrument::getMax1490bFaultsNumeric() {
        iio_device *dev = iio_context_get_device(ctx, 1); // TODO: make it safer :))
        qDebug() << "dev name: " << iio_device_get_name(dev);
        iio_channel *chn = iio_device_find_channel(dev, "voltage", false);
        if (chn == nullptr) {
                qDebug() << "Device is in config mode";
                return;
        }
        char fau[100];
        iio_channel_attr_read(chn, "raw", fau, 100);
        qDebug() << "read attr: " << fau;

        uint32_t result = std::stoi(fau);
        this->ui->lineEdit_maxNumeric->setText(QString::number(result));

        for (int i = this->max1490b_faults_num-1; i >= 0; --i) {
                max1490b_faults[i].stored = (bool)(result & 0b1);
                result >>= 1;
        }
}

void NewInstrument::initAd74413rFaults() {
        for (int i = 0; i < this->ad74413r_faults_num; ++i) {
                this->ui->gridL_ad_faults->addWidget(this->ad74413r_fault_buttons[i]->first, 0, i);
        }

        for (int i = 0; i < this->ad74413r_faults_num; ++i) {
                this->ui->gridL_ad_faults->addWidget(this->ad74413r_fault_buttons[i]->second, 1, i);
        }

        for (int i = 0; i < this->ad74413r_faults_num; ++i) {
                auto c = new QPushButton();
                c->setText(tr(ad74413r_faults[i].name));
                this->ui->gridL_ad_faults->addWidget(c, 2, i);
        }
}

void NewInstrument::initMax1490bFaults() {
        for (int i = 0; i < this->max1490b_faults_num; ++i) {
                this->ui->gridL_max_faults->addWidget(this->max1490b_fault_buttons[i]->first, 0, i);
        }

        for (int i = 0; i < this->max1490b_faults_num; ++i) {
                this->ui->gridL_max_faults->addWidget(this->max1490b_fault_buttons[i]->second, 1, i);
        }

        for (int i = 0; i < this->max1490b_faults_num; ++i) {
                auto a = new QPushButton();
                a->setText(tr(max1490b_faults[i].name));
                this->ui->gridL_max_faults->addWidget(a, 2, i);
        }
}

void NewInstrument::setAd74413rFaults() {
        for (int i = 0; i < this->ad74413r_faults_num; ++i) {
                this->ad74413r_fault_buttons[i]->first->setProperty("high", ad74413r_faults[i].stored);
                this->ad74413r_fault_buttons[i]->first->style()->polish(this->ad74413r_fault_buttons[i]->first);

                this->ad74413r_fault_buttons[i]->second->setProperty("high", ad74413r_faults[i].active);
                this->ad74413r_fault_buttons[i]->second->style()->polish(this->ad74413r_fault_buttons[i]->second);
        }
}

void NewInstrument::setMax1490bFaults() {
        for (int i = 0; i < this->max1490b_faults_num; ++i) {
                this->max1490b_fault_buttons[i]->first->setProperty("high", max1490b_faults[i].stored);
                this->max1490b_fault_buttons[i]->first->style()->polish(this->max1490b_fault_buttons[i]->first);

                this->max1490b_fault_buttons[i]->second->setProperty("high", max1490b_faults[i].active);
                this->max1490b_fault_buttons[i]->second->style()->polish(this->max1490b_fault_buttons[i]->second);
        }
}

void NewInstrument::populateAdExplanations() {
        this->ui->ad_faults_explanation->clear();
        for (const auto &ad74413r_fault: ad74413r_faults) {
                if (ad74413r_fault.active) {
                        this->ui->ad_faults_explanation->append(tr(ad74413r_fault.explanation));
                }
        }

        if (this->ui->ad_faults_explanation->toPlainText().isEmpty()) {
                this->ui->ad_faults_explanation->setText(tr("No faults as of now"));
        }
}

void NewInstrument::populateMaxExplanations() {
        this->ui->max_faults_explanation->clear();

        for (const auto &max1490b_fault: max1490b_faults) {
                if (max1490b_fault.active) {
                        this->ui->max_faults_explanation->append(tr(max1490b_fault.explanation));
                }
        }

        if (this->ui->max_faults_explanation->toPlainText().isEmpty()) {
                this->ui->max_faults_explanation->setText(tr("No faults as of now"));
        }
}

void NewInstrument::resetStoredAd74413r() {
        for (auto &ad74413r_fault: ad74413r_faults) {
                ad74413r_fault.stored = false;
        }

        this->setAd74413rFaults();
}

void NewInstrument::resetStoredMax1490b() {
        for (auto &max1490b_fault: max1490b_faults) {
                max1490b_fault.stored = false;
        }

        this->setMax1490bFaults();
}
