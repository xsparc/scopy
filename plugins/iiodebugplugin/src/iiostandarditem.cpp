#include "iiostandarditem.h"

using namespace scopy::iiodebugplugin;

IIOStandardItem::IIOStandardItem(struct iio_context *context, const QString &text, IIOStandardItemType type)
	: QStandardItem(text)
	, m_itemModel(new IIOStandardItemModel(type))
	, m_itemView(new IIOStandardItemView(type))
	, m_factory(new IIOWidgetFactory())
	, m_context(context)
{
	if(type == IIOStandardItemType::Context) {
		m_iioWidgets = m_factory->buildAllAttrsForContext(context);
	} else if(type == IIOStandardItemType::ContextAttribute) {
		m_iioWidgets.append(m_factory->buildSingle(IIOWidgetFactory::EditableUi |
								   IIOWidgetFactory::InstantSave |
								   IIOWidgetFactory::ContextAttrData,
							   {.context = context, .data = text}));
	}
}

IIOStandardItem::IIOStandardItem(struct iio_device *device, const QString &text, IIOStandardItemType type)
	: QStandardItem(text)
	, m_itemModel(new IIOStandardItemModel(type))
	, m_itemView(new IIOStandardItemView(type))
	, m_factory(new IIOWidgetFactory())
	, m_device(device)
{
	if(type == IIOStandardItemType::Device) {
		m_iioWidgets = m_factory->buildAllAttrsForDevice(device);
	} else if(type == IIOStandardItemType::DeviceAttribute) {
		m_iioWidgets.append(m_factory->buildSingle(
			IIOWidgetFactory::EditableUi | IIOWidgetFactory::InstantSave | IIOWidgetFactory::DeviceAttrData,
			{.device = device, .data = text}));
	}
}

IIOStandardItem::IIOStandardItem(struct iio_channel *channel, const QString &text, IIOStandardItemType type)
	: QStandardItem(text)
	, m_itemModel(new IIOStandardItemModel(type))
	, m_itemView(new IIOStandardItemView(type))
	, m_factory(new IIOWidgetFactory())
	, m_channel(channel)
{
	if(type == IIOStandardItemType::Channel) {
		m_iioWidgets = m_factory->buildAllAttrsForChannel(channel);
	} else if(type == IIOStandardItemType::ChannelAttribute) {
		m_iioWidgets.append(m_factory->buildSingle(
			IIOWidgetFactory::EditableUi | IIOWidgetFactory::InstantSave | IIOWidgetFactory::AttrData,
			{.channel = channel, .data = text}));
	}
}

IIOStandardItem::~IIOStandardItem()
{
	delete m_itemModel;
	delete m_itemView;
	delete m_factory;
}

QList<scopy::IIOWidget *> IIOStandardItem::getIIOWidgets() { return m_iioWidgets; }
