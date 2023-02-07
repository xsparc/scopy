#ifndef SCOPY_SWIOTDIGITALCHANNELCONTROLLER_H
#define SCOPY_SWIOTDIGITALCHANNELCONTROLLER_H

#include "swiotdigitalchannel.h"
#include "logging_categories.h"


#include <QObject>
#include <iio.h>
#include <sstream>

#define ATTR_BUFFER_LEN 200

namespace adiscope {
        class DigitalChannelController : public QObject {
                Q_OBJECT

        private:
                DigitalChannel *m_digitalChannel;

        private:
                QString m_channelName;
                QString m_channelType; // output or input

                std::string m_iioAttrAvailableTypes; // iio attribute
                std::vector<std::string> m_availableTypes;

                std::string m_iioAttrType; // iio attribute
                std::string m_type;

                struct iio_channel* m_channel;
        public:

                explicit DigitalChannelController(struct iio_channel* channel, const QString& deviceName, const QString& deviceType, QWidget *parent);

                DigitalChannel *getDigitalChannel() const;

                void writeType();
        };

} // adiscope

#endif //SCOPY_SWIOTDIGITALCHANNELCONTROLLER_H
