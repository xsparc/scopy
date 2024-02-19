#include "iiostandarditem.h"

using namespace scopy::iiodebugplugin;

IIOStandardItem::IIOStandardItem(QList<IIOWidget *> widgets, const QString &title, const QString &fullTitle)
	: QStandardItem(title)
	, m_iioWidgets(widgets)
	, m_title(title)
	, m_fullTitle(fullTitle)
	, m_isWatched(false)
{
	if(!m_iioWidgets.empty()) {
		auto widget = m_iioWidgets[0];
		// for now, details are only available for channel attributes
		struct iio_channel *channel = widget->getRecipe().channel;
		struct iio_device *device = widget->getRecipe().device;
		if(channel != nullptr) {
			bool scan_element = iio_channel_is_scan_element(channel);
			m_details.append((scan_element) ? "This channel is scan element."
							: "This channel is NOT scan element.");

			bool output = iio_channel_is_output(channel);
			m_details.append((output) ? "This channel is output." : "This channel is input.");

			bool is_enabled = iio_channel_is_enabled(channel);
			m_details.append((is_enabled) ? "This channel is enabled." : "This channel is not enabled.");

			QString filename =
				iio_channel_attr_get_filename(channel, widget->getRecipe().data.toStdString().c_str());
			m_details.append("Channel attribute filename: " + filename);
		}

		if(device != nullptr) {
			bool is_hwmon = iio_device_is_hwmon(device);
			m_details.append((is_hwmon) ? "This device is hardware monitor."
						    : "This device is NOT hardware monitor.");

			bool is_trigger = iio_device_is_trigger(device);
			m_details.append((is_trigger) ? "This device is trigger." : "This device is NOT trigger.");
		}
	}
}

IIOStandardItem::~IIOStandardItem() = default;

QList<scopy::IIOWidget *> IIOStandardItem::getIIOWidgets() { return m_iioWidgets; }

QStringList IIOStandardItem::details() { return m_details; }

QString IIOStandardItem::title() { return m_title; }

QString IIOStandardItem::fullTitle() { return m_fullTitle; }

bool IIOStandardItem::isWatched() { return m_isWatched; }

void IIOStandardItem::setWatched(bool isWatched) { m_isWatched = isWatched; }
