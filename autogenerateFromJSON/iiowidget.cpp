#include "iiowidget.hpp"

#include <QTimer>

IioWidget::IioWidget(CustomWidgetInterface *customWidget, ReadWriteInterface *readWrite, double readTimer, QObject *parent)
	: QObject{parent}
	, customWidget(customWidget)
	, readWrite(readWrite)
{
	connect(readWrite, &ReadWriteInterface::writeError, customWidget, [=](const char* err){
		customWidget->setStatus("background-color: red", err);
	});
	connect(readWrite, &ReadWriteInterface::writeSuccess, customWidget, [=](){
		customWidget->setStatus("background-color: green", "ok");
	});


	connect(readWrite, &ReadWriteInterface::readDone, customWidget, &CustomWidgetInterface::updateValue);
	connect(customWidget, &CustomWidgetInterface::valueChanged, readWrite, [=](const char *val){

		customWidget->setStatus("background-color: yellow ", "loading");
		readWrite->write(val);
	});

	readWrite->read();

	if (readTimer > 0) {
		timer = new QTimer();

		connect(timer, &QTimer::timeout, this, [=](){
			readWrite->read();
		});

		timer->start(readTimer);

	}
}

QWidget* IioWidget::getWidget()
{
	return customWidget->getWidget();
}

IioWidget::~IioWidget()
{
	delete customWidget;
	delete readWrite;
	delete timer;
}

