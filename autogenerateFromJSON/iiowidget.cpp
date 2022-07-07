#include "iiowidget.hpp"

#include <QTimer>

IioWidget::IioWidget(CustomWidgetInterface *customWidget, ReadWriteInterface *readWrite, double readTimer, QObject *parent)
	: QObject{parent}
	, customWidget(customWidget)
	, readWrite(readWrite)
{
	connect(readWrite, &ReadWriteInterface::readDone, customWidget, &CustomWidgetInterface::updateValue);
	connect(customWidget, &CustomWidgetInterface::valueChanged, readWrite, &ReadWriteInterface::write);

	readWrite->read();

	if (readTimer > 0) {
		timer = new QTimer();

		connect(timer, &QTimer::timeout, this, [=](){
			readWrite->read();
		});

		timer->start(readTimer);
	}

	connect(readWrite, &ReadWriteInterface::writeError, customWidget, [=](const char* err){
		customWidget->giveFeedback(false, err);
	});
	connect(readWrite, &ReadWriteInterface::writeSuccess, customWidget, [=](){
		customWidget->giveFeedback(true, "ok");
	});

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

