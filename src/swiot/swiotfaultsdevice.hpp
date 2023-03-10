#ifndef SCOPY_SWIOTFAULTSDEVICE_HPP
#define SCOPY_SWIOTFAULTSDEVICE_HPP

#include <QWidget>
#include <QTextEdit>
#include "ui_faultsdevice.h"
#include "faultsgroup.hpp"
#include "gui/subsection_separator.hpp"


namespace adiscope::gui {
class FaultsGroup;

class FaultsDevice : public QWidget {
	Q_OBJECT
public:
	explicit FaultsDevice(QString name, QString path, uint32_t max_faults_num, QWidget* parent);
	~FaultsDevice() override;

	void update(uint32_t faults_numeric);

public Q_SLOTS:
	void resetStored();
	void updateExplanations();

private:
	Ui::FaultsDevice *ui;
	QWidget* m_parent;
	QTextEdit *m_faults_explanation;
	SubsectionSeparator *m_subsectionSeparator;

	FaultsGroup* m_faultsGroup;

	QString m_name;
	QString m_path;
	uint32_t m_max_faults_num;

	void resizeEvent(QResizeEvent *event) override;

private Q_SLOTS:
	void updateMinimumHeight();
};

} // adiscope::gui

#endif //SCOPY_SWIOTFAULTSDEVICE_HPP
