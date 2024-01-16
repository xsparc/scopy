#ifndef DATAMONITORVIEW_H
#define DATAMONITORVIEW_H

#include "datamonitormodel.hpp"
#include "monitorplot.hpp"

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <plotwidget.h>
#include <plotaxis.h>
#include <lcdNumber.hpp>
#include <QLineEdit>

namespace scopy {
namespace datamonitor {

class DataMonitorView : public QFrame
{
	friend class DataMonitorStyleHelper;
	Q_OBJECT
public:
	explicit DataMonitorView(DataMonitorModel *dataMonitorModel, QWidget *parent = nullptr);

	DataMonitorView(QWidget *parent = nullptr);

	void init();

	QWidget *getHeader() const;
	QWidget *getValueWidget() const;
	QWidget *getMinValueWidget() const;
	QWidget *getMaxValueWidget() const;
	void updateValue(double value);
	void updateMinValue(double value);
	void updateMaxValue(double value);
	void updatePrecision(int precision);

	void togglePeakHolder(bool toggle);
	void togglePlot(bool toggle);

	MonitorPlot *getMonitorPlot() const;
	void configureMonitor(DataMonitorModel *dataMonitorModel);

	void setMeasureUnit(UnitOfMeasurement *newMeasureUnit);

	QString getTitle();

	void setSelected(bool selected);
	void toggleSelected();

	QString getMainMonitor() const;

Q_SIGNALS:
	void widgetClicked();
	void titleChanged(QString newTitle);

private:
	bool isSelected = false;
	QLabel *measuringUnit;
	QLineEdit *title;
	QWidget *header;
	QWidget *valueWidget;
	QWidget *minValueWidget;
	QWidget *maxValueWidget;
	LcdNumber *value;
	QLabel *valueUMLabel;
	LcdNumber *minValue;
	QLabel *minValueUMLabel;
	LcdNumber *maxValue;
	QLabel *maxValueUMLabel;

	UnitOfMeasurement *measureUnit;

	bool eventFilter(QObject *watched, QEvent *event) override;

	//////monitorPlot
	MonitorPlot *monitorPlot;

	QString mainMonitor = "";
	QMetaObject::Connection valueUpdateConnection;
	QMetaObject::Connection minValueUpdateConnection;
	QMetaObject::Connection maxValueUpdateConnection;
};
} // namespace datamonitor
} // namespace scopy
#endif // DATAMONITORVIEW_H
