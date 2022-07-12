#ifndef CMDREADWRITE_HPP
#define CMDREADWRITE_HPP

#include "readwriteinterface.hpp"
#include <QObject>

struct ConsoleCmd {
	QString cmd;
	QStringList params;
};

class QProcess;

class CmdReadWrite : public ReadWriteInterface
{
	Q_OBJECT
public:
	explicit CmdReadWrite(ConsoleCmd readCmd, ConsoleCmd writeCmd, QObject *parent = nullptr);
	~CmdReadWrite();

private:
	QProcess *cmdReadProcess;
	QProcess *cmdWriteProcess;
	ConsoleCmd readCmd;
	ConsoleCmd writeCmd;

	// ReadWriteInterface interface
public:
	void read();
	void write(const char *val);
};

#endif // CMDREADWRITE_HPP
