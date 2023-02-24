#ifndef FAULTSPAGE_HPP
#define FAULTSPAGE_HPP

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>

#include "faultsgroup.hpp"
#include "swiotfaultsdevice.hpp"

namespace Ui {
    class FaultsPage;
}

namespace adiscope::gui {
    class FaultsDevice;

    class FaultsPage : public QWidget {
    Q_OBJECT

    public:
        explicit FaultsPage(QWidget *parent = nullptr);

        ~FaultsPage() override;

        QPushButton* getAdResetButton();
        QPushButton* getMaxResetButton();

        QLabel* getAdExplanations();
        QLabel* getMaxExplanations();

        adiscope::gui::FaultsGroup *getAdFaultsGroup() const;
        adiscope::gui::FaultsGroup *getMaxFaultsGroup() const;

        void setAdNumericText(const QString& text);
        void setMaxNumericText(const QString& text);

    private:
        Ui::FaultsPage *ui;

        FaultsDevice* m_ad74413rFaultsDevice;
        FaultsDevice* m_max14906FaultsDevice;
    };

}
#endif // FAULTSPAGE_HPP
