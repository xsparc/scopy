#ifndef CUSTOMCOMBOBOXWIDGET_HPP
#define CUSTOMCOMBOBOXWIDGET_HPP

#include <QWidget>
#include <customwidgetinterface.hpp>

namespace Ui {
class CustomComboBoxWidget;
}

class CustomComboBoxWidget : public CustomWidgetInterface
{
	Q_OBJECT

public:
	explicit CustomComboBoxWidget(const char * attr, QStringList available_values, bool readOnly,QWidget *parent = nullptr);
	~CustomComboBoxWidget();

private:
	Ui::CustomComboBoxWidget *ui;

	// CustomWidgetInterface interface
public:
	void updateValue(const char *val);
	QWidget *getWidget();
};

#endif // CUSTOMCOMBOBOXWIDGET_HPP
