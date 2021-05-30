#include "AddressStorage.hpp"

#include <QQmlEngine>

#include <ranges>

namespace AddressStorageNs
{

AddressStorageModel::AddressStorageModel(QObject* _parent) : QAbstractTableModel{_parent}
{
}

int AddressStorageModel::rowCount(const QModelIndex& _parent) const
{
    Q_UNUSED(_parent)
    return m_storage.size();
}

int AddressStorageModel::columnCount(const QModelIndex& _parent) const
{
    Q_UNUSED(_parent)
    return AddressStorageModel::RolesCount;
}

QHash<int, QByteArray> AddressStorageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AddressRoles::AddressRole] = "address";
    roles[AddressRoles::Count] = "count";
    return roles;
}

QVariant AddressStorageModel::data(const QModelIndex& _index, int role) const
{
    switch (role)
    {
    case AddressRoles::AddressRole:
        return m_storage[_index.row()].first;
        break;
    case AddressRoles::Count:
        return m_storage[_index.row()].second;
        break;
    }
    return QVariant{};
}

void AddressStorageModel::addAddress(const QString& _address)
{
    auto addressIt = std::ranges::find_if(m_storage, [&_address](const auto& _itemToCheck) {
        const auto& [itemAddress, count] = _itemToCheck;
        return itemAddress == _address;
    });

    if (addressIt == m_storage.end())
    {
        beginInsertRows(QModelIndex{}, m_storage.size(), m_storage.size());
        m_storage.push_back({_address, 1});
        endInsertRows();
    }
    else
    {
        std::size_t itemIndex = std::distance(m_storage.begin(), addressIt);
        ++addressIt->second;
        emit dataChanged(
            index(itemIndex, 0), index(itemIndex, AddressStorageModel::RolesCount - 1));
    }
}

QVector<QString> AddressStorageModel::getAddressList() const
{
    QVector<QString> result;
    std::ranges::transform(m_storage, std::back_inserter(result), [](const auto& _item) {
        const auto& [address, count] = _item;
        return address;
    });
    return result;
}

void AddressStorageModel::registerQmlType()
{
    qmlRegisterType<AddressStorageModel>("app.address.storage", 1, 0, "AddressStorageModel");
}

} // namespace AddressStorageNs
