#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "track.h"
#include "trackscontroller.h"
#include "apicontroller.h"

int main(int argc, char *argv[]) {
    qRegisterMetaType<Track*>("Track*");
    qRegisterMetaType<TracksController*>("TracksController*");
    qmlRegisterUncreatableType<Track>("chachkouski.type", 1, 0, "track", "test");
    qmlRegisterUncreatableType<TracksController>("chachkouski.type", 1, 0, "trackC", "test");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    TracksService* tracksService = new TracksService(&app);
    TracksController* tracksC = new TracksController(tracksService, &app);
    ApiController* api = new ApiController(tracksService, &app);

    QQmlApplicationEngine engine;
    QQmlContext* rootContext = engine.rootContext();
    rootContext->setContextProperty("_api", api);
    rootContext->setContextProperty("_tracksService", tracksService);
    rootContext->setContextProperty("_tracksController", tracksC);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
