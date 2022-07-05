#include "customspinboxwidget.hpp"
#include "ui_customspinboxwidget.h"

CustomSpinBoxWidget::CustomSpinBoxWidget(const char * attr, const char * min_range, const char * max_range, const char * step, bool readOnly,QWidget *parent) :
	ui(new Ui::CustomSpinBoxWidget)
{
	ui->setupUi(this);

	ui->name->setText(attr);
	ui->value->setRange(std::stod(min_range),std::stod(max_range));
	ui->value->setSingleStep(std::stod(step));

	connect(this, &CustomSpinBoxWidget::valueChanged, ui->value, [=](const char* val){
		updateValue(val);
	});

	connect(ui->value, &QDoubleSpinBox::textChanged, this, [=](){
		//TODO int and double checks
		auto aux = (int) ui->value->value();
		const char *val = std::to_string(aux).c_str();
		Q_EMIT CustomWidgetInterface::valueChanged(val);
	});

	if (readOnly) {
		ui->value->setEnabled(false);
	}
}

CustomSpinBoxWidget::~CustomSpinBoxWidget()
{
	delete ui;
}

void CustomSpinBoxWidget::updateValue(const char *val)
{
	ui->value->setValue(std::stod(val));
}

QWidget *CustomSpinBoxWidget::getWidget()
{
	return ui->mainWIdget;
}
