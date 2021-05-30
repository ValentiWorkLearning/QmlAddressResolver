#ifndef WORDS_LIST_MODEL_HPP
#define WORDS_LIST_MODEL_HPP

#include <QAbstractListModel>
#include <QQmlEngine>

namespace AddressSearchProcessingNs
{
class WordItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString word MEMBER m_word CONSTANT)
    Q_PROPERTY(QString closeToStandard MEMBER m_closeTo  READ getCloseToStandard CONSTANT)
public:
    WordItem(const QString& _word, int _closeTo, QObject* _parent = nullptr)
        : QObject{_parent}, m_word{_word}, m_closeTo{QString::number(_closeTo)}
    {
    }

    WordItem(const QString& _word, const QString& _closeTo, QObject* _parent = nullptr)
        : QObject{_parent}, m_word{_word}, m_closeTo{_closeTo}
    {
    }
    static void registerQmlType()
    {
        qmlRegisterUncreatableType<WordItem>("app.address_list_model", 1, 0, "WordItem", "interface");
    }

public:

    QString getCloseToStandard()const
    {
        return m_closeTo;
    }

private:
    QString m_word;
    QString m_closeTo;
};
Q_DECLARE_METATYPE(WordItem*);

class DistanceListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    DistanceListModel(QObject* _parent = nullptr) : QAbstractListModel{_parent}
    {
    }

public:
    Q_INVOKABLE WordItem* getWordItem(int _index)
    {
        const bool isIndexValid = (_index >= 0) && (_index <= m_wordList.size() - 1);
        if (isIndexValid)
        {
            const auto& item = m_wordList[_index];
            QQmlEngine::setObjectOwnership(item.get(), QQmlEngine::CppOwnership);
            return item.get();
        }
        return nullptr;
    }

public:
    static void registerQmlType()
    {
        qmlRegisterUncreatableType<DistanceListModel>(
            "app.address_list_model", 1, 0, "DistanceListModel", "interface");
    }

    int rowCount(const QModelIndex& _parent) const override
    {
        return m_wordList.count();
    }

    void storeNewResults(const QList<std::shared_ptr<WordItem>>& _list)
    {
        beginResetModel();
        m_wordList.clear();
        m_wordList = _list;
        endResetModel();
    }
    QVariant data(const QModelIndex& _index, int _role) const override
    {
        return QVariant();
    }

private:
    QList<std::shared_ptr<WordItem>> m_wordList;
};

Q_DECLARE_METATYPE(DistanceListModel*);

} // namespace AddressSearchProcessingNs
#endif // WORDS_LIST_MODEL_HPP
