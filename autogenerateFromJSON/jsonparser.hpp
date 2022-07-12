#ifndef JSONPARSER_HPP
#define JSONPARSER_HPP

#include <QLayout>
#include <iio.h>
#include <qobject.h>
#include <qstring.h>

class IioWidget;

class ReadWriteInterface;
class ChnAttrReadWrite;
class DevAttrReadWrite;
class ContextAttrReadWrite;
class CmdReadWrite;

class CustomWidgetInterface;
class CustomCheckBoxWidget;
class CustomEditTextWidget;
class CustomComboBoxWidget;
class CustomLabelWidget;
class CustomSpinBoxWidget;

class JsonParser: public QObject
{
	Q_OBJECT
public:
	JsonParser(struct iio_context *ctx, QObject *parent = nullptr);

	QVector<QWidget*> getWidgets(QString path);

	iio_device* getIioDevice(const char* dev_name);
	iio_channel* getIioCh(iio_device* dev, const char* ch_name);

	ReadWriteInterface* getReadWrite(QJsonObject object);
	ChnAttrReadWrite* getChnReadWrite(QJsonObject object);
	DevAttrReadWrite* getDevReadWrite(QJsonObject object);
	ContextAttrReadWrite* getCtxReadWrite(QJsonObject object);
	CmdReadWrite* getCmdReadWrite(QJsonObject object);

	CustomWidgetInterface* getWidget(QJsonObject object);
	CustomCheckBoxWidget* getCheckBoxWidget(QJsonObject object);
	CustomEditTextWidget* getEditTextWidget(QJsonObject object);
	CustomComboBoxWidget* getComboBoxWidget(QJsonObject object);
	CustomLabelWidget* getLabelWidget(QJsonObject object);
	CustomSpinBoxWidget* getSpinBoxWidget(QJsonObject object);

	IioWidget* getIioWidgetFromJson(QJsonObject object);
	QWidget* getWidgetFromJson(QJsonObject object);


	QWidget* getContent(QJsonObject object, QWidget* parent);

	QLayout* getLayout(QJsonObject object);

private:
	struct iio_context *ctx;
};

#endif // JSONPARSER_HPP
