#ifndef IREGISTERREADSTRATEGY_HPP
#define IREGISTERREADSTRATEGY_HPP

#include <QObject>

namespace scopy {
namespace datamonitor {
class IReadStrategy : public QObject
{
	Q_OBJECT
public:
	virtual void read(QString title) = 0;

Q_SIGNALS:
	void readDone(QString title, double value);
	void readError(const char *err);
};
} // namespace datamonitor
} // namespace scopy
#endif // IREGISTERREADSTRATEGY_HPP
