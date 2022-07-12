#ifndef CUSTOMSPINBOXWIDGET_HPP
#define CUSTOMSPINBOXWIDGET_HPP

#include <QWidget>
#include <customwidgetinterface.hpp>
#include <qboxlayout.h>
#include <qpushbutton.h>

class QDoubleSpinBox;

class CustomSpinBoxWidget : public CustomWidgetInterface
{
	Q_OBJECT

public:
	explicit CustomSpinBoxWidget(const char * attr, const char * min_range, const char * max_range, const char * step,
				     QString type, bool readOnly, QLayout* layout = new QHBoxLayout(), QWidget *parent = nullptr);
	~CustomSpinBoxWidget();

private:
	QWidget *widget;
	QDoubleSpinBox *value;
	QLayout *mainLayout = new QVBoxLayout();

	QPushButton *pushButton;
	QTimer* timer;
	bool isInt;

	// CustomWidgetInterface interface
public:
	void updateValue(const char *val);
	QWidget *getWidget();
	void setStatus(QString styleSheet, const char* msg);
};

#endif // CUSTOMSPINBOXWIDGET_HPP
