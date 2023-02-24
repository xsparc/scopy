#include "swiotfaultsdevice.hpp"

#include <utility>

namespace adiscope::gui {
        FaultsDevice::FaultsDevice(QString name, QString path, uint32_t max_faults_num, QWidget *parent)
                : ui(new Ui::FaultsDevice),
                m_parent(parent),
                m_faultsGroup(new FaultsGroup(name, path, this)),
                m_name(std::move(name)),
                m_path(std::move(path)),
                m_max_faults_num(max_faults_num){

                ui->setupUi(this);

                this->ui->label_name->setText(this->m_name);

                this->ui->reset_button->setProperty("blue_button", QVariant(true));
                this->ui->clear_selection_button->setProperty("blue_button", QVariant(true));

                this->ui->faults_layout->addWidget(this->m_faultsGroup);
        }

        FaultsDevice::~FaultsDevice() {
                delete ui;
        }

        QPushButton *FaultsDevice::getResetButton() {
                return this->ui->reset_button;
        }

        FaultsGroup *FaultsDevice::getFaultsGroup() const {
                return m_faultsGroup;
        }

        QLabel *FaultsDevice::getExplanations() {
                return this->ui->faults_explanation;
        }

        void FaultsDevice::setNumericText(const QString &text) {
                this->ui->lineEdit_numeric->setText(text);
        }
} // adiscope