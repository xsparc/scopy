#ifndef CUSTOMWIDGETINTERFACE_H
#define CUSTOMWIDGETINTERFACE_H

#include <QObject>
#include <qwidget.h>

class CustomWidgetInterface: public QWidget
{
	Q_OBJECT
public:
	CustomWidgetInterface();

public:
	virtual void updateValue(const char *val)=0;
	virtual QWidget* getWidget()=0;
	bool readOnly;
signals:
	void valueChanged(const char *val);
};

#endif // CUSTOMWIDGETINTERFACE_H
