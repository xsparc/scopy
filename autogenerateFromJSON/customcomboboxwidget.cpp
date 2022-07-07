#include "customcomboboxwidget.hpp"
#include "ui_customcomboboxwidget.h"

CustomComboBoxWidget::CustomComboBoxWidget(const char * attr, QStringList available_values, bool readOnly,QWidget *parent) :
	ui(new Ui::CustomComboBoxWidget)
{
	ui->setupUi(this);

	ui->name->setText(attr);

	ui->comboBox->addItems(available_values);

	connect(this, &CustomComboBoxWidget::valueChanged, ui->comboBox, [=](const char* val){
		updateValue(val);
	});

	connect(ui->comboBox, &QComboBox::currentTextChanged, this, [=](){
		auto aux = ui->comboBox->currentText();
		const char *val = aux.toStdString().c_str();
		Q_EMIT CustomWidgetInterface::valueChanged(val);
	});

	if (readOnly) {
		ui->comboBox->setEnabled(false);
	}
}

CustomComboBoxWidget::~CustomComboBoxWidget()
{
	delete ui;
}

void CustomComboBoxWidget::updateValue(const char *val)
{
	ui->comboBox->setCurrentText(val);
}

QWidget *CustomComboBoxWidget::getWidget()
{
	return ui->mainWidget;
}

void CustomComboBoxWidget::giveFeedback(bool interaction, const char* msg)
{

}
