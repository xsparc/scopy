#ifndef SCOPY_CHANNEL_CONTROL_H
#define SCOPY_CHANNEL_CONTROL_H

#include <QWidget>
#include "ui_channel_control.h"
#include "gui/generic_menu.hpp"


namespace adiscope {
    class ChannelControl : public gui::GenericMenu {
    Q_OBJECT

    private:
        Ui::ChannelControl *ui;

    public:
        explicit ChannelControl(const QString &deviceName, const QString &deviceType, QWidget *parent = nullptr);

        ~ChannelControl() override;

        Ui::ChannelControl *getUi() const;

        void updateTimeScale(double newMin, double newMax);

    private:
        void connectSignalsAndSlots();
    };
}


#endif //SCOPY_CHANNEL_CONTROL_H
