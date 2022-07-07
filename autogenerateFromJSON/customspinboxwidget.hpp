#ifndef CUSTOMSPINBOXWIDGET_HPP
#define CUSTOMSPINBOXWIDGET_HPP

#include <QWidget>
#include <customwidgetinterface.hpp>

namespace Ui {
class CustomSpinBoxWidget;
}

class CustomSpinBoxWidget : public CustomWidgetInterface
{
	Q_OBJECT

public:
	explicit CustomSpinBoxWidget(const char * attr, const char * min_range, const char * max_range, const char * step,
				     QString type, bool readOnly, QWidget *parent = nullptr);
	~CustomSpinBoxWidget();

private:
	Ui::CustomSpinBoxWidget *ui;
	QTimer* timer;
	bool isInt;

	// CustomWidgetInterface interface
public:
	void updateValue(const char *val);
	QWidget *getWidget();
	void giveFeedback(bool interaction, const char* msg);
};

#endif // CUSTOMSPINBOXWIDGET_HPP
