#ifndef BOUNDEDPLOTZOOMER_H
#define BOUNDEDPLOTZOOMER_H

#include "scopy-gui_export.h"

#include <QwtPlotShapeItem>
#include <qwt_plot_zoomer.h>

namespace scopy {
class SCOPY_GUI_EXPORT BoundedPlotZoomer : public QwtPlotZoomer
{
	Q_OBJECT
public:
	BoundedPlotZoomer(QwtAxisId xAxis, QwtAxisId yAxis, QwtPlot *plot, bool doReplot = true);
	~BoundedPlotZoomer();

	void resetZoom();
	void popZoom();
	void setBounded(bool en);
	void setZoom(const QRectF &);

Q_SIGNALS:
	void zoomed();

protected:
	QPolygon adjustedPoints(const QPolygon &) const override;
	void zoom(const QRectF &) override;
	QSizeF minZoomSize() const override;

	void detachCornerMarkers() const;
	void detachExtendedMarkers() const;
	void detachMarkers() const;
	void drawMarker(QwtPlotShapeItem *marker, QPainterPath path) const;

private:
	void init();

private:
	bool m_bounded;

	QVector<QwtPlotShapeItem *> cornerMarkers;
	QVector<QwtPlotShapeItem *> extendMarkers;
};
} // namespace scopy

#endif // BOUNDEDPLOTZOOMER_H
