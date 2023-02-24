#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "faultsgroup.hpp"
#include "ui_faultsgroup.h"
#include "logging_categories.h"

adiscope::gui::FaultsGroup::FaultsGroup(const QString& name, const QString& path, QWidget *parent) :
                QWidget(parent),
                ui(new Ui::FaultsGroup),
                name(name)
{
	ui->setupUi(this);

        QString contents; // TODO: arr err/warning messages
        QFile file;
        file.setFileName(path);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        if (!file.isOpen()) {
                qCritical(CAT_NEWINSTRUMENT) << "File could not be opened (read): " << path;
        } else {
                qDebug(CAT_NEWINSTRUMENT) << "File opened (read): " << path;
        }
        contents = file.readAll();
        file.close();

        QJsonDocument document = QJsonDocument::fromJson(contents.toUtf8());
        QJsonObject document_object = document.object();
        QJsonValue device_value = document_object.value(name);
        QJsonObject device_object = device_value.toObject();

        QJsonValue max_faults_value = device_object["max_faults"];
        this->max_faults = max_faults_value.toInt(-1);

        QJsonValue faults_json = device_object["faults"];
        QJsonArray faults_obj = faults_json.toArray();


        for (int i = 0; i < this->max_faults; ++i) {
                QJsonObject fault_object = faults_obj[i].toObject();
                QString fault_name = fault_object.value("name").toString();
                QString fault_description = fault_object.value("description").toString();
                auto fault_widget = new FaultWidget(i, fault_name, fault_description, this);
                this->faults.push_back(fault_widget);
                this->ui->horizontalLayout->addWidget(fault_widget);
        }
}

adiscope::gui::FaultsGroup::~FaultsGroup()
{
        delete ui;
}

const std::vector<FaultWidget *> &adiscope::gui::FaultsGroup::getFaults() const {
        return faults;
}

const QString &adiscope::gui::FaultsGroup::getName() const {
        return name;
}

void adiscope::gui::FaultsGroup::setName(const QString &name_) {
        FaultsGroup::name = name_;
}

int adiscope::gui::FaultsGroup::getMaxFaults() const {
        return max_faults;
}

void adiscope::gui::FaultsGroup::setMaxFaults(int maxFaults) {
        max_faults = maxFaults;
}
