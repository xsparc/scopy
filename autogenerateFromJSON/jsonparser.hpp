#ifndef JSONPARSER_HPP
#define JSONPARSER_HPP

#include "iiowidget.hpp"

#include <chnattrreadwrite.hpp>
#include <customcheckboxwidget.hpp>
#include <customcomboboxwidget.hpp>
#include <customedittextwidget.hpp>
#include <customlabelwidget.h>
#include <customspinboxwidget.hpp>
#include <iio.h>
#include <qobject.h>
#include <qstring.h>



class JsonParser: public QObject
{
	Q_OBJECT
public:
	JsonParser(struct iio_context *ctx, QObject *parent = nullptr);

	QVector<IioWidget*> parse(QString path);

	QVector<QWidget*> getWidgets(QString path);

	iio_device* getIioDevice(const char* dev_name);
	iio_channel* getIioCh(iio_device* dev, const char* ch_name);



	ReadWriteInterface* getReadWrite(QJsonObject object);
	ChnAttrReadWrite* getChnReadWrite(QJsonObject object);

	CustomWidgetInterface* getWidget(QJsonObject object);
	CustomCheckBoxWidget* getCheckBoxWidget(QJsonObject object);
	CustomEditTextWidget* getEditTextWidget(QJsonObject object);
	CustomComboBoxWidget* getComboBoxWidget(QJsonObject object);
	CustomLabelWidget* getLabelWidget(QJsonObject object);
	CustomSpinBoxWidget* getSpinBoxWidget(QJsonObject object);

	IioWidget* getIioWidgetFromJson(QJsonObject object);
	QWidget* getWidgetFromJson(QJsonObject object);


	QWidget* getContent(QJsonObject object, QWidget* parent);

private:
	struct iio_context *ctx;
};

#endif // JSONPARSER_HPP
