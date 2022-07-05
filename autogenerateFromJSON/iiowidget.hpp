#ifndef IIOWIDGET_HPP
#define IIOWIDGET_HPP


#include <QObject>
#include "customwidgetinterface.hpp"
#include "readwriteinterface.hpp"

class IioWidget : public QObject
{
	Q_OBJECT
public:
	explicit IioWidget(CustomWidgetInterface *customWidget, ReadWriteInterface *readWrite, QObject *parent = nullptr);
	~IioWidget();

	QWidget* getWidget();
signals:

private:
	CustomWidgetInterface *customWidget;
	ReadWriteInterface *readWrite;
};

#endif // IIOWIDGET_HPP
