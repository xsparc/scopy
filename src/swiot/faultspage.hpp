#ifndef FAULTSPAGE_HPP
#define FAULTSPAGE_HPP

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>

#include "faultsgroup.hpp"

namespace Ui {
    class FaultsPage;
}

namespace adiscope::gui {
    class FaultsGroup;

    class FaultsPage : public QWidget {
    Q_OBJECT

    public:
        explicit FaultsPage(QWidget *parent = nullptr);

        ~FaultsPage() override;

        QPushButton* getAdResetButton();
        QPushButton* getMaxResetButton();

        QTextEdit* getAdExplanations();
        QTextEdit* getMaxExplanations();

        adiscope::gui::FaultsGroup *getAdFaultsGroup() const;
        adiscope::gui::FaultsGroup *getMaxFaultsGroup() const;

        void setAdNumericText(const QString& text);
        void setMaxNumericText(const QString& text);

    private:
        Ui::FaultsPage *ui;
        FaultsGroup* adFaultsGroup;
        FaultsGroup* maxFaultsGroup;
    };

}
#endif // FAULTSPAGE_HPP
