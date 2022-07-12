#include "customlabelwidget.h"

#include <QImage>
#include <qboxlayout.h>
#include <qlabel.h>

CustomLabelWidget::CustomLabelWidget(const char * attr, QLayout* layout, QWidget *parent)
{
	widget = new QWidget(this);
	widget->setLayout(layout);
	widget->layout()->addWidget(new QLabel(attr));

	value = new QLabel(widget);
	widget->layout()->addWidget(value);

	connect(this, &CustomLabelWidget::valueChanged, value, [=](const char* val){
		updateValue(val);
	});

	mainLayout->addWidget(widget);
}

CustomLabelWidget::CustomLabelWidget(const char *attr, QString imgPath, QLayout *layout, QWidget *parent)
{

	widget = new QWidget(this);
	widget->setLayout(layout);
	widget->layout()->addWidget(new QLabel(attr));

	QImage* image = new QImage(imgPath);
	value = new QLabel(widget);
	value->setPixmap(QPixmap::fromImage(*image));
	widget->layout()->addWidget(value);

	mainLayout->addWidget(widget);
}

CustomLabelWidget::~CustomLabelWidget()
{
	delete mainLayout;
}

void CustomLabelWidget::updateValue(const char *val)
{
	value->setText(val);
}
QWidget *CustomLabelWidget::getWidget()
{
	return widget;
}

void CustomLabelWidget::setStatus(QString styleSheet, const char* msg)
{

}
