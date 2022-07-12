#ifndef CUSTOMEDITTEXTWIDGET_HPP
#define CUSTOMEDITTEXTWIDGET_HPP

#include <QWidget>
#include <customwidgetinterface.hpp>
#include <qboxlayout.h>

class QLineEdit;
class QPushButton;

class CustomEditTextWidget : public CustomWidgetInterface
{
	Q_OBJECT

public:
	explicit CustomEditTextWidget(const char * attr, bool readOnly, QLayout* layout = new QHBoxLayout(), QWidget *parent = nullptr);
	~CustomEditTextWidget();

private:
	QWidget *widget;
	QLineEdit *value;
	QPushButton *updateButton;
	QLayout *mainLayout = new QVBoxLayout();

	// CustomWidgetInterface interface
public:
	void updateValue(const char *val);
	QWidget *getWidget();
	bool readOnly;

	// CustomWidgetInterface interface
public:
	void setStatus(QString styleSheet, const char* msg);
};

#endif // CUSTOMEDITTEXTWIDGET_HPP
