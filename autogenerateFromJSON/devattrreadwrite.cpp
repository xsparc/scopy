#include "devattrreadwrite.hpp"
#include "qdebug.h"
#include <iio.h>

DevAttrReadWrite::DevAttrReadWrite(const struct iio_device *dev, const char *attr, QObject *parent)
	: dev(dev),
	  attr(attr)
{

}

void DevAttrReadWrite::readAttr()
{
	char *val;
	ssize_t read = iio_device_attr_read(dev, attr, val, BUF_SIZE);
	if (read < 0) {
		qDebug()<< "device read err";
	} else {
		Q_EMIT readDone(val);

	}
}

void DevAttrReadWrite::writeAttr(const char *val)
{
	ssize_t write = iio_device_attr_write(dev, attr, val);
	if (write < 0) {
		qDebug()<< "device write err";
	} else {
		readAttr();
	}
}
