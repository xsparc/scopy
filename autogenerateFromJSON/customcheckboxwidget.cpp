#include "customcheckboxwidget.hpp"
#include "ui_customcheckboxwidget.h"

CustomCheckBoxWidget::CustomCheckBoxWidget(const char* attr, bool readOnly, QWidget *parent) :
	ui(new Ui::CustomCheckBoxWidget),
	readOnly(readOnly)
{
	ui->setupUi(this);

	ui->checkBox->setText(attr);

	connect(this, &CustomCheckBoxWidget::valueChanged, ui->checkBox, [=](const char* val){
		updateValue(val);
	});

	connect(ui->checkBox, &QCheckBox::toggled, this, [=](bool toggled){
		const char *val = "0";
		if (toggled) {
			val = "1";
		}
		Q_EMIT CustomWidgetInterface::valueChanged(val);
	});

	if (readOnly) {
		ui->checkBox->setEnabled(false);
	}
}

CustomCheckBoxWidget::~CustomCheckBoxWidget()
{
	delete ui;
}

void CustomCheckBoxWidget::updateValue(const char *val)
{
	bool toggle = false;
	if (strncmp(val,"1",2) == 0) {
		toggle = true;
	}
	ui->checkBox->setChecked(toggle);
}

QWidget* CustomCheckBoxWidget::getWidget()
{
	return ui->checkBox;
}

void CustomCheckBoxWidget::giveFeedback(bool interaction, const char* msg)
{

}
