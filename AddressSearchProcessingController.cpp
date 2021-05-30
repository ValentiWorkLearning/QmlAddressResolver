#include "AddressSearchProcessingController.hpp"
#include "ProcessingCore.hpp"

#include <QDebug>

namespace AddressSearchProcessingNs
{
SearchProcessingController::SearchProcessingController(QObject* _parent) : QObject(_parent)
{
    m_listModel = new DistanceListModel(this);
}

void SearchProcessingController::startProcessing()
{
    qDebug() << "SearchProcessingController::startProcessing()\n";
    qDebug() << "Processing address:" << m_searchFor;

    auto workerThread = new QThread();
    ProcessingCore* pProcessingCore{new ProcessingCore(m_processingList, m_searchFor)};
    pProcessingCore->moveToThread(workerThread);

    connect(workerThread, &QThread::started, pProcessingCore, &ProcessingCore::startSearch);
    connect(pProcessingCore, &ProcessingCore::onResultsReady, [this](const auto& results) {
        m_results = results;
        emit onNewResultsArrived();
    });
    connect(
        this,
        &SearchProcessingController::onNewResultsArrived,
        this,
        &SearchProcessingController::refreshModel,
        Qt::QueuedConnection);
    connect(
        pProcessingCore,
        &ProcessingCore::onComputationCompleted,
        pProcessingCore,
        &ProcessingCore::deleteLater,
        Qt::QueuedConnection);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);

    workerThread->start();
}

void SearchProcessingController::refreshModel()
{
    m_listModel->storeNewResults(m_results);
}

void SearchProcessingController::registerQmlType()
{
    qmlRegisterType<SearchProcessingController>(
        "app.address_processing", 1, 0, "SearchProcessingController");
}

DistanceListModel* SearchProcessingController::foundWordsModel() const
{
    return m_listModel;
}

} // namespace AddressSearchProcessingNs
