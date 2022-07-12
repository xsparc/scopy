#ifndef CONTEXTATTRREADWRITE_HPP
#define CONTEXTATTRREADWRITE_HPP

#include <QObject>
#include <readwriteinterface.hpp>

class ContextAttrReadWrite: public ReadWriteInterface
{
	Q_OBJECT
public:
	explicit ContextAttrReadWrite(struct iio_context *ctx, QString attr, QObject *parent = nullptr);
	~ContextAttrReadWrite();

private:
	struct iio_context *ctx;
	char *attr = nullptr;

	// AttrReadWriteInterface interface
public:
	void read() override;
	void write(const char *val) override;
};

#endif // CONTEXTATTRREADWRITE_HPP
