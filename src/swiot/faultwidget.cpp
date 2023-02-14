#include "faultwidget.hpp"

#include <utility>
#include "ui_faultwidget.h"


FaultWidget::FaultWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::FaultWidget), stored(false), active(false) {
        ui->setupUi(this);
}

FaultWidget::FaultWidget(QString name, QString faultExplanation, QWidget *parent) :
        QWidget(parent), ui(new Ui::FaultWidget), stored(false), active(false), name(std::move(name)),
        faultExplanation(std::move(faultExplanation)) {
        ui->setupUi(this);

        this->ui->title->setText(this->name);
}

FaultWidget::~FaultWidget() {
        delete ui;
}

bool FaultWidget::isStored() const {
        return stored;
}

void FaultWidget::setStored(bool stored_) {
        FaultWidget::stored = stored_;
        this->ui->storedButton->setProperty("high", this->stored);
        this->ui->storedButton->setStyle(this->ui->storedButton->style());
}

bool FaultWidget::isActive() const {
        return active;
}

void FaultWidget::setActive(bool active_) {
        FaultWidget::active = active_;
        this->ui->activeButton->setProperty("high", this->active);
        this->ui->activeButton->setStyle(this->ui->activeButton->style());
}

const QString &FaultWidget::getName() const {
        return name;
}

void FaultWidget::setName(const QString &name_) {
        FaultWidget::name = name_;
}

const QString &FaultWidget::getFaultExplanation() const {
        return faultExplanation;
}

void FaultWidget::setFaultExplanation(const QString &faultExplanation_) {
        FaultWidget::faultExplanation = faultExplanation_;
}
