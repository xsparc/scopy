#ifndef IIOWIDGET_HPP
#define IIOWIDGET_HPP


#include <QObject>
#include "customwidgetinterface.hpp"
#include "readwriteinterface.hpp"

class IioWidget : public QObject
{
	Q_OBJECT
public:
	explicit IioWidget(CustomWidgetInterface *customWidget, ReadWriteInterface *readWrite, double read_timer, QObject *parent = nullptr);
	~IioWidget();

	QWidget* getWidget();
signals:

private:
	CustomWidgetInterface *customWidget;
	ReadWriteInterface *readWrite;
	QTimer* timer;
};

#endif // IIOWIDGET_HPP
