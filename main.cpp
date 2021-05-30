#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>

#include "AddressModelHandler.hpp"
#include "AddressSearchProcessingController.hpp"
#include "AddressStorage.hpp"
#include "WordListModel.hpp"

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QLoggingCategory::setFilterRules(QStringLiteral("qt.qml.binding.removal.info=true"));

    QQmlApplicationEngine engine;

    AddressModelHandlerNs::AddressModelHandler::registerQmlType();
    AddressStorageNs::AddressStorageModel::registerQmlType();

    AddressSearchProcessingNs::WordItem::registerQmlType();
    AddressSearchProcessingNs::SearchProcessingController::registerQmlType();

    const QUrl url(QStringLiteral("qrc:/qml_ui/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
