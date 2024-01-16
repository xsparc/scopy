#include "dataloggercontroller.hpp"
#include "datamonitorstylehelper.hpp"
#include "datamonitortoolsettings.hpp"

#include <menusectionwidget.h>

using namespace scopy;
using namespace datamonitor;

DataMonitorToolSettings::DataMonitorToolSettings(QWidget *parent)
	: QWidget{parent}
{
	layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(10);
	setLayout(layout);
}

DataMonitorToolSettings::~DataMonitorToolSettings() {}

void DataMonitorToolSettings::init(QString title, QColor color)
{
	header = new MenuHeaderWidget(title, QPen(color), this);
	layout->addWidget(header);

	MenuSectionWidget *showAllSection = new MenuSectionWidget(this);
	layout->addWidget(showAllSection);

	showAllSwitch = new MenuOnOffSwitch("Show all", showAllSection, false);
	showAllSwitch->onOffswitch()->setChecked(true);
	connect(showAllSwitch->onOffswitch(), &QAbstractButton::toggled, this, &DataMonitorToolSettings::toggleAll);
	showAllSection->contentLayout()->addWidget(showAllSwitch);

	MenuSectionWidget *precisionSection = new MenuSectionWidget(this);
	layout->addWidget(precisionSection);

	QHBoxLayout *precisionLayout = new QHBoxLayout();
	precisionLayout->setMargin(0);
	precisionLayout->setSpacing(10);

	precisionBtn = new QPushButton("Set", precisionSection);

	precisionValue = new QLineEdit(precisionSection);
	precisionValue->setValidator(new QIntValidator(this));
	precisionValue->setText("3");

	connect(precisionBtn, &QPushButton::clicked, this, [=]() {
		int value = precisionValue->text().toInt();
		if(value < 0) {
			precisionValue->setText("0");
		}
		if(value >= 10) {
			precisionValue->setText("9");
		}

		Q_EMIT precisionChanged(precisionValue->text().toInt());
	});

	precisionLayout->addWidget(precisionBtn, Qt::AlignLeft);
	precisionLayout->addWidget(precisionValue, Qt::AlignRight);
	precisionSection->contentLayout()->addLayout(precisionLayout);

	MenuSectionWidget *recordingIntervalSection = new MenuSectionWidget(this);
	layout->addWidget(recordingIntervalSection);

	recordingTimer = new PositionSpinButton({{"s", 1}, {"min", 60}, {"h", 3600}}, tr("Timer"), 0, 3600, true, false,
						recordingIntervalSection);

	recordingTimer->setValue(1);
	recordingIntervalSection->contentLayout()->addWidget(recordingTimer);
	connect(recordingTimer, &PositionSpinButton::valueChanged, this, [=]() {
		double interval = recordingTimer->value() * 1000;
		if(interval < 100) {
			interval = 100;
			recordingTimer->setValue(interval);
		}
		Q_EMIT recordingIntervalChanged(interval);
	});

	MenuSectionWidget *dataLoggingSection = new MenuSectionWidget(this);
	layout->addWidget(dataLoggingSection);

	dataLoggerController = new DataLoggerController(true, true, false);
	dataLoggerController->setWarningMessage("* While data logging you won't be able to add/remove channels");

	dataLoggingSection->contentLayout()->addWidget(dataLoggerController->getWidget());

	connect(this, &DataMonitorToolSettings::updateDataLoggerValue, dataLoggerController,
		&DataLoggerController::receiveValue);

	QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Expanding);
	layout->addItem(spacer);

	DataMonitorStyleHelper::DataMonitorToolSettingsStyle(this);
}
