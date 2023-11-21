#ifndef DATAMONITORTOOLSETTINGS_H
#define DATAMONITORTOOLSETTINGS_H

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <menuheader.h>
#include <menuonoffswitch.h>
#include <spinbox_a.hpp>

namespace scopy {
namespace datamonitor {

class DataLoggerController;

class DataMonitorToolSettings : public QWidget
{
	Q_OBJECT
public:
	explicit DataMonitorToolSettings(QWidget *parent = nullptr);
	~DataMonitorToolSettings();

	void init(QString title, QColor color);

signals:
	void toggleAll(bool toggle);
	void precisionChanged(int precision);
	void recordingIntervalChanged(double recordingInterval);
	void updateDataLoggerValue(QString name, QString value);

private:
	MenuHeaderWidget *header;
	QVBoxLayout *layout;
	MenuOnOffSwitch *showAllSwitch;
	QPushButton *precisionBtn;
	QLineEdit *precisionValue;
	PositionSpinButton *recordingTimer;
	DataLoggerController *dataLoggerController;
};
} // namespace datamonitor
} // namespace scopy
#endif // DATAMONITORTOOLSETTINGS_H
