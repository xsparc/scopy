#include "jsonparser.hpp"
#include "qdebug.h"

#include <iiowidget.hpp>

#include <chnattrreadwrite.hpp>
#include <devattrreadwrite.hpp>
#include <contextattrreadwrite.hpp>

#include <customcheckboxwidget.hpp>
#include <customcomboboxwidget.hpp>
#include <customedittextwidget.hpp>
#include <customlabelwidget.h>
#include <customspinboxwidget.hpp>

#include <qboxlayout.h>
#include <qfile.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qtabwidget.h>

JsonParser::JsonParser(struct iio_context *ctx, QObject *parent)
	:QObject{parent}
{
	this->ctx = ctx;
}

QVector<QWidget *> JsonParser::getWidgets(QString path)
{
	QVector<QWidget*> result;

	QString val;
	QFile file;
	file.setFileName(path);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	val = file.readAll();
	file.close();
	QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

	QJsonArray jsonArray = d.array();

	for (auto obj : jsonArray) {
		QWidget *widget = getWidgetFromJson(obj.toObject());

		if (widget) {
			result.push_back(widget);
		}
	}

	return result;
}

IioWidget *JsonParser::getIioWidgetFromJson(QJsonObject object)
{
	QJsonObject params = object.value(QString("params")).toObject();

	//create io based on JSON info
	QJsonObject io = params.value(QString("io")).toObject();
	ReadWriteInterface* readWrite = getReadWrite(io);

	double timer = 0;
	// if timer is needed get timer value
	if (io.value(QString("io_params")).toObject().contains("read_timer")) {
		QJsonValue read_timer = io.value(QString("io_params")).toObject()["read_timer"];
		timer = read_timer.toDouble();
	}

	// widget type and params
	QJsonObject widget = params.value(QString("io_widget")).toObject();
	CustomWidgetInterface* customWidget = getWidget(widget);

	return new IioWidget(customWidget,readWrite,timer);
}


QWidget *JsonParser::getWidgetFromJson(QJsonObject object)
{
	QWidget* result = new QWidget();

	if (object.contains("widget")) {

		QJsonObject widget = object.value(QString("widget")).toObject();

		QJsonObject params = widget.value(QString("params")).toObject();

		// apply layout if exists
		if (params.contains("layout")) {
			QString layout = params["layout"].toString();

			if (layout == "verticalLayout") {
				result->setLayout(new QVBoxLayout());
			}

			if (layout == "horizontalLayout") {
				result->setLayout(new QHBoxLayout());
			}
		} else {
			result->setLayout(new QHBoxLayout());
		}

		// apply stykesheet if exists
		if (params.contains("stylesheet")) {
			result->setStyleSheet(params["stylesheet"].toString());
		}

		QString type = widget["type"].toString();

		if (type == "empty") {
			return result;
		}

		// if widget type is iiowidget we draw the widget
		if (type == "iiowidget") {
			auto  iioWidget = getIioWidgetFromJson(widget)->getWidget();
			iioWidget->setStyleSheet(result->styleSheet());
			return iioWidget;
		}

		if (type == "tabwidget") {

			QTabWidget* tabs = new QTabWidget();
			tabs->setStyleSheet(result->styleSheet());

			//check if the widget contains other widgets
			//for tab widgets all containing widgets are it's tabs
			if (widget.contains("content")) {
				QJsonValue content = widget.value(QString("content"));

				for (auto aux : content.toArray()) {
					QString name = aux.toObject().value(QString("widget"))["name"].toString();
					tabs->addTab(getContent(aux.toObject(),result), name);
				}
			}
			return tabs;
		}
		// for other widget types that are not threated (like simple widget )
		return getContent(widget,result);
	}

	//if no known widget type is found return empty widget
	return result;
}


QWidget* JsonParser::getContent(QJsonObject object, QWidget* parent)
{

	QWidget* result = new QWidget(parent);

	//apply parent stylesheet and init layout as default value
	result->setStyleSheet(parent->styleSheet());
	result->setLayout(parent->layout());

	if (object.contains("content")) {
		// if the widget contains more widgets put all the conained widgets inside the widget
		QJsonValue content = object.value(QString("content"));
		if (content.isArray()) {
			for (auto widget : content.toArray()) {
				result->layout()->addWidget(getWidgetFromJson(widget.toObject()));
			}
		}
	} else {
		//if widget does not have content means we are at the deepest point in the json
		return getWidgetFromJson(object);
	}

	return result;
}

iio_device* JsonParser::getIioDevice(const char *dev_name)
{
	auto deviceCount = iio_context_get_devices_count(ctx);

	for (int i = 0 ; i < deviceCount; i++) {
		iio_device *dev = iio_context_get_device(ctx, i);
		if (strcmp(iio_device_get_name(dev), dev_name) == 0) {
			return dev;
		}
	}

	return nullptr;
}

iio_channel *JsonParser::getIioCh(iio_device *dev, const char *ch_name)
{
	auto chCount = iio_device_get_channels_count(dev);

	for (int i = 0 ; i < chCount; i++) {
		iio_channel *chn = iio_device_get_channel(dev, i);
		if (strcmp(iio_channel_get_id(chn), ch_name) == 0) {
			return chn;
		}
	}

	return nullptr;
}

ReadWriteInterface *JsonParser::getReadWrite(QJsonObject object)
{
	QString type = object["type"].toString();

	if (type ==  "iio_chan_attr" ) {
		return getChnReadWrite(object);
	}

	if (type == "iio_dev_attr") {
		return getDevReadWrite(object);
	}

	if (type == "iio_ctx_attr") {
		return getCtxReadWrite(object);
	}

	if (type == "cmd") {
		//TODO create rw based on console cmd
	}

	return nullptr;
}

ChnAttrReadWrite *JsonParser::getChnReadWrite(QJsonObject object)
{
	QJsonObject io_params = object.value(QString("io_params")).toObject();
	QJsonValue iio_dev = io_params["iio_dev"];
	iio_device* dev = getIioDevice(iio_dev.toString().toStdString().c_str());
	QJsonValue iio_chan = io_params["iio_chan"];
	iio_channel* chn = getIioCh(dev,iio_chan.toString().toStdString().c_str());
	QJsonValue iio_attr = io_params["iio_attr"];
	auto attr_s = iio_attr.toString()+ '\0';

	return new ChnAttrReadWrite(chn,attr_s);
}

DevAttrReadWrite *JsonParser::getDevReadWrite(QJsonObject object)
{
	QJsonObject io_params = object.value(QString("io_params")).toObject();
	QJsonValue iio_dev = io_params["iio_dev"];
	iio_device* dev = getIioDevice(iio_dev.toString().toStdString().c_str());
	QJsonValue iio_attr = io_params["iio_attr"];
	auto attr_s = iio_attr.toString()+ '\0';

	return new DevAttrReadWrite(dev,attr_s);
}

ContextAttrReadWrite *JsonParser::getCtxReadWrite(QJsonObject object)
{
	QJsonObject io_params = object.value(QString("io_params")).toObject();
	QJsonValue iio_attr = io_params["iio_attr"];
	auto attr_s = iio_attr.toString()+ '\0';

	return new ContextAttrReadWrite(ctx,attr_s);
}

CustomWidgetInterface *JsonParser::getWidget(QJsonObject object)
{
	QString widget_type = object["widget_type"].toString();

	if (widget_type == "checkbox") {
		return getCheckBoxWidget(object);
	}

	if (widget_type == "edittext") {
		return getEditTextWidget(object);
	}

	if (widget_type == "combobox") {
		return getComboBoxWidget(object);
	}

	if (widget_type == "spinbox") {
		return getSpinBoxWidget(object);
	}

	return getLabelWidget(object);
}

CustomCheckBoxWidget *JsonParser::getCheckBoxWidget(QJsonObject object)
{
	QJsonValue description = object.value(QString("description"));
	QJsonValue readonly = object.value(QString("readonly"));

	return new CustomCheckBoxWidget(description.toString().toStdString().c_str(),readonly.toBool());
}

CustomEditTextWidget *JsonParser::getEditTextWidget(QJsonObject object)
{
	QJsonValue description = object.value(QString("description"));
	QJsonValue readonly = object.value(QString("readonly"));

	return new CustomEditTextWidget(description.toString().toStdString().c_str(),readonly.toBool());
}

CustomComboBoxWidget *JsonParser::getComboBoxWidget(QJsonObject object)
{
	QJsonValue readonly = object.value(QString("readonly"));
	QJsonValue description = object.value(QString("description"));
	QJsonArray availableValuesList = object.value(QString("available_values")).toArray();
	QStringList available;

	for (auto value : availableValuesList) {
		available.append(value.toString());
	}

	return new CustomComboBoxWidget(description.toString().toStdString().c_str(),
					available, readonly.toBool());
}

CustomLabelWidget *JsonParser::getLabelWidget(QJsonObject object)
{
	QJsonValue description = object.value(QString("description"));
	return new CustomLabelWidget(description.toString().toStdString().c_str());
}

CustomSpinBoxWidget *JsonParser::getSpinBoxWidget(QJsonObject object)
{
	QJsonValue description = object.value(QString("description"));
	QJsonValue min_range = object.value(QString("min_range"));
	QJsonValue max_range = object.value(QString("max_range"));
	QJsonValue step = object.value(QString("step"));
	QJsonValue readonly = object.value(QString("readonly"));
	QJsonValue type = object.value(QString("type"));

	return new CustomSpinBoxWidget(description.toString().toStdString().c_str(),
				       min_range.toString().toStdString().c_str(),
				       max_range.toString().toStdString().c_str(),
				       step.toString().toStdString().c_str(),
				       type.toString(),
				       readonly.toBool());
}

