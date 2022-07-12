#include "customedittextwidget.hpp"

#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>

CustomEditTextWidget::CustomEditTextWidget(const char * attr, bool readOnly, QLayout* layout, QWidget *parent) :
	readOnly(readOnly)
{

	widget = new QWidget(this);
	widget->setLayout(layout);
	widget->layout()->addWidget(new QLabel(attr));

	value = new QLineEdit();
	updateButton = new QPushButton("update");

	widget->layout()->addWidget(value);
	widget->layout()->addWidget(updateButton);

	connect(this, &CustomEditTextWidget::valueChanged, value, [=](const char* val){
		updateValue(val);
	});

	connect(updateButton, &QPushButton::clicked, this, [=](){
		auto aux = value->text().toStdString();
		const char *val = aux.c_str();
		Q_EMIT CustomWidgetInterface::valueChanged(val);
	});

	if (readOnly) {
		value->setEnabled(false);
		updateButton->setEnabled(false);
	}

	mainLayout->addWidget(widget);
}

CustomEditTextWidget::~CustomEditTextWidget()
{
	delete mainLayout;
}

void CustomEditTextWidget::updateValue(const char *val)
{
	value->setText(val);
}

QWidget *CustomEditTextWidget::getWidget()
{
	return widget;
}

void CustomEditTextWidget::setStatus(QString styleSheet, const char* msg)
{

}
