#include "testreadstrategy.hpp"

using namespace scopy;
using namespace datamonitor;

TestReadStrategy::TestReadStrategy() {}

void TestReadStrategy::read(QString title)
{
	testData += 0.2;
	Q_EMIT readDone(title, testData);
}
