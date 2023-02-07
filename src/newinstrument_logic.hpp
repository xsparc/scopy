//
// Created by andrei-fabian on 06/02/23.
//

#ifndef SCOPY_NEWINSTRUMENT_LOGIC_HPP
#define SCOPY_NEWINSTRUMENT_LOGIC_HPP

#include <QString>
#include <utility>
#include <iio.h>
#include <QThread>

namespace adiscope {
    class NewInstrumentLogic {
    private:
        QString deviceName;
        struct iio_context* context;
        struct iio_device* device;

        std::vector<QThread*> readerThreads;

    public:
        explicit NewInstrumentLogic(struct iio_context* context_, QString deviceName = "max14906"); // idk man
        ~NewInstrumentLogic();

	int getChannelCount();

        QString getChannelName(unsigned int index);
        QString getChannelType(unsigned int index);


    };
}

#endif //SCOPY_NEWINSTRUMENT_LOGIC_HPP
