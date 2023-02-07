#ifndef SCOPY_SWIOTMAX14906CONTROLLER_HPP
#define SCOPY_SWIOTMAX14906CONTROLLER_HPP

#include <QString>
#include <utility>
#include <iio.h>
#include <QThread>
#include <QTimer>

namespace adiscope {
    class Max14906ToolController : public QObject {
        Q_OBJECT
        private:
                QString m_deviceName;
                struct iio_context* m_context;
                struct iio_device* m_device;

//                std::vector<QThread*> readerThreads;

                void connectSignalsAndSlots();

        public:
                explicit Max14906ToolController(struct iio_context* context_, QString deviceName = "max14906");
                ~Max14906ToolController();

                int getChannelCount();

                iio_device *getDevice() const;

                QString getChannelName(unsigned int index);
                QString getChannelType(unsigned int index);

                void singleRead();
                void startRead();
                void stopRead();
        };
}

#endif //SCOPY_SWIOTMAX14906CONTROLLER_HPP
