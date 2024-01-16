#include "monitorplot.hpp"

#include <cursorcontroller.h>
#include <plotinfo.h>

using namespace scopy;
using namespace datamonitor;

MonitorPlot::MonitorPlot(QWidget *parent)
	: QWidget{parent}
{
	installEventFilter(this);

	QVBoxLayout *layout = new QVBoxLayout(this);
	setLayout(layout);

	m_plot = new PlotWidget(this);
	// TODO set this
	m_plot->xAxis()->setInterval(0, 100);

	m_plotInfo = new TimePlotInfo(m_plot, this);

	layout->addWidget(m_plotInfo);
	layout->addWidget(m_plot);

	m_monitorCurves = new QMap<QString, PlotData *>();
}

PlotWidget *MonitorPlot::plot() const { return m_plot; }

void MonitorPlot::addMonitor(DataMonitorModel *dataMonitorModel)
{
	QPen chpen = QPen(dataMonitorModel->getColor(), 1);
	PlotAxis *chPlotAxis = new PlotAxis(QwtAxis::YLeft, m_plot, chpen);
	PlotChannel *plotch =
		new PlotChannel(dataMonitorModel->getTitle(), chpen, m_plot, m_plot->xAxis(), chPlotAxis, this);
	plotch->setEnabled(true);

	PlotData *pd = new PlotData();
	pd->plotch = plotch;
	pd->xdata = dataMonitorModel->getXdata();
	pd->ydata = dataMonitorModel->getYdata();

	// TODO Get size from pref
	int size = 10000;

	// TODO CLEAR PLOT ON CLEAR DATA
	connect(dataMonitorModel, &DataMonitorModel::dataCleared, this, [=]() {
		pd->xdata = dataMonitorModel->getXdata();
		pd->ydata = dataMonitorModel->getYdata();
		plotch->curve()->setRawSamples(pd->ydata->data(), pd->xdata->data(), size);
		m_plot->replot();
	});

	plotch->curve()->setRawSamples(pd->ydata->data(), pd->xdata->data(), size);

	m_monitorCurves->insert(dataMonitorModel->getTitle(), pd);

	connect(dataMonitorModel, &DataMonitorModel::valueUpdated, m_plot, [=]() { m_plot->replot(); });

	m_plot->replot();
}

void MonitorPlot::removeMonitor(QString monitorTitle)
{
	m_plot->removePlotChannel(m_monitorCurves->value(monitorTitle)->plotch);
	m_monitorCurves->remove(monitorTitle);
	delete m_monitorCurves->value(monitorTitle);
}

void MonitorPlot::toggleMonitor(bool toggled, QString monitorTitle)
{
	if(m_monitorCurves->contains(monitorTitle)) {
		m_monitorCurves->value(monitorTitle)->plotch->setEnabled(toggled);
		m_plot->replot();
	}
}

bool MonitorPlot::hasMonitor(QString title) { return m_monitorCurves->contains(title); }

bool MonitorPlot::firstMonitor() const { return m_firstMonitor; }

void MonitorPlot::setFirstMonitor(bool newFirstMonitor) { m_firstMonitor = newFirstMonitor; }

void MonitorPlot::changeCurveStyle(QString plotCurve, int style)
{
	if(m_monitorCurves->contains(plotCurve)) {

		auto curve = m_monitorCurves->value(plotCurve)->plotch->curve();

		curve->setPaintAttribute(QwtPlotCurve::ClipPolygons, true);
		curve->setCurveAttribute(QwtPlotCurve::Fitted, false);

		switch(style) {
		case 0:
			curve->setStyle(QwtPlotCurve::CurveStyle::Lines);
			m_plot->replot();
			break;
		case 1:
			curve->setStyle(QwtPlotCurve::CurveStyle::Dots);
			m_plot->replot();
			break;
		case 2:
			curve->setStyle(QwtPlotCurve::CurveStyle::Steps);
			m_plot->replot();
			break;
		case 3:
			curve->setStyle(QwtPlotCurve::CurveStyle::Sticks);
			m_plot->replot();
			break;
		case 4:
			curve->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
			curve->setCurveAttribute(QwtPlotCurve::Fitted, true);
			curve->setStyle(QwtPlotCurve::CurveStyle::Lines);
			m_plot->replot();
			break;
		}
	}
}

void MonitorPlot::setMainMonitor(QString newMainMonitor)
{
	if(m_monitorCurves->contains(newMainMonitor)) {
		m_monitorCurves->value(newMainMonitor)->plotch->detach();
		m_monitorCurves->value(newMainMonitor)->plotch->attach();
	}
}
