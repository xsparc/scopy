#include "datamonitormodel.hpp"

using namespace scopy;
using namespace datamonitor;

DataMonitorModel::DataMonitorModel(QString title, QColor color, UnitOfMeasurement *unitOfMeasure, QObject *parent)
	: title(title)
	, color(color)
	, unitOfMeasure(unitOfMeasure)
	, m_minValue(Q_INFINITY)
	, m_maxValue(-Q_INFINITY)
	, QObject{parent}
{
	m_lastReadValue = 0;

	// TODO use size from pref
	int size = 10000;
	ydata.reserve(size);
	xdata.reserve(size);
}

QString DataMonitorModel::getTitle() const { return title; }

void DataMonitorModel::setTitle(QString newTitle) { title = newTitle; }

QColor DataMonitorModel::getColor() const { return color; }

void DataMonitorModel::setColor(QColor newColor) { color = newColor; }

UnitOfMeasurement *DataMonitorModel::getUnitOfMeasure() const { return unitOfMeasure; }

void DataMonitorModel::setUnitOfMeasure(UnitOfMeasurement *newUnitOfMeasure) { unitOfMeasure = newUnitOfMeasure; }

double DataMonitorModel::getLastReadValue() const { return m_lastReadValue; }

void DataMonitorModel::clearMonitorData()
{

	xdata.erase(xdata.begin(), xdata.end());
	ydata.erase(ydata.begin(), ydata.end());

	resetMinMax();

	Q_EMIT dataCleared();
}

QVector<double> *DataMonitorModel::getXdata() { return &xdata; }

QVector<double> *DataMonitorModel::getYdata() { return &ydata; }

double DataMonitorModel::minValue() const { return m_minValue; }

double DataMonitorModel::maxValue() const { return m_maxValue; }

double DataMonitorModel::getValueAtTime(double time) { return ydata.at(xdata.indexOf(time)); }

void DataMonitorModel::checkMinMaxUpdate(double value)
{
	if(value < m_minValue) {
		m_minValue = value;
		Q_EMIT minValueUpdated(value);
	}
	if(value > m_maxValue) {
		m_maxValue = value;
		Q_EMIT maxValueUpdated(value);
	}
}

void DataMonitorModel::updateValue(double time, double value)
{
	m_lastReadValue = value;
	xdata.push_back(time);
	ydata.push_back(value);
	checkMinMaxUpdate(value);
	Q_EMIT valueUpdated(time, value);
}

void DataMonitorModel::resetMinMax()
{
	m_minValue = Q_INFINITY;
	Q_EMIT minValueUpdated(m_minValue);
	m_maxValue = -Q_INFINITY;
	Q_EMIT maxValueUpdated(m_maxValue);
}
