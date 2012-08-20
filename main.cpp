// Copyright (c) 2012, Jolla Mobile
// <insert closed source header here>

#include <QDebug>
#include <homeapplication.h>

int main(int argc, char *argv[])
{
    HomeApplication app(argc, argv, "qrc:/qml/main.qml");
    return app.exec();
}
