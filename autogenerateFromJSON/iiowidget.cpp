#include "iiowidget.hpp"

IioWidget::IioWidget(CustomWidgetInterface *customWidget, ReadWriteInterface *readWrite,QObject *parent)
	: QObject{parent}
	, customWidget(customWidget)
	, readWrite(readWrite)
{
	connect(readWrite, &ReadWriteInterface::readDone, customWidget, &CustomWidgetInterface::updateValue);
	readWrite->read();

	connect(customWidget, &CustomWidgetInterface::valueChanged, readWrite, &ReadWriteInterface::write);

}

QWidget* IioWidget::getWidget()
{
	return customWidget->getWidget();
}

IioWidget::~IioWidget()
{
	delete customWidget;
	delete readWrite;
}

