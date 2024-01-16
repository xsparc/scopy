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
						.scopy--datamonitor--DataMonitorView:hover {
							border: 1px solid &&hoverBackground&& ;
							border-radius: 4px;

						}

						*[is_selected=true] {
							border: 1px solid &&hoverBackground&& ;
							border-radius: 4px;
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
	style.replace("&&hoverBackground&&", StyleHelper::getColor("LabelText"));
	style.replace("&&widgetColor&&", StyleHelper::getColor("LabelText"));
	style.replace("&&borderColor&&", StyleHelper::getColor("UIElementHighlight"));
	style.replace("&&backgroundColor&&", StyleHelper::getColor("UIElementBackground"));

	StyleHelper::MenuLargeLabel(dataMonitorView->measuringUnit);

	dataMonitorView->monitorPlot->setMinimumHeight(300);
	dataMonitorView->monitorPlot->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	dataMonitorView->value->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
	dataMonitorView->minValue->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
	dataMonitorView->maxValue->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);

	dataMonitorView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
	dataMonitorView->setMinimumWidth(500);

	dataMonitorView->setStyleSheet(style);
}

void DataMonitorStyleHelper::DataMonitorSettingsStyle(DataMonitorSettings *dataMonitorSettings)
{
	QString style = QString(R"css(
						QWidget {
							background-color : &&backgroundColor&& ;
						}
						)css");

	style.replace("&&backgroundColor&&", StyleHelper::getColor("Transparent"));

	StyleHelper::MenuComboWidget(dataMonitorSettings->plotSize);
	StyleHelper::BlueButton(dataMonitorSettings->peakHolderReset);
	StyleHelper::BlueButton(dataMonitorSettings->deleteMonitor);

	dataMonitorSettings->setStyleSheet(style);
}

void DataMonitorStyleHelper::DataMonitorToolSettingsStyle(DataMonitorToolSettings *dataMonitorToolSettings)
{
	QString style = QString(R"css(
						QWidget {
							background-color : &&backgroundColor&& ;
						}
						)css");

	style.replace("&&backgroundColor&&", StyleHelper::getColor("Transparent"));

	StyleHelper::SmallBlueButton(dataMonitorToolSettings->precisionBtn);
	StyleHelper::MenuSpinBox(dataMonitorToolSettings->recordingTimer);
	dataMonitorToolSettings->setStyleSheet(style);
}
