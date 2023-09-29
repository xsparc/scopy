#include "registercontroller.hpp"
#include "qtextspinbox.hpp"

#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qdebug.h>
#include <QPushButton>
#include <qspinbox.h>

#include "utils.hpp"
#include "regmapstylehelper.hpp"

using namespace scopy;
using namespace regmap;

RegisterController::RegisterController(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    QWidget *readWidget = new QWidget(this);
    mainLayout->addWidget(readWidget);
    QHBoxLayout *readWidgetLayout = new QHBoxLayout(readWidget);
    readWidget->setLayout(readWidgetLayout);

    QVBoxLayout *readWidgetLeftLayout = new QVBoxLayout();
    addressLabel = new QLabel("Address: ", readWidget);
    readWidgetLeftLayout->addWidget(addressLabel);

    addressPicker = new QSpinBox(readWidget);
    addressPicker->setDisplayIntegerBase(16);
    addressPicker->setMinimum(0);
    addressPicker->setMaximum(INT_MAX);
    addressPicker->setPrefix("0x");
    addressPicker->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    QObject::connect(addressPicker, &QSpinBox::textChanged, this, [=](QString address){
        bool ok;
        Q_EMIT	registerAddressChanged(address.toInt(&ok,16));
    });

    readWidgetLeftLayout->addWidget(addressPicker);
    readWidgetLayout->addLayout(readWidgetLeftLayout,3);

    readButton = new QPushButton("Read", readWidget);
    //request read
    QObject::connect( readButton, &QPushButton::clicked, this , [=](){
        bool ok;
        Q_EMIT requestRead(addressPicker->text().toInt(&ok,16));
    });

    readWidgetLayout->addWidget(readButton, 1, Qt::AlignRight);

    QWidget *writeWidget = new QWidget(this);
    mainLayout->addWidget(writeWidget);
    QHBoxLayout *writeWidgetLayout = new QHBoxLayout(writeWidget);
    writeWidget->setLayout(writeWidgetLayout);

    QVBoxLayout *writeWidgetLeftLayout = new QVBoxLayout();
    valueLabel = new QLabel("Value: ", writeWidget);
    writeWidgetLeftLayout->addWidget(valueLabel);

    regValue = new QLineEdit(writeWidget);
    regValue->setText("N/R");
    regValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QObject::connect(regValue, &QLineEdit::textChanged, this, &RegisterController::valueChanged);

    writeWidgetLeftLayout->addWidget(regValue);
    writeWidgetLayout->addLayout(writeWidgetLeftLayout, 3);

    writeButton = new QPushButton("Write", writeWidget);
    //request write on register
    QObject::connect( writeButton, &QPushButton::clicked, this, [=](){
        bool ok;
        Q_EMIT requestWrite(addressPicker->value(), regValue->text().toInt(&ok,16));
    });

    writeWidgetLayout->addWidget(writeButton, 1, Qt::AlignRight);

    applyStyle();
}

RegisterController::~RegisterController()
{

    delete regValue;
    delete addressPicker;
    if (nameLabel) delete nameLabel;
    if (descriptionLabel) delete descriptionLabel;
    delete mainLayout;
}

void RegisterController::registerChanged(uint32_t address)
{
    if (!addressChanged) {
        addressPicker->setValue(address);
    } else {
        addressChanged = false;
    }
}

void RegisterController::registerValueChanged(QString value)
{
    regValue->setText(value);
}

void RegisterController::applyStyle()
{
    setMaximumHeight(90);
    setStyleSheet( RegmapStyleHelper::simpleWidgetWithButtonStyle(nullptr));
    RegmapStyleHelper::labelStyle(addressLabel, "");
    RegmapStyleHelper::labelStyle(valueLabel, "");
    if (nameLabel) RegmapStyleHelper::labelStyle(nameLabel, "");
    if (descriptionLabel) RegmapStyleHelper::labelStyle(descriptionLabel, "");
}
