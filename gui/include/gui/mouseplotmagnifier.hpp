#ifndef MOUSEPLOTMAGNIFIER_H
#define MOUSEPLOTMAGNIFIER_H
#include "qevent.h"
#include "scopy-gui_export.h"

#include <QPoint>
#include <QwtPlotMagnifier>
#include <QwtPlot>
#include <qwt_scale_map.h>

namespace scopy {

class SCOPY_GUI_EXPORT MousePlotMagnifier : public QwtPlotMagnifier
{
	Q_OBJECT

public:
	explicit MousePlotMagnifier(QWidget *canvas);
	~MousePlotMagnifier();

	virtual void setAxisEnabled(QwtAxisId axisId, bool on);
	virtual bool isAxisEnabled(QwtAxisId axisId) const;
	void setZoomBase(const QRectF &rect);
	void setAxes(QwtAxisId xAxis, QwtAxisId yAxis);
	void setEnabled(bool en);
	bool isEnabled();
	void setBounded(bool en);
	void zoom(double factor, QPointF cursorPos);
	void silentZoom(double factor, QPointF cursorPos);
	void setLogarithmic(bool log);
	bool isLogarithmic();
	QRectF zoomBase() const;
Q_SIGNALS:
	void reset();
	void zoomed(double factor, QPointF cursorPos);

protected:
	void widgetWheelEvent(QWheelEvent *wheelEvent) override;
	bool eventFilter(QObject *object, QEvent *event) override;
	void rescale(double factor) override;

private Q_SLOTS:
	void zoomToBase();

private:
	QPointF m_cursorPos;
	QRectF m_zoomBase;
	QwtAxisId m_xAxis, m_yAxis;
	bool m_xAxisEn, m_yAxisEn;
	bool m_isZoomed;
	bool m_en;
	bool m_bounded;
	bool m_logarithmic;
	void setPlotAxisScale(QwtAxisId axisId, double min, double max);
};
} // namespace scopy

#endif // MOUSEPLOTMAGNIFIER_H
