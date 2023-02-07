#include "swiotmax14906settingstab.h"
#include "ui_swiotmax14906settingstab.h"

Max14906SettingsTab::Max14906SettingsTab(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Max14906SettingsTab)
{
	ui->setupUi(this);


        this->m_max_spin_button = new PositionSpinButton({
                                                               {tr("s"), 1}
                                                       }, tr("Time"), 0, 30, true, false, this);
        this->m_max_spin_button->setValue(10); // default value
        this->ui->verticalLayout_maxTimer->addWidget(this->m_max_spin_button);
}

Max14906SettingsTab::~Max14906SettingsTab()
{
	delete ui;
}

PositionSpinButton *Max14906SettingsTab::getSpinButton() {
        return this->m_max_spin_button;
}
