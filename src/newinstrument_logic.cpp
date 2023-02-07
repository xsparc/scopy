//
// Created by andrei-fabian on 06/02/23.
//

#include <iostream>
#include "newinstrument_logic.hpp"

adiscope::NewInstrumentLogic::NewInstrumentLogic(struct iio_context* context_, QString deviceName) :
        deviceName(std::move(deviceName)),
        context(context_) {
        struct iio_device* dev = iio_context_get_device(this->context, 1);
        this->device = dev;
}

adiscope::NewInstrumentLogic::~NewInstrumentLogic() = default;

int adiscope::NewInstrumentLogic::getChannelCount() {
        unsigned int dev_count = iio_device_get_channels_count(this->device);
        return (int)(dev_count);
}

QString adiscope::NewInstrumentLogic::getChannelName(unsigned int index) {
        iio_channel *channel = iio_device_get_channel(this->device, index);
        QString name = iio_channel_get_id(channel);

        return name;
}

QString adiscope::NewInstrumentLogic::getChannelType(unsigned int index) {
        iio_channel* channel = iio_device_get_channel(this->device, index);
        bool output = iio_channel_is_output(channel);

        return output ? "output" : "input";
}

