#include "iiocheckbox.hpp"

#include <customcheckboxwidget.hpp>

IioCheckBox::IioCheckBox(ReadWriteInterface *readWrite,bool readOnly, QObject *parent)
	: QObject{parent},
	  readWrite(readWrite),
	  readOnly(readOnly)
{
	//checkBoxWidget = new CustomCheckBoxWidget(readWrite->getAttr(), readOnly);

	if (!readOnly) {
		connect(checkBoxWidget, &CustomCheckBoxWidget::valueChanged, readWrite, [=](const char* val){
			readWrite->write(val);
		});
	}

	connect(readWrite, &ReadWriteInterface::readDone, checkBoxWidget, [=](const char* val){
		checkBoxWidget->updateValue(val);
	});

	readWrite->read();
}

CustomCheckBoxWidget* IioCheckBox::getWidget()
{
	return checkBoxWidget;
}
