#pragma once

#include <QAbstractTableModel>
#include <QHash>
#include <QByteArray>

namespace AddressStorageNs
{

class AddressStorageModel
        :   public QAbstractTableModel
{
    Q_OBJECT

public:

    enum AddressRoles {
            AddressRole = Qt::UserRole + 1,
            Count,
        };

    AddressStorageModel( QObject* _parent = nullptr );

    static void registerQmlType();


public:

    Q_INVOKABLE void addAddress( const QString& _address );

protected:

    int rowCount( const QModelIndex& _parent )const override;

    int columnCount(const QModelIndex& _parent)const override;

    QVariant data(const QModelIndex& _index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

private:

    static constexpr inline int RolesCount = 2;
    using TAddressStorage = QVector<QPair<QString,int>>;
    TAddressStorage m_storage;
};

} // namespace AddressStorageNs
