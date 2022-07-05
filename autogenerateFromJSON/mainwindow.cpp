#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include <chnattrreadwrite.hpp>
#include <iio.h>
#include <iiocheckbox.hpp>
#include <iiowidget.hpp>
#include <jsonparser.hpp>
#include <qboxlayout.h>
#include <qlabel.h>
#include "qdebug.h"

// TODO set the right buf size
#define BUF_SIZE 16384

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);



	ui->centralwidget->setLayout(new QVBoxLayout());



	ctx = iio_create_context_from_uri("ip:192.168.2.1");

	if (!ctx) {
		qDebug()<< "Connection Error: No device available/connected to your PC.";
	}


	JsonParser *parser = new JsonParser(ctx);
//	QVector<IioWidget*> objList =  parser->parse("///home/ionut/autogenerateFromJSON/demo.json");

//	for (auto obj : objList) {
//		ui->centralwidget->layout()->addWidget(obj->getWidget());
//	}

	QVector<QWidget*> widgetList =  parser->getWidgets("///home/ionut/autogenerateFromJSON/jsonLogic.json");

	for (auto w : widgetList) {
		ui->centralwidget->layout()->addWidget(w);
	}
	//auto deviceCount = iio_context_get_devices_count(ctx);
	/*

	auto deviceCount = iio_context_get_devices_count(ctx);
	char *buf = (char*) malloc(BUF_SIZE);

	for (int i = 0 ; i < deviceCount; i++) {
		const struct iio_device *dev = iio_context_get_device(ctx, i);

		unsigned int nb_channels = iio_device_get_channels_count(dev);

		for (int j = 0; j < nb_channels; j++) {

			auto ch = iio_device_get_channel(dev, j);

			const char *type_name;

			if (iio_channel_is_output(ch))
				type_name = "output";
			else
				type_name = "input";


			unsigned int nb_attrs = iio_channel_get_attrs_count(ch);


			QVector<QString> attributes;

			QLabel *channelId = new QLabel();
			channelId->setText(iio_device_get_name(dev) + QString::fromStdString(iio_channel_get_id(ch)) + " " + type_name );

			ui->centralwidget->layout()->addWidget(channelId);

			for (int k = 0; k < nb_attrs; k++) {
				const char *attr = iio_channel_get_attr(ch, k);
				auto ret = (int) iio_channel_attr_read(ch, attr, buf, 100);
				// if the element is enable type
				if (QString::fromStdString(attr).contains("_en")) {
					IioCheckBox *checkBox = new IioCheckBox(new ChnAttrReadWrite(ch,attr,this),this);
					ui->centralwidget->layout()->addWidget(checkBox->getWidget());

					//if the element has available values
				} else  {
					attributes.push_back(QString::fromStdString("attr ") + attr + " = " + buf);

				}

			}



			for(auto attribute : attributes){
				QLabel *attr = new QLabel();
				attr->setText( "                " + attribute );

				//layout->addWidget(attr);
			}

		}
	}
	*/



}

MainWindow::~MainWindow()
{
	iio_context_destroy(ctx);
	delete ui;
}

