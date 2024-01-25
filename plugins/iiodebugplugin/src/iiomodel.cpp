#include "iiomodel.h"
#include "iiostandarditem.h"
#include <QLoggingCategory>

#define BUFFER_SIZE 256
#define PATH_DELIMITER "/"

Q_LOGGING_CATEGORY(CAT_IIOMODEL, "IIOModel")
using namespace scopy::iiodebugplugin;

IIOModel::IIOModel(struct iio_context *context, QObject *parent)
	: QObject(parent)
	, m_ctx(context)
	, m_factory(new IIOWidgetFactory)
{
	m_model = new QStandardItemModel(this);
	iioTreeSetup();
}

QStandardItemModel *IIOModel::getModel() { return m_model; }

void IIOModel::iioTreeSetup()
{
	QString rootString = iio_context_get_name(m_ctx);
	QList<IIOWidget *> ctxList = m_factory->buildAllAttrsForContext(m_ctx);
	auto *rootItem = new IIOStandardItem(ctxList, rootString);
	rootItem->setEditable(false);

	for(auto & ctxWidget : ctxList) {
		m_entries.append(rootString + PATH_DELIMITER + ctxWidget->getRecipe().data);
		auto *attrItem = new IIOStandardItem({ctxWidget}, ctxWidget->getRecipe().data);
		attrItem->setEditable(false);
		rootItem->appendRow(attrItem);
	}

	// add all devices from context, dfs
	uint ctx_devices_count = iio_context_get_devices_count(m_ctx);
	for(int i = 0; i < ctx_devices_count; ++i) {
		struct iio_device *device = iio_context_get_device(m_ctx, i);
		QList<IIOWidget *> devList = m_factory->buildAllAttrsForDevice(device);
		QString device_name = iio_device_get_name(device);
		bool is_trigger = iio_device_is_trigger(device);
		IIOStandardItem *device_item;
		if(is_trigger) {
			device_item =
				new IIOStandardItem({}, device_name + " (trigger)");
		} else {
			device_item = new IIOStandardItem(devList, device_name);
		}
		device_item->setEditable(false);
		m_entries.append(rootString + PATH_DELIMITER + device_name);

		// add all attrs to current device
		for(int j = 0; j < devList.size(); ++j) {
			QString device_attr = iio_device_get_attr(device, j);

			m_entries.append(rootString + PATH_DELIMITER + device_name + PATH_DELIMITER + device_attr);
			auto *attrItem = new IIOStandardItem({devList[j]}, devList[j]->getRecipe().data);
			attrItem->setEditable(false);
			device_item->appendRow(attrItem);
		}

		// add all channels to current device
		uint device_channels_count = iio_device_get_channels_count(device);
		for(int j = 0; j < device_channels_count; ++j) {
			struct iio_channel *channel = iio_device_get_channel(device, j);
			QList<IIOWidget *> chnList = m_factory->buildAllAttrsForChannel(channel);
			QString channel_name = iio_channel_get_id(channel);
			auto *channel_item = new IIOStandardItem(chnList, channel_name);
			channel_item->setEditable(false);
			m_entries.append(rootString + PATH_DELIMITER + device_name + PATH_DELIMITER + channel_name);

			// add all attrs from channel
			for(int k = 0; k < chnList.size(); ++k) {
				QString attr_name = iio_channel_get_attr(channel, k);

				m_entries.append(rootString + PATH_DELIMITER + device_name + PATH_DELIMITER +
						 channel_name + PATH_DELIMITER + attr_name);
				auto *attr_item =
					new IIOStandardItem({chnList[k]}, chnList[k]->getRecipe().data);
				attr_item->setEditable(false);
				channel_item->appendRow(attr_item);
			}

			// add channel to device
			device_item->appendRow(channel_item);
		}

		// add device to ctx
		rootItem->appendRow(device_item);
	}

	m_model->appendRow(rootItem);
}

QStringList IIOModel::getEntries() { return m_entries; }
