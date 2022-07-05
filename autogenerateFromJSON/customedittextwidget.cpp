#include "customedittextwidget.hpp"
#include "ui_customedittextwidget.h"

CustomEditTextWidget::CustomEditTextWidget(const char * attr, bool readOnly, QWidget *parent) :
	ui(new Ui::CustomEditTextWidget),
	readOnly(readOnly)
{
	ui->setupUi(this);

	ui->title->setText(attr);

	connect(this, &CustomEditTextWidget::valueChanged, ui->value, [=](const char* val){
		ui->value->setText(val);
	});

	connect(ui->updateButton, &QPushButton::clicked, this, [=](){
		auto aux = ui->value->text().toStdString();
		const char *val = aux.c_str();
		Q_EMIT CustomWidgetInterface::valueChanged(val);
	});

	if (readOnly) {
		ui->value->setEnabled(false);
		ui->updateButton->setEnabled(false);
	}
}

CustomEditTextWidget::~CustomEditTextWidget()
{
	delete ui;
}

void CustomEditTextWidget::updateValue(const char *val)
{
	ui->value->setText(val);
}

QWidget *CustomEditTextWidget::getWidget()
{
	return ui->mainWidget;
}
