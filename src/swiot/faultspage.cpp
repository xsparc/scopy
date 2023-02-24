#include "faultspage.hpp"
#include "ui_faultspage.h"

adiscope::gui::FaultsPage::FaultsPage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FaultsPage),
        m_ad74413rFaultsDevice(new FaultsDevice("ad74413r", ":/swiot/swiot_faults.json", 16, this)),
        m_max14906FaultsDevice(new FaultsDevice("max14906", ":/swiot/swiot_faults.json", 32, this))
{
	ui->setupUi(this);

        this->ui->mainVerticalLayout->addWidget(this->m_ad74413rFaultsDevice);
        this->ui->mainVerticalLayout->addWidget(this->m_max14906FaultsDevice);
}

adiscope::gui::FaultsPage::~FaultsPage() {
	delete ui;
}

QPushButton *adiscope::gui::FaultsPage::getAdResetButton() {
        return this->m_ad74413rFaultsDevice->getResetButton();
}

QPushButton *adiscope::gui::FaultsPage::getMaxResetButton() {
        return this->m_max14906FaultsDevice->getResetButton();
}

adiscope::gui::FaultsGroup *adiscope::gui::FaultsPage::getAdFaultsGroup() const {
        return this->m_ad74413rFaultsDevice->getFaultsGroup();
}

adiscope::gui::FaultsGroup *adiscope::gui::FaultsPage::getMaxFaultsGroup() const {
        return this->m_max14906FaultsDevice->getFaultsGroup();
}

void adiscope::gui::FaultsPage::setAdNumericText(const QString& text) {
        this->m_ad74413rFaultsDevice->setNumericText(text);
}

void adiscope::gui::FaultsPage::setMaxNumericText(const QString &text) {
        this->m_max14906FaultsDevice->setNumericText(text);
}

QLabel *adiscope::gui::FaultsPage::getAdExplanations() {
        return this->m_ad74413rFaultsDevice->getExplanations();
}

QLabel *adiscope::gui::FaultsPage::getMaxExplanations() {
        return this->m_max14906FaultsDevice->getExplanations();
}
