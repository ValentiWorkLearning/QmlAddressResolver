#ifndef FILEPROCESSOR_HPP
#define FILEPROCESSOR_HPP

#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QThread>
#include <QTimer>

#include "WordListModel.hpp"

namespace AddressSearchProcessingNs
{
class SearchProcessingController : public QObject
{

    Q_OBJECT

    Q_PROPERTY(
        QVector<QString> processingAddressList MEMBER m_processingList NOTIFY processingListChanged)
    Q_PROPERTY(QString processingAddress MEMBER m_searchFor NOTIFY processingAddressChanged)
    Q_PROPERTY(DistanceListModel* foundWordsModel READ foundWordsModel CONSTANT)

public:
    Q_INVOKABLE void startProcessing();

    QML_ELEMENT

public:
    SearchProcessingController(QObject* _parent = nullptr);

public:
    static void registerQmlType();

public:
    DistanceListModel* foundWordsModel() const;

signals:

    void processingListChanged();
    void processingAddressChanged();

private:
signals:
    void onNewResultsArrived();

private slots:
    void refreshModel();

private:
    QVector<QString> m_processingList;
    QString m_searchFor;
    DistanceListModel* m_listModel;
    QList<std::shared_ptr<WordItem>> m_results;
};

} // namespace AddressSearchProcessingNs
#endif // FILEPROCESSOR_HPP
