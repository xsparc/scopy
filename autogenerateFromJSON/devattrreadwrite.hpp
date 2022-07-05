#ifndef DEVATTRREADWRITE_HPP
#define DEVATTRREADWRITE_HPP

#include <readwriteinterface.hpp>



class DevAttrReadWrite: public ReadWriteInterface
{
	Q_OBJECT
public:
	explicit DevAttrReadWrite(const struct iio_device *dev, const char *attr, QObject *parent = nullptr);

	// AttrReadWriteInterface interface
public:
	void readAttr();
	void writeAttr(const char *val);

private:
	const struct iio_device *dev;
	const char *attr;
};

#endif // DEVATTRREADWRITE_HPP
