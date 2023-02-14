#include "faultspage.hpp"
#include "ui_faultspage.h"

adiscope::gui::FaultsPage::FaultsPage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FaultsPage),
	adFaultsGroup(new FaultsGroup("ad74413r", ":/swiot/swiot_faults.json", this)),
        maxFaultsGroup(new FaultsGroup("max14906", ":/swiot/swiot_faults.json", this))
{
	ui->setupUi(this);

        this->ui->ad_reset_button->setProperty("blue_button", QVariant(true));
        this->ui->max_reset_button->setProperty("blue_button", QVariant(true));

	this->ui->ad74413rFaults->addWidget(this->adFaultsGroup);
	this->ui->max14906Faults->addWidget(this->maxFaultsGroup);
}

adiscope::gui::FaultsPage::~FaultsPage()
{
	delete ui;
}

QPushButton *adiscope::gui::FaultsPage::getAdResetButton() {
        return this->ui->ad_reset_button;
}

QPushButton *adiscope::gui::FaultsPage::getMaxResetButton() {
        return this->ui->max_reset_button;
}

adiscope::gui::FaultsGroup *adiscope::gui::FaultsPage::getAdFaultsGroup() const {
        return this->adFaultsGroup;
}

adiscope::gui::FaultsGroup *adiscope::gui::FaultsPage::getMaxFaultsGroup() const {
        return this->maxFaultsGroup;
}

void adiscope::gui::FaultsPage::setAdNumericText(const QString& text) {
        this->ui->lineEdit_adNumeric->setText(text);
}

void adiscope::gui::FaultsPage::setMaxNumericText(const QString &text) {
        this->ui->lineEdit_maxNumeric->setText(text);
}

QTextEdit *adiscope::gui::FaultsPage::getAdExplanations() {
        return this->ui->ad_faults_explanation;
}

QTextEdit *adiscope::gui::FaultsPage::getMaxExplanations() {
        return this->ui->max_faults_explanation;
}
