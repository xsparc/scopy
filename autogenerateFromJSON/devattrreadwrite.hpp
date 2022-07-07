#ifndef DEVATTRREADWRITE_HPP
#define DEVATTRREADWRITE_HPP

#include <readwriteinterface.hpp>


class DevAttrReadWrite: public ReadWriteInterface
{
	Q_OBJECT
public:
	explicit DevAttrReadWrite(struct iio_device *dev, QString attr, QObject *parent = nullptr);
	~DevAttrReadWrite();

private:
	const struct iio_device *dev;
	char *attr = nullptr;

	// ReadWriteInterface interface
public:
	void read() override;
	void write(const char *val) override;
};

#endif // DEVATTRREADWRITE_HPP
