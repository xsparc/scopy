#ifndef FAULTSGROUP_HPP
#define FAULTSGROUP_HPP

#include <QWidget>
#include "src/swiot/faultwidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class FaultsGroup; }
QT_END_NAMESPACE

namespace adiscope::gui {
    class FaultsGroup : public QWidget {
    Q_OBJECT

    public:
        explicit FaultsGroup(const QString& name, const QString& path, QWidget *parent = nullptr);
        ~FaultsGroup() override;

        const std::vector<FaultWidget *> &getFaults() const;

        const QString &getName() const;
        void setName(const QString &name);

        int getMaxFaults() const;
        void setMaxFaults(int maxFaults);
    private:
        Ui::FaultsGroup *ui;
        QString name;
        std::vector<FaultWidget *> faults;
        int max_faults;
    };

}

#endif // FAULTSGROUP_HPP
