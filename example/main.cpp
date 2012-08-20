
// This file is part of lipstick, a QML desktop library
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// Copyright (c) 2011, Robin Burchell
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>

#include <QDebug>
#include <homeapplication.h>

int main(int argc, char *argv[])
{
    // We don't need the meego graphics system here
    QApplication::setGraphicsSystem("native");
    HomeApplication app(argc, argv, "qrc:/qml/main.qml");

    qDebug() << "Good morning! This is the lipstick example app! :)";

    return app.exec();
}
