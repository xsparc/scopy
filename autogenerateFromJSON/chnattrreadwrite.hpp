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

private:
	const struct iio_channel *chn;
	char *attr = nullptr;

	// AttrReadWriteInterface interface
public:
	void read() override;
	void write(const char *val) override;
};

#endif // CHNATTRREADWRITE_HPP
