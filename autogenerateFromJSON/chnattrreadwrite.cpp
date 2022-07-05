#include "chnattrreadwrite.hpp"
#include <iio.h>
#include "qdebug.h"

ChnAttrReadWrite::ChnAttrReadWrite(struct iio_channel *chn, QString attr, QObject *parent)
{
	this->chn = chn;
	// TBD
	auto s = attr.size();
	this->attr = new char[s];
	for (int i = 0; i < s ; i++) {
		this->attr[i] = attr.at(i).toLatin1();

	}
}

ChnAttrReadWrite::~ChnAttrReadWrite()
{
	delete []attr;
}

void ChnAttrReadWrite::read()
{
	char *val = (char* ) malloc(BUF_SIZE);
	ssize_t read = iio_channel_attr_read(chn, attr, val, BUF_SIZE);
	if (read < 0) {
		qDebug()<< "channel read err";
	} else {
		Q_EMIT readDone(val);

	}

	free(val);
}

void ChnAttrReadWrite::write( const char *val)
{
	ssize_t write = iio_channel_attr_write(chn, attr, val);
	if (write < 0) {
		qDebug()<< "channel write err";
	}
	read();
}
