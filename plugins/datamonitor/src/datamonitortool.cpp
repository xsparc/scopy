#include "datamonitortool.hpp"

#include <QBoxLayout>
#include <flexgridlayout.hpp>
#include <menucontrolbutton.h>
#include <stylehelper.h>
#include <verticalchannelmanager.h>

#include <src/dataacquisition/testreadstrategy.hpp>

using namespace scopy;
using namespace datamonitor;

DataMonitorTool::DataMonitorTool(QWidget *parent)
	: QWidget{parent}
{
	QHBoxLayout *lay = new QHBoxLayout(this);
	setLayout(lay);

	tool = new ToolTemplate(this);
	tool->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	tool->topContainer()->setVisible(true);
	tool->leftContainer()->setVisible(true);
	tool->rightContainer()->setVisible(true);

	tool->setRightContainerWidth(300);
	tool->centralContainer()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	tool->openLeftContainerHelper(false);
	tool->openRightContainerHelper(false);

	lay->addWidget(tool);

	openLatMenuBtn = new OpenLastMenuBtn(dynamic_cast<MenuHAnim *>(tool->rightContainer()), false, this);

	settingsButton = new GearBtn(this);
	infoBtn = new InfoBtn(this);
	runBtn = new RunBtn(this);
	clearBtn = new QPushButton("Clear", this);
	// TODO create a style for clear button
	StyleHelper::BlueButton(clearBtn);

	tool->addWidgetToTopContainerMenuControlHelper(openLatMenuBtn, TTA_RIGHT);
	tool->addWidgetToTopContainerMenuControlHelper(settingsButton, TTA_LEFT);

	tool->addWidgetToTopContainerHelper(runBtn, TTA_RIGHT);
	tool->addWidgetToTopContainerHelper(clearBtn, TTA_RIGHT);

	grp = static_cast<OpenLastMenuBtn *>(openLatMenuBtn)->getButtonGroup();
	grp->addButton(settingsButton);

	// channel monitors layout
	m_scrollArea = new QScrollArea(this);
	m_scrollArea->setWidgetResizable(true);
	m_flexGridLayout = new FlexGridLayout(100, this);
	m_scrollArea->setWidget(m_flexGridLayout);
	tool->addWidgetToCentralContainerHelper(m_scrollArea);

	initDataMonitor();

	QPushButton *addMonitorButton = new QPushButton(this);
	addMonitorButton->setText("+");
	// TODO add monitor button style
	StyleHelper::BlueButton(addMonitorButton);
	tool->addWidgetToTopContainerHelper(addMonitorButton, TTA_LEFT);

	connect(addMonitorButton, &QPushButton::clicked, this, &DataMonitorTool::addMonitor);
}

RunBtn *DataMonitorTool::getRunButton() const { return runBtn; }

void DataMonitorTool::initDataMonitor()
{
	tool->addWidgetToTopContainerHelper(infoBtn, TTA_LEFT);

	m_monitorControllers = new QMap<int, DataMonitorController *>();

	m_dataAcquisitionManager = new DataAcquisitionManager(this);
	// TODO read strategy will be replaced to DMM read strategy
	m_dataAcquisitionManager->setReadStrategy(new TestReadStrategy());
	// TODO timer will be handled from general settings time interval
	m_dataAcquisitionManager->setTimer(new QTimer());
	m_dataAcquisitionManager->setTimerInterval(1000);

	connect(runBtn, &QAbstractButton::toggled, m_dataAcquisitionManager, [=](bool toggled) {
		if(toggled) {
			m_dataAcquisitionManager->startTimer();
		} else {
			m_dataAcquisitionManager->stopTimer();
		}
	});

	connect(clearBtn, &QPushButton::clicked, m_dataAcquisitionManager,
		[=]() { m_dataAcquisitionManager->getDataMonitorHanlder()->clearMonitorsData(); });

	// TODO get a list of DataMonitorModel for each device we want to add
	// ?? use de generateMonitor function outside the tool ? tool->generateMonitor( monitor data ) ;
	for(int i = 0; i < 4; i++) {
		DataMonitorModel *dataModel = new DataMonitorModel("test:" + QString::number(i),
								   StyleHelper::getColor("CH" + QString::number(i)),
								   new UnitOfMeasurement("Volt", "V"));

		m_dataAcquisitionManager->getDataMonitorHanlder()->addMonitor(dataModel);
	}
}

void DataMonitorTool::addMonitor()
{
	DataMonitorController *monitorController = new DataMonitorController(this);
	monitorController->setMonitors(m_dataAcquisitionManager->getDataMonitorHanlder());

	int controllerId = m_flexGridLayout->addQWidgetToList(monitorController->getDataMonitorView());
	m_flexGridLayout->addWidget(controllerId);

	m_monitorControllers->insert(controllerId, monitorController);

	tool->rightStack()->add(QString::number(controllerId), monitorController->getDataMonitorSettings());

	connect(monitorController->getDataMonitorView(), &DataMonitorView::widgetClicked, this, [=]() {
		tool->openRightContainerHelper(true);
		tool->requestMenu(QString::number(controllerId));

		if(m_monitorControllers->contains(activeMonitor)) {
			m_monitorControllers->value(activeMonitor)->getDataMonitorView()->toggleSelected();
		}
		activeMonitor = controllerId;
		m_monitorControllers->value(activeMonitor)->getDataMonitorView()->toggleSelected();
	});

	monitorController->getDataMonitorSettings()->addMonitorsList(
		m_dataAcquisitionManager->getDataMonitorHanlder()->getMonitors());

	connect(monitorController->getDataMonitorSettings(), &DataMonitorSettings::removeMonitor, [=]() {
		m_flexGridLayout->removeWidget(controllerId);
		tool->rightStack()->remove(QString::number(controllerId));
		tool->openRightContainerHelper(false);
		m_monitorControllers->remove(controllerId);
		delete monitorController;
	});

	// settings active monitors update
	connect(monitorController->getDataMonitorSettings(), &DataMonitorSettings::monitorToggled,
		m_dataAcquisitionManager, &DataAcquisitionManager::updateActiveMonitors);

	/// TODO
	// connect tool general settings signals
}
