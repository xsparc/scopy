#ifndef READWRITEINTERFACE_H
#define READWRITEINTERFACE_H

#include <QObject>


// TODO set the right buf size
#define BUF_SIZE 16384

class ReadWriteInterface : public QObject
{
	Q_OBJECT
public:
	explicit ReadWriteInterface(QObject *parent = nullptr);

	virtual void read()=0;
	virtual void write(const char *val)=0;

signals:
	void readDone(const char *value);
};

#endif // READWRITEINTERFACE_H
