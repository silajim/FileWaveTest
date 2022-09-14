#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "applicationui.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    ApplicationUI ui;

    return app.exec();
}
