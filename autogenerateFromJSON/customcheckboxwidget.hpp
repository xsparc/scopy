#ifndef CUSTOMCHECKBOXWIDGET_HPP
#define CUSTOMCHECKBOXWIDGET_HPP

#include <QWidget>
#include <customwidgetinterface.hpp>

namespace Ui {
class CustomCheckBoxWidget;
}

class CustomCheckBoxWidget : public CustomWidgetInterface
{
	Q_OBJECT

public:
	explicit CustomCheckBoxWidget(const char * attr, bool readOnly, QWidget *parent = nullptr);
	~CustomCheckBoxWidget();

private:
	Ui::CustomCheckBoxWidget *ui;



	// CustomWidget interface
public:
	void updateValue(const char *val);
	QWidget* getWidget();
	bool readOnly;
};

#endif // CUSTOMCHECKBOXWIDGET_HPP
