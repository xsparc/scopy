#include "customspinboxwidget.hpp"

#include <QDoubleSpinBox>
#include <qlabel.h>
#include <qtimer.h>

CustomSpinBoxWidget::CustomSpinBoxWidget(const char * attr, const char * min_range, const char * max_range, const char * step,
					 QString type, bool readOnly, QLayout* layout, QWidget *parent) :
	isInt(false)
{


	widget = new QWidget(this);
	widget->setLayout(layout);
	widget->layout()->addWidget(new QLabel(attr));

	value = new QDoubleSpinBox();
	value->setRange(std::stod(min_range),std::stod(max_range));
	value->setSingleStep(std::stod(step));

	widget->layout()->addWidget(value);

	pushButton = new QPushButton();
	pushButton->setToolTip("test");

	widget->layout()->addWidget(pushButton);

	if (type == "int") {
		isInt = true;
	}

	connect(this, &CustomSpinBoxWidget::valueChanged, value, [=](const char* val){
		updateValue(val);
	});

	connect(value, &QDoubleSpinBox::textChanged, this, [=](){
		//TODO int and double checks
		auto aux = value->value();
		char *val;
		if (isInt) {
			val =(char*) std::to_string((int)aux).c_str();
		} else {
			val =(char*) std::to_string(aux).c_str();
		}

		Q_EMIT CustomWidgetInterface::valueChanged(val);

	});

	if (readOnly) {
		value->setEnabled(false);
	}

	timer = new QTimer();

	mainLayout->addWidget(widget);
}

CustomSpinBoxWidget::~CustomSpinBoxWidget()
{
	delete mainLayout;
}

void CustomSpinBoxWidget::updateValue(const char *val)
{
	value->setValue(std::stod(val));
}

QWidget *CustomSpinBoxWidget::getWidget()
{
	return widget;
}

void CustomSpinBoxWidget::setStatus(QString styleSheet, const char* msg)
{
	pushButton->setStyleSheet(styleSheet);
	pushButton->setToolTip(msg);

}
