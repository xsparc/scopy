#include "cmdreadwrite.hpp"
#include "qdebug.h"

#include <QProcess>

CmdReadWrite::CmdReadWrite(ConsoleCmd readCmd, ConsoleCmd writeCmd, QObject *parent)
{
	cmdReadProcess = new QProcess();
	cmdWriteProcess = new QProcess();

	connect(cmdReadProcess, &QProcess::readyReadStandardOutput, [=]() {
		QString output = cmdReadProcess->readAllStandardOutput();
		qDebug() << "read value is " << output;
		Q_EMIT readDone(output.toStdString().c_str());
		if (cmdReadProcess->isOpen()) {
			cmdReadProcess->terminate();
		}
	});

	connect(cmdWriteProcess, &QProcess::readyReadStandardOutput, [=]() {
		QString output = cmdWriteProcess->readAllStandardOutput();
		qDebug() << "write value is " << output;
		if (output.contains("error")) {
			Q_EMIT writeError(output.toStdString().c_str());
		} else {
			Q_EMIT writeSuccess();
		}
		if (cmdWriteProcess->isOpen()) {
			cmdWriteProcess->terminate();
		}
		read();
	});

	this->readCmd = readCmd;
	this->writeCmd = writeCmd;
}

CmdReadWrite::~CmdReadWrite()
{
	delete cmdReadProcess;
	delete cmdWriteProcess;
}

void CmdReadWrite::read()
{
	cmdReadProcess->start(readCmd.cmd, readCmd.params);
}

void CmdReadWrite::write(const char *val)
{
	auto aux = writeCmd.params;
	aux.replaceInStrings("$EDITVAL",val);

	cmdWriteProcess->start(writeCmd.cmd, aux);
}
