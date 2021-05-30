#pragma once

#include <QObject>

#include <array>
#include <optional>
#include <ranges>

namespace AddressModelHandlerNs
{

class AddressModelHandler : public QObject
{

    Q_OBJECT

public:
    AddressModelHandler(QObject* _parent = nullptr);

    Q_INVOKABLE void setItemSelected(int _itemIndex, int _itemValue);

    Q_INVOKABLE void setItemDeselected(int _itemIndex);

    Q_INVOKABLE bool canAcceptClicks() const noexcept;

    Q_INVOKABLE void onItemClicked(int _itemIndex, int _itemValue);

    Q_INVOKABLE void onItemEditFinished(QString _itemValue, int _itemIndex);

    Q_INVOKABLE bool isAddressValid() const;

    Q_INVOKABLE bool containsValue(int _itemIndex) const;

    Q_INVOKABLE QString getCurrentAddress() const;

    Q_INVOKABLE QString getAvailableAddressParts()const;

    Q_INVOKABLE void clearAll();

public:
    static void registerQmlType();

public:
signals:
    void onItemHiglhightSetRequested(int _itemIndex, int _addressPosition);

    void onItemHiglhightResetRequested(int _itemIndex, int _addressPosition);

    void clearAllSig();

private:
    bool checkRepeatedValues(int _valueToCheck);

private:
    int m_selectedItemsCount;

    struct AddressRecord
    {
        std::optional<int> itemIndex;
        std::optional<int> component;
    };
    static constexpr inline std::size_t AddressSize = 5;

    std::array<AddressRecord, AddressSize> m_addressRecords;
};

} // namespace AddressModelHandlerNs
