// Copyright (c) 2012, Jolla Mobile
// <insert closed source header here>

#include <QFile>

#include <homeapplication.h>

int main(int argc, char *argv[])
{
    if (QFile::exists("main.qml")) {
        HomeApplication app(argc, argv, "main.qml");
        return app.exec();
    } else {
        HomeApplication app(argc, argv, "qrc:/qml/main.qml");
        return app.exec();
    }
}
