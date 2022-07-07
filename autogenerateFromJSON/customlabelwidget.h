#ifndef CUSTOMLABELWIDGET_H
#define CUSTOMLABELWIDGET_H

#include <QWidget>
#include <customwidgetinterface.hpp>

namespace Ui {
class CustomLabelWidget;
}

class CustomLabelWidget : public CustomWidgetInterface
{
	Q_OBJECT

public:
	explicit CustomLabelWidget(const char * attr, QWidget *parent = nullptr);
	~CustomLabelWidget();

private:
	Ui::CustomLabelWidget *ui;

	// CustomWidgetInterface interface
public:
	void updateValue(const char *val);
	QWidget *getWidget();

	// CustomWidgetInterface interface
public:
	void giveFeedback(bool interaction, const char* msg);
};

#endif // CUSTOMLABELWIDGET_H
