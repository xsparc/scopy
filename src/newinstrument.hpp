#ifndef NEWINSTRUMENT_HPP
#define NEWINSTRUMENT_HPP

#include <filter.hpp>
#include <tool_launcher.hpp>
#include "ui_newinstrument.h"
#include <tool.hpp>

namespace adiscope {
    class NewInstrument : public Tool {
    Q_OBJECT

    private:
        Ui::NewInstrument *ui;

        const QString buttonStyleSheet = "QPushButton { width: 40px; height: 14px; border: 2px solid ; border-radius: 8px; border-color: white; }"
                                         "QPushButton[high=false]"
                                         "{"
                                         "border-color: white;"
                                         "background-color: rgba(255, 255, 255, 50); "
                                         "}"
                                         "QPushButton[high=true] "
                                         "{"
                                         "border-color: white;"
                                         "background-color: white;"
                                         "}"
                                         "QPushButton[high=false][short=true] {"
                                         "border-color: red;"
                                         "}"
                                         "QPushButton[high=true][short=true] {"
                                         "border-color: red;"
                                         "background-color: red;"
                                         "}";

        QVector<QPair<QPushButton*, QPushButton*>*> ad74413r_fault_buttons;
        QVector<QPair<QPushButton*, QPushButton*>*> max1490b_fault_buttons;

        uint16_t ad74413r_faults_num;
        uint16_t max1490b_faults_num;

        void connectSignalsAndSlots();

    public:
        NewInstrument(struct iio_context *ctx, Filter *filt,
                      ToolMenuItem *toolMenuItem,
                      QJSEngine *engine, ToolLauncher *parent);

        ~NewInstrument() override;

        void generateAd74413rButtons();
        void generateMax1490bButtons();

        void ad74413rSetup();
        void max1490bSetup();

        void getAd74413rFaultsNumeric();
        void getMax1490bFaultsNumeric();

        void initAd74413rFaults();
        void initMax1490bFaults();

        void setAd74413rFaults();
        void setMax1490bFaults();

        void populateAdExplanations();
        void populateMaxExplanations();

    private Q_SLOTS:

        void resetStoredAd74413r();
        void resetStoredMax1490b();

    Q_SIGNALS:

        void showTool();
    };
}
#endif // NEWINSTRUMENT_HPP
