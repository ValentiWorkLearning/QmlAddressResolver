#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLoggingCategory>

#include "AddressModelHandler.hpp"
#include "AddressStorage.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QLoggingCategory::setFilterRules(QStringLiteral("qt.qml.binding.removal.info=true"));

    QQmlApplicationEngine engine;

   AddressModelHandlerNs::AddressModelHandler::registerQmlType();
   AddressStorageNs::AddressStorageModel::registerQmlType();

    const QUrl url(QStringLiteral("qrc:/qml_ui/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
