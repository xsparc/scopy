#include "iiomodel.h"
#include "iiostandarditem.h"
#include <QLoggingCategory>

#define BUFFER_SIZE 256
#define SEPARATOR "/"

Q_LOGGING_CATEGORY(CAT_IIOMODEL, "IIOModel")
using namespace scopy::iiodebugplugin;

IIOModel::IIOModel(struct iio_context *context, QString uri, QObject *parent)
	: QObject(parent)
	, m_ctx(context)
	, m_uri(uri)
{
	m_model = new QStandardItemModel(this);
	m_model->setHorizontalHeaderItem(0, new QStandardItem("IIO Tree"));
	iioTreeSetup();
}

QStandardItemModel *IIOModel::getModel() { return m_model; }

void IIOModel::iioTreeSetup()
{
	QString rootString = m_uri;
	QList<IIOWidget *> ctxList = IIOWidgetFactory::buildAllAttrsForContext(m_ctx);
	auto *rootItem = new IIOStandardItem(ctxList, rootString, rootString);
	rootItem->setEditable(false);

	for(auto &ctxWidget : ctxList) {
		m_entries.insert(ctxWidget->getRecipe().data);
		auto *attrItem = new IIOStandardItem({ctxWidget}, ctxWidget->getRecipe().data,
						     rootString + SEPARATOR + ctxWidget->getRecipe().data);
		attrItem->setEditable(false);
		rootItem->appendRow(attrItem);
	}

	// add all devices from context, dfs
	uint ctx_devices_count = iio_context_get_devices_count(m_ctx);
	for(int i = 0; i < ctx_devices_count; ++i) {
		struct iio_device *device = iio_context_get_device(m_ctx, i);
		QList<IIOWidget *> devList = IIOWidgetFactory::buildAllAttrsForDevice(device);
		QString device_name = iio_device_get_name(device);
		bool is_trigger = iio_device_is_trigger(device);
		IIOStandardItem *device_item;
		if(is_trigger) {
			// IIOWidget *triggerWidget = IIOWidgetFactory::buildSingle(IIOWidgetFactory::TriggerData |
			// IIOWidgetFactory::ComboUi, {.device = device, .data = ""});
			device_item = new IIOStandardItem({}, device_name + " (trigger)",
							  rootString + SEPARATOR + device_name);
		} else {
			device_item = new IIOStandardItem(devList, device_name, rootString + SEPARATOR + device_name);
		}
		device_item->setEditable(false);
		m_entries.insert(device_name);

		// add all attrs to current device
		for(int j = 0; j < devList.size(); ++j) {
			QString device_attr = iio_device_get_attr(device, j);

			m_entries.insert(device_attr);
			auto *attrItem =
				new IIOStandardItem({devList[j]}, devList[j]->getRecipe().data,
						    rootString + SEPARATOR + device_name + SEPARATOR + device_attr);
			attrItem->setEditable(false);
			device_item->appendRow(attrItem);
		}

		// add all channels to current device
		uint device_channels_count = iio_device_get_channels_count(device);
		for(int j = 0; j < device_channels_count; ++j) {
			struct iio_channel *channel = iio_device_get_channel(device, j);
			QList<IIOWidget *> chnList = IIOWidgetFactory::buildAllAttrsForChannel(channel);
			QString channel_name = iio_channel_get_id(channel);
			auto *channel_item = new IIOStandardItem(
				chnList, channel_name, rootString + SEPARATOR + device_name + SEPARATOR + channel_name);
			channel_item->setEditable(false);
			m_entries.insert(channel_name);

			// add all attrs from channel
			for(int k = 0; k < chnList.size(); ++k) {
				QString attr_name = iio_channel_get_attr(channel, k);

				m_entries.insert(attr_name);
				auto *attr_item = new IIOStandardItem({chnList[k]}, chnList[k]->getRecipe().data,
								      rootString + SEPARATOR + device_name + SEPARATOR +
									      channel_name + SEPARATOR + attr_name);
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

QSet<QString> IIOModel::getEntries() { return m_entries; }
