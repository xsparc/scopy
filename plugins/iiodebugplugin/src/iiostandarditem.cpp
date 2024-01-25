#include "iiostandarditem.h"

using namespace scopy::iiodebugplugin;

IIOStandardItem::IIOStandardItem(QList<IIOWidget *> widgets, const QString &text)
	: QStandardItem(text)
	, m_iioWidgets(widgets)
{
	if(!m_iioWidgets.empty()) {
		auto widget = m_iioWidgets[0];
		// for now, details are only available for channel attributes
		struct iio_channel *channel = widget->getRecipe().channel;
		struct iio_device *device = widget->getRecipe().device;
		if(channel != nullptr) {
			bool scan_element = iio_channel_is_scan_element(channel);
			m_details.append((scan_element) ? "Is scan element." : "It is NOT scan element.");

			bool output = iio_channel_is_output(channel);
			m_details.append((output) ? "Is output." : "Is input.");

			bool is_enabled = iio_channel_is_enabled(channel);
			m_details.append((is_enabled) ? "Is enabled." : "Is not enabled.");

			QString filename =
				iio_channel_attr_get_filename(channel, widget->getRecipe().data.toStdString().c_str());
			m_details.append("Filename: " + filename);
		}

		if(device != nullptr) {
			bool is_hwmon = iio_device_is_hwmon(device);
			m_details.append((is_hwmon) ? "Device is hardware monitor."
						    : "Device is NOT hardware monitor.");

			bool is_trigger = iio_device_is_trigger(device);
			m_details.append((is_trigger) ? "Device is trigger." : "Device is NOT trigger.");
		}
	}
}

IIOStandardItem::~IIOStandardItem() {}

QList<scopy::IIOWidget *> IIOStandardItem::getIIOWidgets() { return m_iioWidgets; }

QStringList IIOStandardItem::getDetails() { return m_details; }
