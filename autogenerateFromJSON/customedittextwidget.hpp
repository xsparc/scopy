#ifndef CUSTOMEDITTEXTWIDGET_HPP
#define CUSTOMEDITTEXTWIDGET_HPP

#include <QWidget>
#include <customwidgetinterface.hpp>

namespace Ui {
class CustomEditTextWidget;
}

class CustomEditTextWidget : public CustomWidgetInterface
{
	Q_OBJECT

public:
	explicit CustomEditTextWidget(const char * attr, bool readOnly,QWidget *parent = nullptr);
	~CustomEditTextWidget();

private:
	Ui::CustomEditTextWidget *ui;

	// CustomWidgetInterface interface
public:
	void updateValue(const char *val);
	QWidget *getWidget();
	bool readOnly;
};

#endif // CUSTOMEDITTEXTWIDGET_HPP
