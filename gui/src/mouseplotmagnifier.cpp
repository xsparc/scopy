#include <qwt_interval.h>
#include <qwt_scale_engine.h>
#include <iostream>
#include <mouseplotmagnifier.hpp>
#include <DisplayPlot.h>

using namespace scopy;

MousePlotMagnifier::MousePlotMagnifier(QWidget *canvas)
	: QwtPlotMagnifier(canvas)
	, m_cursorPos(QPoint())
	, m_zoomBase(QRectF())
	, m_xAxis(QwtAxis::XBottom)
	, m_yAxis(QwtAxis::YLeft)
	, m_xAxisEn(true)
	, m_yAxisEn(true)
	, m_isZoomed(false)
	, m_en(false)
	, m_bounded(true)
	, m_logarithmic(false)
{
	setWheelModifiers(Qt::NoModifier);
	setMouseButton(Qt::NoButton);
	installEventFilter(canvas);

	connect(this, &MousePlotMagnifier::reset, this, &MousePlotMagnifier::zoomToBase);
}

MousePlotMagnifier::~MousePlotMagnifier() {}

void MousePlotMagnifier::setAxisEnabled(QwtAxisId axisId, bool on)
{
	if(QwtAxis::isXAxis(axisId)) {
		m_xAxis = axisId;
		m_xAxisEn = on;
	}
	if(QwtAxis::isYAxis(axisId)) {
		m_yAxis = axisId;
		m_yAxisEn = on;
	}
}

void MousePlotMagnifier::setLogarithmic(bool log) { m_logarithmic = log; }

bool MousePlotMagnifier::isLogarithmic() { return m_logarithmic; }

QRectF MousePlotMagnifier::zoomBase() const
{
	return QRectF(m_zoomBase);
}

bool MousePlotMagnifier::isAxisEnabled(QwtAxisId axisId) const
{
	return (m_xAxis == axisId && m_xAxisEn) || (m_yAxis == axisId && m_yAxisEn);
}

void MousePlotMagnifier::setZoomBase(const QRectF &rect) {
	m_zoomBase = rect;
}

void MousePlotMagnifier::setAxes(QwtAxisId xAxis, QwtAxisId yAxis)
{
	m_xAxis = xAxis;
	m_yAxis = yAxis;
}

void MousePlotMagnifier::setEnabled(bool en) { m_en = en; }

bool MousePlotMagnifier::isEnabled() { return m_en; }

void MousePlotMagnifier::silentZoom(double factor, QPointF cursorPos)
{
	blockSignals(true);
	zoom(factor, cursorPos);
	blockSignals(false);
}

void MousePlotMagnifier::zoom(double factor, QPointF cursorPos)
{
	if(factor == -1) {
		zoomToBase();
	} else {
		m_cursorPos = cursorPos;
		rescale(factor);
	}
}

void MousePlotMagnifier::setBounded(bool en) { m_bounded = en; }

void MousePlotMagnifier::widgetWheelEvent(QWheelEvent *wheelEvent)
{
	m_cursorPos = wheelEvent->position();
	QwtPlotMagnifier::widgetWheelEvent(wheelEvent);
}

bool MousePlotMagnifier::eventFilter(QObject *object, QEvent *event)
{
	if(event->type() == QEvent::MouseButtonRelease) {
		QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);

		if(mouseEvent->button() == Qt::MouseButton::RightButton && m_isZoomed && m_en) {
			Q_EMIT reset();
			m_isZoomed = false;
		}
	}

	return QwtPlotMagnifier::eventFilter(object, event);
}

void MousePlotMagnifier::rescale(double factor)
{
	DisplayPlot *plt = dynamic_cast<DisplayPlot*>(plot());
	double tmp_factor = qAbs(factor);

	if(plt == nullptr || !m_en || tmp_factor == 1.0 || tmp_factor == 0.0)
		return;

	bool doReplot = false;
	const bool autoReplot = plt->autoReplot();
	m_isZoomed = true;

	std::cout << tmp_factor << std::endl;

	tmp_factor = 1 / tmp_factor; // invert zoom in/out bindings

	plt->setAutoReplot(false);

	std::cout << "before:" << plt->axisInterval(QwtAxis::XBottom).width() << std::endl;

	for(QwtAxisId axisId : {m_xAxis, m_yAxis}) {
		if(isAxisEnabled(axisId)) {
			const QwtScaleMap scaleMap = plt->canvasMap(axisId);
			double v1 = scaleMap.s1(); // v1 is the bottom value of the axis scale
			double v2 = scaleMap.s2(); // v2 is the top value of the axis scale

			if(scaleMap.transformation() && !isLogarithmic()) {
				// the coordinate system of the paint device is always linear
				v1 = scaleMap.transform(v1);
				v2 = scaleMap.transform(v2);
			}

			double cursor = 0; // position of the cursor in the axis coordinates
			if(QwtAxis::isXAxis(axisId) && m_xAxisEn)
				cursor = scaleMap.invTransform(m_cursorPos.x());
			if(QwtAxis::isYAxis(axisId) && m_yAxisEn)
				cursor = scaleMap.invTransform(m_cursorPos.y());

			const double center = 0.5 * (v1 + v2);
			const double width_2 = 0.5 * (v2 - v1) * tmp_factor;
			const double newCenter = cursor - tmp_factor * (cursor - center);
			v1 = newCenter - width_2;
			v2 = newCenter + width_2;

			if(scaleMap.transformation() && !isLogarithmic()) {
				v1 = scaleMap.invTransform(v1);
				v2 = scaleMap.invTransform(v2);
			}

			// limit zoom to zoomBase
			if(QwtAxis::isXAxis(axisId) && m_bounded) {
				v1 = std::max(v1, m_zoomBase.left());
				v2 = std::min(v2, m_zoomBase.right());
			}
			if(QwtAxis::isYAxis(axisId) && m_bounded) {
				v1 = std::max(v1, m_zoomBase.top());
				v2 = std::min(v2, m_zoomBase.bottom());
			}

//			plt->setAxisScale(axisId, v1, v2);
			setPlotAxisScale(axisId, v1, v2);
			doReplot = true;
		}
	}
	plt->setAutoReplot(autoReplot);

	if(doReplot) {
		plt->replot();
		Q_EMIT zoomed(factor, m_cursorPos);
	}

	std::cout << "after:" << plt->axisInterval(QwtAxis::XBottom).width() << std::endl << std::endl;
}

void MousePlotMagnifier::setPlotAxisScale(QwtAxisId axisId, double min, double max)
{
	DisplayPlot *plt = dynamic_cast<DisplayPlot*>(plot());

	if(QwtAxis::isXAxis(axisId)) {
		double width = qAbs(max - min);
		plt->setHorizUnitsPerDiv(width / plt->xAxisNumDiv());
		plt->setHorizOffset(min + (width / 2));
	}
	if(QwtAxis::isYAxis(axisId)) {
		double height = qAbs(max - min);
		plt->setVertUnitsPerDiv(height / plt->yAxisNumDiv(), axisId.id);
		plt->setVertOffset(min + (height / 2), axisId.id);
	}
}

void MousePlotMagnifier::zoomToBase()
{
	if(isAxisEnabled(m_xAxis)) {
		setPlotAxisScale(m_xAxis, m_zoomBase.left(), m_zoomBase.right());
//		plot()->setAxisScale(m_xAxis, m_zoomBase.left(), m_zoomBase.right());
	}
	if(isAxisEnabled(m_yAxis)) {
		setPlotAxisScale(m_yAxis, m_zoomBase.top(), m_zoomBase.bottom());
//		plot()->setAxisScale(m_yAxis, m_zoomBase.top(), m_zoomBase.bottom());
	}

	plot()->replot();
	Q_EMIT zoomed(-1, QPoint());
}
