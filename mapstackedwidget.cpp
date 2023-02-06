#include "mapstackedwidget.h"
#include <QDebug>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(CAT_MAPSTACKWIDGET, "MapStackedWidget")

using namespace adiscope;

MapStackedWidget::MapStackedWidget(QWidget *parent) :
	QStackedWidget(parent)
{

}

MapStackedWidget::~MapStackedWidget()
{

}

void MapStackedWidget::add(QString key, QWidget *w)
{
	map[key] = w;
	addWidget(w);
	qDebug(CAT_MAPSTACKWIDGET) << key <<"added to stack";
}

bool MapStackedWidget::remove(QString key)
{
	QWidget *w = map[key];
	if(w) {
		removeWidget(map[key]);
		map.remove(key);
	} else {
		qWarning(CAT_MAPSTACKWIDGET)<<key<< "not found in MapStackWidget. cannot remove";
		return false;
	}
	qDebug(CAT_MAPSTACKWIDGET) << key <<"removed from to stack";
	return true;
}

bool MapStackedWidget::show(QString key)
{
	QWidget *w = map[key];
	if(w)	{
		if(indexOf(map[key]) != -1) {
			setCurrentWidget(w);
			qDebug(CAT_MAPSTACKWIDGET)<<key<<" found - showing";
			return true;
		} else {
			qWarning(CAT_MAPSTACKWIDGET)<<key<<" found in stack but not found in qwidgetstack";
		}
	} else {
		qWarning(CAT_MAPSTACKWIDGET)<<key<<"not found in MapStackWidget. cannot show";
	}
	return false;
}
