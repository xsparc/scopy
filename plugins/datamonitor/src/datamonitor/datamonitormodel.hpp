#ifndef DATAMONITORMODEL_H
#define DATAMONITORMODEL_H

#include "unitofmeasurement.hpp"

#include <QColor>
#include <QObject>

namespace scopy {
namespace datamonitor {

class DataMonitorModel : public QObject
{

	Q_OBJECT
public:
	explicit DataMonitorModel(QString title, QColor color, UnitOfMeasurement *unitOfMeasure,
				  QObject *parent = nullptr);

	QString getTitle() const;
	void setTitle(QString newTitle);

	QColor getColor() const;
	void setColor(QColor newColor);

	UnitOfMeasurement *getUnitOfMeasure() const;
	void setUnitOfMeasure(UnitOfMeasurement *newUnitOfMeasure);

	void updateValue(double time, double value);

	void resetMinMax();
	void clearMonitorData();

	QVector<double> *getXdata();
	QVector<double> *getYdata();

	double minValue() const;
	double maxValue() const;
	double getLastReadValue() const;

	double getValueAtTime(double time);

Q_SIGNALS:
	void valueUpdated(double time, double value);
	void minValueUpdated(double value);
	void maxValueUpdated(double value);
	void dataCleared();

private:
	QString title;
	QColor color;
	UnitOfMeasurement *unitOfMeasure;
	double m_lastReadValue;
	double m_minValue;
	double m_maxValue;
	QVector<double> ydata;
	QVector<double> xdata;
	void checkMinMaxUpdate(double value);
};
} // namespace datamonitor
} // namespace scopy
#endif // DATAMONITORMODEL_H
