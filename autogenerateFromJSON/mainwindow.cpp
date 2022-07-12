#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include <QProcess>
#include <chnattrreadwrite.hpp>
#include <iio.h>
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

	QVector<QWidget*> widgetList =  parser->getWidgets("///home/ionut/autogenerateFromJSON/demoOneItem.json");

	for (auto w : widgetList) {
		ui->centralwidget->layout()->addWidget(w);
	}

//	QProcess *testProcess = new QProcess();

//	connect(testProcess, &QProcess::readyReadStandardOutput, [=]() {
//	       QString output = testProcess->readAllStandardOutput();
//	       qDebug() << "output is " << output;
//	   });
//	   connect(testProcess, &QProcess::readyReadStandardError, [=]() {
//	       QString err = testProcess->readAllStandardError();
//	       qDebug() << "error: " << err;
//	   });

//	   QStringList arguments;
//	   arguments<< "--uri" << "ip:192.168.2.1" << "-i" << "-c"  << "ad9361-phy" << "voltage0" << "hardwaregain" << "10";

//	   testProcess->start("iio_attr",arguments);

}

MainWindow::~MainWindow()
{
	iio_context_destroy(ctx);
	delete ui;
}

