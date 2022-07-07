#include "contextattrreadwrite.hpp"
#include "qdebug.h"
#include <iio.h>

ContextAttrReadWrite::ContextAttrReadWrite(struct iio_context *ctx,QString attr, QObject *parent)
{
	this->ctx = ctx;
	// TBD
	auto s = attr.size();
	this->attr = new char[s];
	for (int i = 0; i < s ; i++) {
		this->attr[i] = attr.at(i).toLatin1();

	}
}

ContextAttrReadWrite::~ContextAttrReadWrite()
{
	delete []attr;
}

void ContextAttrReadWrite::read()
{
	const char *val = iio_context_get_attr_value(ctx, attr);
	Q_EMIT readDone(val);
}

void ContextAttrReadWrite::write(const char *val)
{
	//context attributes can't be weitten
}
