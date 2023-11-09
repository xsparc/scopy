#ifndef PLOTNAVIGATOR_H
#define PLOTNAVIGATOR_H
#include "scopy-gui_export.h"

#include <QObject>
#include <QwtPlot>
#include <boundedplotzoomer.hpp>
#include <mouseplotmagnifier.hpp>

namespace scopy {

class SCOPY_GUI_EXPORT PlotNavigator : public QObject
{
	Q_OBJECT

public:
	explicit PlotNavigator(QwtPlot *plot);
	~PlotNavigator();

	QwtPlot *plot();

	MousePlotMagnifier *getMagnifier();
	void initMagnifier();
	void setMagnifierEnabled(bool en);

	BoundedPlotZoomer *getZoomer();
	void initZoomer();
	void setZoomerEnabled(bool en);

private:
	QwtPlot *m_plot;
	MousePlotMagnifier* m_magnifier;
	BoundedPlotZoomer *m_zoomer;

};
} // namespace scopy

#endif // PLOTNAVIGATOR_H
