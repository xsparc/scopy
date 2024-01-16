#ifndef DATAMONITORSETTINGS_H
#define DATAMONITORSETTINGS_H

#include <QPushButton>
#include <QWidget>
#include <menucombo.h>
#include <menuheader.h>
#include <menuonoffswitch.h>

namespace scopy {
namespace datamonitor {
class DataMonitorSettings : public QWidget
{
	friend class DataMonitorStyleHelper;
	Q_OBJECT
public:
	explicit DataMonitorSettings(QWidget *parent = nullptr);
	~DataMonitorSettings();

	void init(QString title, QColor color);

public Q_SLOTS:
	void scaleToggle(bool toggled);
	void peakHolderToggle(bool toggled);
	void plotToggle(bool toggled);
	void peakHolderResetClicked();
	void plotSizeChanged(int size);
	void changeLineStyle(int index);
	void addMonitorsList(QList<QString> monitoList);
	void updateTitle(QString title);
	void updateMainMonitor(QString monitorTitle);
	QList<QString> *getActiveMonitors();

Q_SIGNALS:
	void toggleScale(bool toggle);
	void monitorColorChanged(QString color);
	void togglePlot(bool toggled);
	void changeHistorySize(int size);
	void lineStyleChanged(Qt::PenStyle lineStyle);
	void lineStyleIndexChanged(int index);
	void plotSizeIndexChanged(int index);
	void resetPeakHolder();
	void togglePeakHolder(bool toggled);
	void toggleAll(bool toggle);
	void monitorToggled(bool toggled, QString monitorTitle);
	void removeMonitor();
	void mainMonitorChanged(QString monitorTitle);

private:
	double numSamplesFromIdx(int idx);
	Qt::PenStyle lineStyleFromIdx(int idx);
	void setPlotSize(int idx);
	MenuOnOffSwitch *peakHolderSwitch;
	MenuOnOffSwitch *scaleSWitch;
	MenuOnOffSwitch *plotSwitch;
	QPushButton *peakHolderReset;
	MenuCombo *plotStyle;
	MenuComboWidget *plotSize;

	MenuComboWidget *mainMonitorCombo;
	QPushButton *deleteMonitor;

	MenuHeaderWidget *header;
	QVBoxLayout *layout;

	QWidget *settingsBody;
	QVBoxLayout *mainLayout;

	QList<QPair<QString, QCheckBox *>> *monitorsCheckboxList;

	// activeMonitors dropdown
	QList<QString> *activeMonitors;
};
} // namespace datamonitor
} // namespace scopy
#endif // DATAMONITORSETTINGS_H
