#include "boundedplotzoomer.hpp"
#include "qpainterpath.h"
#include <QStack>
#include <qwt_plot.h>
#include <qwt_scale_div.h>
#include <math.h>
#include <QwtPlot>

using namespace scopy;

BoundedPlotZoomer::BoundedPlotZoomer(QwtAxisId xAxis, QwtAxisId yAxis, QwtPlot *plot, bool doReplot)
	: QwtPlotZoomer(xAxis, yAxis, plot->canvas(), doReplot)
	, m_bounded(true)
{
	init();
}

BoundedPlotZoomer::~BoundedPlotZoomer()
{
	for(auto marker : extendMarkers) {
		delete marker;
	}
	for(auto marker : cornerMarkers) {
		delete marker;
	}
}

void BoundedPlotZoomer::init()
{
	setMaxStackDepth(5);

	extendMarkers.push_back(new QwtPlotShapeItem());
	extendMarkers.push_back(new QwtPlotShapeItem());

	cornerMarkers.push_back(new QwtPlotShapeItem());
	cornerMarkers.push_back(new QwtPlotShapeItem());
	cornerMarkers.push_back(new QwtPlotShapeItem());
	cornerMarkers.push_back(new QwtPlotShapeItem());
}

void BoundedPlotZoomer::detachCornerMarkers() const
{
	for(auto marker : cornerMarkers) {
		marker->detach();
	}
}

void BoundedPlotZoomer::detachExtendedMarkers() const
{
	for(auto marker : extendMarkers) {
		marker->detach();
	}
}

void BoundedPlotZoomer::detachMarkers() const
{
	detachCornerMarkers();
	detachExtendedMarkers();
}

void BoundedPlotZoomer::resetZoom() { QwtPlotZoomer::zoom(0); }

void BoundedPlotZoomer::popZoom() { QwtPlotZoomer::zoom(-1); }

void BoundedPlotZoomer::setBounded(bool en) { m_bounded = en; }

void BoundedPlotZoomer::zoom(const QRectF &rect)
{
	detachMarkers();

	QRectF boundedRect = rect;
	if(m_bounded) {
		QRectF boundedRect = rect & zoomBase();
	}

	QwtPlotZoomer::zoom(boundedRect);
	Q_EMIT zoomed();
}

void BoundedPlotZoomer::setZoom(const QRectF &rect) { zoom(rect); }

QSizeF BoundedPlotZoomer::minZoomSize() const
{
	const double eps = 10e12;
	return {zoomBase().width() / eps, zoomBase().height() / eps};
}

void BoundedPlotZoomer::drawMarker(QwtPlotShapeItem *marker, QPainterPath path) const
{
	marker->setShape(path);
	marker->setPen(Qt::white, 3, Qt::SolidLine);
	marker->setZ(1000);
	marker->attach((QwtPlot *)plot());
}

QPolygon BoundedPlotZoomer::adjustedPoints(const QPolygon &points) const
{
	if(points.size() < 2 || points[0] == points[1]) {
		detachCornerMarkers();
		return points;
	}

	const int offsetSize = 20;
	const int offsetDouble = offsetSize * 2;
	const int width = qAbs(points[1].x() - points[0].x());
	const int height = qAbs(points[1].y() - points[0].y());

	// extend by width
	if(width > offsetDouble && height <= offsetDouble) {
		if(yAxis() == QwtAxisId(QwtAxis::YLeft, 0)) {
			QPainterPath path;
			detachCornerMarkers();

			for(int i = 0; i < points.size(); i++) {
				path.moveTo(invTransform(QPoint(points[0].x() - offsetDouble, points[i].y())));
				path.lineTo(invTransform(QPoint(points[0].x() + offsetDouble, points[i].y())));
				drawMarker(extendMarkers[i], path);
			}
			((QwtPlot *)plot())->replot();
		}

		return QPolygon(QRect(QPoint(0, points[0].y()), QPoint(canvas()->width() - 1, points[1].y())));
	}

	// extend by height
	if(width <= offsetDouble && height > offsetDouble) {
		if(yAxis() == QwtAxisId(QwtAxis::YLeft, 0)) {
			QPainterPath path;
			detachCornerMarkers();

			for(int i = 0; i < points.size(); i++) {
				path.moveTo(invTransform(QPoint(points[i].x(), points[0].y() - offsetDouble)));
				path.lineTo(invTransform(QPoint(points[i].x(), points[0].y() + offsetDouble)));
				drawMarker(extendMarkers[i], path);
			}
			((QwtPlot *)plot())->replot();
		}
		return QPolygon(QRect(QPoint(points[0].x(), 0), QPoint(points[1].x(), canvas()->height() - 1)));
	}

	if(yAxis() == QwtAxisId(QwtAxis::YLeft, 0)) {
		detachExtendedMarkers();

		QVector<QPainterPath> path;
		int offsetX = points[0].x() < points[1].x() ? offsetSize : -offsetSize;
		int offsetY = points[0].y() < points[1].y() ? offsetSize : -offsetSize;

		path.append(QPainterPath());
		path[0].moveTo(invTransform(QPoint(points[0].x(), points[0].y() + offsetY)));
		path[0].lineTo(invTransform(points[0]));
		path[0].lineTo(invTransform(QPoint(points[0].x() + offsetX, points[0].y())));

		path.append(QPainterPath());
		path[1].moveTo(invTransform(QPoint(points[1].x(), points[0].y() + offsetY)));
		path[1].lineTo(invTransform(QPoint(points[1].x(), points[0].y())));
		path[1].lineTo(invTransform(QPoint(points[1].x() - offsetX, points[0].y())));

		path.append(QPainterPath());
		path[2].moveTo(invTransform(QPoint(points[1].x(), points[1].y() - offsetY)));
		path[2].lineTo(invTransform(QPoint(points[1].x(), points[1].y())));
		path[2].lineTo(invTransform(QPoint(points[1].x() - offsetX, points[1].y())));

		path.append(QPainterPath());
		path[3].moveTo(invTransform(QPoint(points[0].x(), points[1].y() - offsetY)));
		path[3].lineTo(invTransform(QPoint(points[0].x(), points[1].y())));
		path[3].lineTo(invTransform(QPoint(points[0].x() + offsetX, points[1].y())));

		for(int i = 0; i < cornerMarkers.size(); i++) {
			drawMarker(cornerMarkers[i], path[i]);
		}
		((QwtPlot *)plot())->replot();
	}
	return points;
}
