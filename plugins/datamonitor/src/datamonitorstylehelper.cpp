#include "datamonitorstylehelper.hpp"

#include <stylehelper.h>

using namespace scopy;
using namespace datamonitor;

DataMonitorStyleHelper::DataMonitorStyleHelper(QObject *parent)
	: QObject{parent}
{}

void DataMonitorStyleHelper::DataMonitorViewStyle(DataMonitorView *dataMonitorView, QColor color)
{
	QString style = QString(R"css(
						.scopy--datamonitor--DataMonitorView {
						margin : 16px ;
						padding: 16px ;

						}
						QWidget {
							color: &&widgetColor&& ;
							background-color : &&backgroundColor&& ;
						}
						QLCDNumber {
							color: &&LCDColor&& ;
							border : 0px ; }

						)css");

	style.replace("&&LCDColor&&", color.name());
	style.replace("&&widgetColor&&", StyleHelper::getColor("LabelText"));
	style.replace("&&borderColor&&", StyleHelper::getColor("UIElementHighlight"));
	style.replace("&&backgroundColor&&", StyleHelper::getColor("UIElementBackground"));

	StyleHelper::MenuLargeLabel(dataMonitorView->measuringUnit);
	StyleHelper::MenuMediumLabel(dataMonitorView->title);

	dataMonitorView->value->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
	dataMonitorView->minValue->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
	dataMonitorView->maxValue->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);

	dataMonitorView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	dataMonitorView->setStyleSheet(style);
}
