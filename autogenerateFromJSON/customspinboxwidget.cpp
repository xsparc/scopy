#include "customspinboxwidget.hpp"
#include "ui_customspinboxwidget.h"

#include <qtimer.h>

CustomSpinBoxWidget::CustomSpinBoxWidget(const char * attr, const char * min_range, const char * max_range, const char * step,
					 QString type, bool readOnly,QWidget *parent) :
	ui(new Ui::CustomSpinBoxWidget),
	isInt(false)
{
	ui->setupUi(this);

	ui->name->setText(attr);
	ui->value->setRange(std::stod(min_range),std::stod(max_range));
	ui->value->setSingleStep(std::stod(step));

	auto current = ui->mainWidget->styleSheet();

	if (type == "int") {
		isInt = true;
	}

	connect(this, &CustomSpinBoxWidget::valueChanged, ui->value, [=](const char* val){
		updateValue(val);
	});

	connect(ui->value, &QDoubleSpinBox::textChanged, this, [=](){
		//TODO int and double checks
		auto aux = ui->value->value();
		char *val;
		if (isInt) {
			val =(char*) std::to_string((int)aux).c_str();
		} else {
			val =(char*) std::to_string(aux).c_str();
		}

		Q_EMIT CustomWidgetInterface::valueChanged(val);

	});

	if (readOnly) {
		ui->value->setEnabled(false);
	}

	timer = new QTimer();

	connect(timer, &QTimer::timeout, this, [=](){
		ui->errorMessage->setVisible(false);
	});


	ui->errorMessage->setVisible(false);
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
	return ui->mainWidget;
}

void CustomSpinBoxWidget::giveFeedback(bool interaction,const char* msg)
{

	if (interaction) {
		ui->errorMessage->setStyleSheet("color: #39FF14");
	} else {
		ui->errorMessage->setStyleSheet("color: red");

	}
	ui->errorMessage->setText(msg);
	ui->errorMessage->setVisible(true);

	timer->start(1000);
}
