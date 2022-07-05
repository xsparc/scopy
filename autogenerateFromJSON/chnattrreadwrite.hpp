#ifndef CHNATTRREADWRITE_HPP
#define CHNATTRREADWRITE_HPP

#include <QObject>
#include <readwriteinterface.hpp>


class ChnAttrReadWrite : public ReadWriteInterface
{
	Q_OBJECT
public:
	explicit ChnAttrReadWrite(struct iio_channel *chn, QString attr, QObject *parent = nullptr);
	~ChnAttrReadWrite();

signals:


	// AttrReadWriteInterface interface
public:
	void read();
	void write(const char *val);

private:
	const struct iio_channel *chn;
	char *attr = nullptr;
};

#endif // CHNATTRREADWRITE_HPP
