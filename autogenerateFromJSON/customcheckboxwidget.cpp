#include "customcheckboxwidget.hpp"

CustomCheckBoxWidget::CustomCheckBoxWidget(const char* attr, bool readOnly, QWidget *parent) :
	readOnly(readOnly)
{
	mainLayout = new QVBoxLayout(this);
	checkBox = new QCheckBox();
	checkBox->setText(attr);
	mainLayout->addWidget(checkBox);

	connect(this, &CustomCheckBoxWidget::valueChanged, checkBox, [=](const char* val){
		updateValue(val);
	});

	connect(checkBox, &QCheckBox::toggled, this, [=](bool toggled){
		const char *val = "0";
		if (toggled) {
			val = "1";
		}
		Q_EMIT CustomWidgetInterface::valueChanged(val);
	});

	if (readOnly) {
		checkBox->setEnabled(false);
	}
}

CustomCheckBoxWidget::~CustomCheckBoxWidget()
{
	delete mainLayout;
}

void CustomCheckBoxWidget::updateValue(const char *val)
{
	bool toggle = false;
	if (strncmp(val,"1",2) == 0) {
		toggle = true;
	}
	checkBox->setChecked(toggle);
}

QWidget* CustomCheckBoxWidget::getWidget()
{
	return checkBox;
}

void CustomCheckBoxWidget::setStatus(QString styleSheet, const char* msg)
{

}
