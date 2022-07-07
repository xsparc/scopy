#include "customlabelwidget.h"
#include "ui_customlabelwidget.h"

CustomLabelWidget::CustomLabelWidget(const char * attr, QWidget *parent) :
	ui(new Ui::CustomLabelWidget)
{
	ui->setupUi(this);
	ui->name->setText(attr);

	connect(this, &CustomLabelWidget::valueChanged, ui->value, [=](const char* val){
		updateValue(val);
	});

}

CustomLabelWidget::~CustomLabelWidget()
{
	delete ui;
}

void CustomLabelWidget::updateValue(const char *val)
{
	ui->value->setText(val);
}

QWidget *CustomLabelWidget::getWidget()
{
	return ui->widget;
}

void CustomLabelWidget::giveFeedback(bool interaction, const char* msg)
{

}
