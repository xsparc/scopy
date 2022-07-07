#include "devattrreadwrite.hpp"
#include "qdebug.h"
#include <iio.h>

DevAttrReadWrite::DevAttrReadWrite(struct iio_device *dev, QString attr, QObject *parent)
{
	this->dev = dev;
	// TBD
	auto s = attr.size();
	this->attr = new char[s];
	for (int i = 0; i < s ; i++) {
		this->attr[i] = attr.at(i).toLatin1();

	}
}

DevAttrReadWrite::~DevAttrReadWrite()
{
	delete []attr;
}

void DevAttrReadWrite::read()
{
	char *val = (char* ) malloc(BUF_SIZE);
	ssize_t read = iio_device_attr_read(dev, attr, val, BUF_SIZE);
	if (read < 0) {
		qDebug()<< "device read err";
	} else {
		Q_EMIT readDone(val);

	}

	free(val);
}

void DevAttrReadWrite::write(const char *val)
{
	ssize_t write = iio_device_attr_write(dev, attr, val);
	if (write < 0) {
		qDebug()<< "device write err";
	}
	read();
}
