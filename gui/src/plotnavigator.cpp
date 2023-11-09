#include <plotnavigator.hpp>

using namespace scopy;

PlotNavigator::PlotNavigator(QwtPlot *plot)
	: m_plot(plot)
	, m_magnifier(nullptr)
	, m_zoomer(nullptr)
{

}

PlotNavigator::~PlotNavigator()
{
	if (!m_magnifier) {
		delete m_magnifier;
	}
	if (!m_zoomer) {
		delete m_zoomer;
	}
}

QwtPlot *PlotNavigator::plot() { return m_plot; }

void PlotNavigator::initMagnifier()
{
	m_magnifier = new MousePlotMagnifier(plot()->canvas());

//	m_magnifier->setAxisEnabled(QwtAxis::YLeft, false);
//	connect(m_magnifier, &MousePlotMagnifier::reset, this, [=](){
//		d_zoomer[0]->zoom(0);
//	});
}

void PlotNavigator::setMagnifierEnabled(bool en)
{
	if (!m_magnifier && en) {
		initMagnifier();
		m_magnifier->setEnabled(true);
	} else if(m_magnifier) {
		m_magnifier->setEnabled(en);
	}
}

MousePlotMagnifier *PlotNavigator::getMagnifier()
{
	return m_magnifier;
}

void PlotNavigator::initZoomer()
{
//	m_zoomer = new BoundedPlotZoomer(plot()->canvas());
}

void PlotNavigator::setZoomerEnabled(bool en)
{
	if (!m_zoomer && en) {
		initZoomer();
		m_zoomer->setEnabled(true);
	} else if(m_zoomer) {
		m_zoomer->setEnabled(en);
	}
}

BoundedPlotZoomer *PlotNavigator::getZoomer()
{
	return m_zoomer;
}
