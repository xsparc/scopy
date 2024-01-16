#ifndef TESTREADSTRATEGY_HPP
#define TESTREADSTRATEGY_HPP

#include "ireadstrategy.hpp"

namespace scopy {
namespace datamonitor {

class TestReadStrategy : public IReadStrategy
{
public:
	TestReadStrategy();

	// IReadStrategy interface
public:
	void read(QString title);
	double testData = 0;
};
} // namespace datamonitor
} // namespace scopy
#endif // TESTREADSTRATEGY_HPP
