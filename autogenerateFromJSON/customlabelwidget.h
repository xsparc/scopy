#ifndef CUSTOMLABELWIDGET_H
#define CUSTOMLABELWIDGET_H

#include <QWidget>
#include <customwidgetinterface.hpp>
#include <qboxlayout.h>


class QLabel;

class CustomLabelWidget : public CustomWidgetInterface
{
	Q_OBJECT

public:
	CustomLabelWidget(const char * attr, QString imgPath, QLayout* layout = new QHBoxLayout(), QWidget *parent = nullptr);
	CustomLabelWidget(const char * attr, QLayout* layout = new QHBoxLayout(), QWidget *parent = nullptr);
	~CustomLabelWidget();

private:
	QWidget *widget;
	QLabel *value;
	QLayout *mainLayout = new QVBoxLayout();

	// CustomWidgetInterface interface
public:
	void updateValue(const char *val);
	QWidget *getWidget();

	// CustomWidgetInterface interface
public:
	void setStatus(QString styleSheet, const char* msg);
};

#endif // CUSTOMLABELWIDGET_H
