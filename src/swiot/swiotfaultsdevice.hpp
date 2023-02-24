#ifndef SCOPY_SWIOTFAULTSDEVICE_HPP
#define SCOPY_SWIOTFAULTSDEVICE_HPP

#include <QWidget>
#include "ui_faultsdevice.h"
#include "faultsgroup.hpp"


namespace adiscope::gui {
        class FaultsGroup;

        class FaultsDevice : public QWidget {
                Q_OBJECT
        public:
                explicit FaultsDevice(QString name, QString path, uint32_t max_faults_num, QWidget* parent);
                ~FaultsDevice() override;

                QPushButton* getResetButton();
                QLabel* getExplanations();
                FaultsGroup *getFaultsGroup() const;
                void setNumericText(const QString& text);

        private:
                Ui::FaultsDevice *ui;

                QWidget* m_parent;
                FaultsGroup* m_faultsGroup;

                QString m_name;
                QString m_path;
                uint32_t m_max_faults_num;
        };

} // adiscope::gui

#endif //SCOPY_SWIOTFAULTSDEVICE_HPP
