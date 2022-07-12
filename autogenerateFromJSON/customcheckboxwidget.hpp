#ifndef CUSTOMCHECKBOXWIDGET_HPP
#define CUSTOMCHECKBOXWIDGET_HPP

#include <QWidget>
#include <customwidgetinterface.hpp>
#include <qboxlayout.h>
#include <qcheckbox.h>

class CustomCheckBoxWidget : public CustomWidgetInterface
{
	Q_OBJECT

public:
	explicit CustomCheckBoxWidget(const char * attr, bool readOnly, QWidget *parent = nullptr);
	~CustomCheckBoxWidget();

private:
	QWidget *widget;
	QCheckBox *checkBox;
	QLayout *mainLayout;

	// CustomWidget interface
public:
	void updateValue(const char *val);
	QWidget* getWidget();
	bool readOnly;

	// CustomWidgetInterface interface
public:
	void setStatus(QString styleSheet, const char* msg);
};

#endif // CUSTOMCHECKBOXWIDGET_HPP
