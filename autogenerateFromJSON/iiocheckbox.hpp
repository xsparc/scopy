#ifndef IIOCHECKBOX_HPP
#define IIOCHECKBOX_HPP

#include <QObject>
#include <readwriteinterface.hpp>
#include <customcheckboxwidget.hpp>

class IioCheckBox : public QObject
{
	Q_OBJECT
public:
	explicit IioCheckBox(ReadWriteInterface *readWrite, bool readOnly, QObject *parent = nullptr);

	CustomCheckBoxWidget* getWidget();

private:
	ReadWriteInterface *readWrite;
	CustomCheckBoxWidget *checkBoxWidget;
	bool readOnly;


};

#endif // IIOCHECKBOX_HPP
