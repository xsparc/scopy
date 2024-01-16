#include "datamonitorview.hpp"

#include "dynamicWidget.h"

#include <src/datamonitorstylehelper.hpp>

#include <stylehelper.h>

using namespace scopy;
using namespace datamonitor;

DataMonitorView::DataMonitorView(DataMonitorModel *dataMonitorModel, QWidget *parent)
	: QFrame{parent}
{}

DataMonitorView::DataMonitorView(QWidget *parent) { init(); }

void DataMonitorView::init()
{
	installEventFilter(this);

	QVBoxLayout *layout = new QVBoxLayout(this);
	setLayout(layout);

	header = new QWidget(this);
	layout->addWidget(header);
	QHBoxLayout *headerLayout = new QHBoxLayout(header);
	header->setLayout(headerLayout);

	measureUnit = new UnitOfMeasurement("", "");

	measuringUnit = new QLabel("", header);
	title = new QLineEdit("DataMonitor", header);
	connect(title, &QLineEdit::textChanged, this, &DataMonitorView::titleChanged);

	headerLayout->addWidget(measuringUnit, 1, Qt::AlignLeft);
	headerLayout->addWidget(title, 1, Qt::AlignRight);

	monitorPlot = new MonitorPlot(this);

	layout->addWidget(monitorPlot);

	valueWidget = new QWidget(this);
	QHBoxLayout *valueLayout = new QHBoxLayout(valueWidget);
	valueWidget->setLayout(valueLayout);

	value = new LcdNumber();
	valueLayout->addWidget(value);
	valueUMLabel = new QLabel("");
	valueLayout->addWidget(valueUMLabel);
	layout->addWidget(valueWidget);

	minValueWidget = new QWidget(this);
	QHBoxLayout *minValueLayout = new QHBoxLayout(minValueWidget);
	minValueWidget->setLayout(minValueLayout);

	QLabel *minValueLabel = new QLabel("MIN");
	minValueLayout->addWidget(minValueLabel);
	minValue = new LcdNumber();
	minValueLayout->addWidget(minValue);
	minValueUMLabel = new QLabel("");
	minValueLayout->addWidget(minValueUMLabel);

	layout->addWidget(minValueWidget);

	maxValueWidget = new QWidget(this);
	QHBoxLayout *maxValueLayout = new QHBoxLayout(maxValueWidget);
	maxValueWidget->setLayout(maxValueLayout);

	QLabel *maxValueLabel = new QLabel("MAX");
	maxValueLayout->addWidget(maxValueLabel);
	maxValue = new LcdNumber();
	maxValueLayout->addWidget(maxValue);
	maxValueUMLabel = new QLabel("");
	maxValueLayout->addWidget(maxValueUMLabel);

	layout->addWidget(maxValueWidget);

	updatePrecision(3);
	DataMonitorStyleHelper::DataMonitorViewStyle(this, StyleHelper::getColor("ScopyBlue"));
}

void DataMonitorView::configureMonitor(DataMonitorModel *dataMonitorModel)
{
	mainMonitor = dataMonitorModel->getTitle();
	setMeasureUnit(dataMonitorModel->getUnitOfMeasure());

	if(mainMonitor != "") {
		disconnect(valueUpdateConnection);
		disconnect(minValueUpdateConnection);
		disconnect(maxValueUpdateConnection);

		monitorPlot->setMainMonitor(mainMonitor);
	}

	valueUpdateConnection =
		connect(dataMonitorModel, &DataMonitorModel::valueUpdated, this, &DataMonitorView::updateValue);

	minValueUpdateConnection =
		connect(dataMonitorModel, &DataMonitorModel::minValueUpdated, this, &DataMonitorView::updateMinValue);

	maxValueUpdateConnection =
		connect(dataMonitorModel, &DataMonitorModel::maxValueUpdated, this, &DataMonitorView::updateMaxValue);

	// if any values are available update current values based on available values
	// when data aquisition is off will still be able to see last read value of the monitor
	if(!dataMonitorModel->getYdata()->isEmpty()) {
		updateValue(dataMonitorModel->getYdata()->last());
		updateMinValue(dataMonitorModel->minValue());
		updateMaxValue(dataMonitorModel->maxValue());
	}

	DataMonitorStyleHelper::DataMonitorViewStyle(this, dataMonitorModel->getColor());
}

void DataMonitorView::setMeasureUnit(UnitOfMeasurement *newMeasureUnit)
{
	measureUnit = newMeasureUnit;
	measuringUnit->setText(measureUnit->getNameAndSymbol());
	valueUMLabel->setText(measureUnit->getSymbol());
	minValueUMLabel->setText(measureUnit->getSymbol());
	maxValueUMLabel->setText(measureUnit->getSymbol());
}

QString DataMonitorView::getTitle() { return title->text(); }

void DataMonitorView::setSelected(bool selected) { scopy::setDynamicProperty(this, "is_selected", selected); }

void DataMonitorView::toggleSelected()
{
	isSelected = !isSelected;
	setSelected(isSelected);
}

QWidget *DataMonitorView::getHeader() const { return header; }

QWidget *DataMonitorView::getValueWidget() const { return value; }

QWidget *DataMonitorView::getMinValueWidget() const { return minValueWidget; }

QWidget *DataMonitorView::getMaxValueWidget() const { return maxValueWidget; }

void DataMonitorView::updateValue(double value) { this->value->display(value); }

void DataMonitorView::updateMinValue(double value)
{
	if(value == Q_INFINITY) {
		this->minValue->display(0);
	} else {
		this->minValue->display(value);
	}
}

void DataMonitorView::updateMaxValue(double value)
{
	if(value == -Q_INFINITY) {
		this->maxValue->display(0);
	} else {
		this->maxValue->display(value);
	}
}

void DataMonitorView::updatePrecision(int precision)
{
	value->setPrecision(precision);
	value->setDigitCount(4 + precision);
	value->display(value->value());

	minValue->setPrecision(precision);
	minValue->setDigitCount(4 + precision);
	minValue->display(minValue->value());

	maxValue->setPrecision(precision);
	maxValue->setDigitCount(4 + precision);
	maxValue->display(maxValue->value());
}

bool DataMonitorView::eventFilter(QObject *watched, QEvent *event)
{
	if(event->type() == QEvent::MouseButtonPress) {
		Q_EMIT widgetClicked();
	}

	return QWidget::eventFilter(watched, event);
}

QString DataMonitorView::getMainMonitor() const { return mainMonitor; }

MonitorPlot *DataMonitorView::getMonitorPlot() const { return monitorPlot; }

void DataMonitorView::togglePeakHolder(bool toggle)
{
	getMinValueWidget()->setVisible(toggle);
	getMaxValueWidget()->setVisible(toggle);
}

void DataMonitorView::togglePlot(bool toggle) { monitorPlot->setVisible(toggle); }
