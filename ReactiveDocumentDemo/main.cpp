#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "reactivetexthelper.h"



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ReactiveTextHelper>("com.eleks.com.qmlcomponents", 1, 0, "ReactiveText");
    qmlRegisterType<ReactiveTextModel>("com.eleks.com.qmlcomponents", 1, 0, "ReactiveTextModel");


    QQmlApplicationEngine engine;

    Property propertyInstance;

    engine.rootContext()->setContextProperty("Property", &propertyInstance);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
