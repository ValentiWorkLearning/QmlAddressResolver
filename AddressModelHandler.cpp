#include "AddressModelHandler.hpp"

#include <QQmlEngine>
#include <format>

namespace AddressModelHandlerNs
{

AddressModelHandler::AddressModelHandler(QObject* _parent)
    :   QObject{_parent}
    ,   m_selectedItemsCount{}
{
}

void AddressModelHandler::setItemSelected(int _itemIndex, int _itemValue)
{

    auto firstEmpty = std::ranges::find_if(
                m_addressRecords,
                [](const auto& _item){ return !_item.component.has_value(); }
    );

    if(firstEmpty == m_addressRecords.end())
        return;

    firstEmpty->itemIndex = _itemIndex;
    firstEmpty->component = _itemValue;

    std::size_t addresPosition = std::distance(m_addressRecords.begin(),firstEmpty);
    emit onItemHiglhightSetRequested(_itemIndex,addresPosition);
}

void AddressModelHandler::setItemDeselected(int _itemIndex)
{
    --m_selectedItemsCount;
    auto requiredNode = std::ranges::find_if(
                m_addressRecords,
                [_itemIndex](const auto& _item){ return _item.itemIndex == _itemIndex; }
    );

    requiredNode->itemIndex.reset();
    requiredNode->component.reset();

    std::size_t addressDistance = std::distance(m_addressRecords.begin(), requiredNode);
    emit onItemHiglhightResetRequested(_itemIndex,addressDistance);
}

void AddressModelHandler::onItemClicked(int _itemIndex, int _itemValue)
{
    auto requiredNode = std::ranges::find_if(
                m_addressRecords,
                [_itemIndex](const auto& _item){ return _item.itemIndex == _itemIndex; }
    );

    if(requiredNode == m_addressRecords.end())
        setItemSelected(_itemIndex,_itemValue);
    else
        setItemDeselected(_itemIndex);
}

void AddressModelHandler::onItemEditFinished(QString _itemValue,int _itemIndex)
{
    if(_itemValue.isEmpty())
    {
        auto indexValueOpt = m_addressRecords.at(_itemIndex).itemIndex;
        if( indexValueOpt.has_value())
        {
            if(const auto& itemIndexOpt = m_addressRecords.at(_itemIndex).itemIndex; itemIndexOpt.has_value())
            {
                emit onItemHiglhightResetRequested(itemIndexOpt.value(),_itemIndex);
            }
             m_addressRecords.at(_itemIndex).itemIndex.reset();
             m_addressRecords.at(_itemIndex).component.reset();

        }
    }
    else
    {
        int value { _itemValue.toInt()};
        auto& editableRecord{m_addressRecords.at(_itemIndex)};

        if(editableRecord.itemIndex.has_value() && !checkRepeatedValues(value))
            setItemDeselected(editableRecord.itemIndex.value());

        editableRecord.component = value;
        editableRecord.itemIndex = value;

        emit onItemHiglhightSetRequested(value,_itemIndex);
    }
}

bool AddressModelHandler::checkRepeatedValues(int _valueToCheck)
{
    int repeatedValues = std::ranges::count_if(
                m_addressRecords,
                [_valueToCheck](const auto& _item)
    {
        return _item.component.has_value() && _item.component.value() == _valueToCheck;
    }

    );
    return repeatedValues > 1;
}


bool AddressModelHandler::canAcceptClicks() const noexcept
{
    return !std::ranges::all_of(
                m_addressRecords,
                [](const auto& _item){ return _item.component.has_value(); }
    );
}

void AddressModelHandler::clearAll()
{
    std::ranges::for_each(
        m_addressRecords,
        [this](const auto& item)
        {
            if(item.itemIndex.has_value())
                setItemDeselected(item.itemIndex.value());
        }
    );
    emit clearAllSig();
}

bool AddressModelHandler::containsValue(int _itemIndex)const
{

    auto itemIt = std::ranges::find_if(
                m_addressRecords,
                [_itemIndex](const auto& _item)
                {
                    return _item.component.has_value() && _item.component.value() == _itemIndex;
                }
    );
    return itemIt != m_addressRecords.end();
}

bool AddressModelHandler::isAddressValid() const
{
    return !canAcceptClicks();
}

QString AddressModelHandler::getCurrentAddress()const
{
    std::string currentAddress = std::format(
                "{0}:{1}:{2}:{3}:{4}",
            m_addressRecords[0].component.value(),
            m_addressRecords[1].component.value(),
            m_addressRecords[2].component.value(),
            m_addressRecords[3].component.value(),
            m_addressRecords[4].component.value()
    );
    return QString::fromStdString(currentAddress);
}

void AddressModelHandler::registerQmlType()
{
    qmlRegisterType<AddressModelHandler>("AddressModelModule", 1, 0, "AddressModelHandler");
}

}
