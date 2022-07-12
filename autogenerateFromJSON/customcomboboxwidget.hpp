#ifndef CUSTOMCOMBOBOXWIDGET_HPP
#define CUSTOMCOMBOBOXWIDGET_HPP

#include <QWidget>
#include <customwidgetinterface.hpp>
#include <qboxlayout.h>

class QComboBox;

class CustomComboBoxWidget : public CustomWidgetInterface
{
	Q_OBJECT

public:
	explicit CustomComboBoxWidget(const char * attr, QStringList available_values, bool readOnly, QLayout* layout = new QHBoxLayout(), QWidget *parent = nullptr);
	~CustomComboBoxWidget();

private:
	QWidget *widget;
	QComboBox *comboBox;
	QLayout *mainLayout = new QVBoxLayout();

	// CustomWidgetInterface interface
public:
	void updateValue(const char *val);
	QWidget *getWidget();

	// CustomWidgetInterface interface
public:
	void setStatus(QString styleSheet, const char* msg);
};

#endif // CUSTOMCOMBOBOXWIDGET_HPP
