#include "customcomboboxwidget.hpp"

#include <qcombobox.h>
#include <qlabel.h>

CustomComboBoxWidget::CustomComboBoxWidget(const char * attr, QStringList available_values, bool readOnly, QLayout* layout, QWidget *parent)
{
	widget = new QWidget(this);
	widget->setLayout(layout);
	widget->layout()->addWidget(new QLabel(attr));

	comboBox = new QComboBox();
	comboBox->addItems(available_values);
	widget->layout()->addWidget(comboBox);

	connect(this, &CustomComboBoxWidget::valueChanged, comboBox, [=](const char* val){
		updateValue(val);
	});

	connect(comboBox, &QComboBox::currentTextChanged, this, [=](){
		auto aux = comboBox->currentText();
		const char *val = aux.toStdString().c_str();
		Q_EMIT CustomWidgetInterface::valueChanged(val);
	});

	if (readOnly) {
		comboBox->setEnabled(false);
	}

	mainLayout->addWidget(widget);
}

CustomComboBoxWidget::~CustomComboBoxWidget()
{
	delete mainLayout;
}

void CustomComboBoxWidget::updateValue(const char *val)
{
	comboBox->setCurrentText(val);
}

QWidget *CustomComboBoxWidget::getWidget()
{
	return widget;
}

void CustomComboBoxWidget::setStatus(QString styleSheet, const char* msg)
{

}
