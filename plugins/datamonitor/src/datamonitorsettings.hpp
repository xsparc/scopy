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
	Q_OBJECT
public:
	explicit DataMonitorSettings(QWidget *parent = nullptr);
	~DataMonitorSettings();

	void init(QString title, QColor color);

public Q_SLOTS:
	void scaleToggle(bool toggled);
	void peakHolderToggle(bool toggled);
	void historyToggle(bool toggled);
	void peakHolderResetClicked();
	void historySizeChanged(int size);
	void changeLineStyle(int index);

Q_SIGNALS:
	void toggleScale(bool toggle);
	void monitorColorChanged(QString color);
	void toggleHistory(bool toggled);
	void changeHistorySize(int size);
	void lineStyleChanged(Qt::PenStyle lineStyle);
	void lineStyleIndexChanged(int index);
	void historySizeIndexChanged(int index);
	void resetPeakHolder();
	void togglePeakHolder(bool toggled);
	void toggleAll(bool toggle);

private:
	double numSamplesFromIdx(int idx);
	Qt::PenStyle lineStyleFromIdx(int idx);
	void setHistorySize(int idx);
	MenuOnOffSwitch *peakHolderSwitch;
	MenuOnOffSwitch *scaleSWitch;
	MenuOnOffSwitch *historySwitch;
	QPushButton *peakHolderReset;
	MenuComboWidget *historyStyle;
	MenuComboWidget *historySize;

	MenuHeaderWidget *header;
	QVBoxLayout *layout;
};
} // namespace datamonitor
} // namespace scopy
#endif // DATAMONITORSETTINGS_H
