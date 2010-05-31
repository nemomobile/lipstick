/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef WEATHERAPPLET_H
#define WEATHERAPPLET_H

#include <MAppletInterface>
#include <QObject>

// This class doesn't do anything else but creates widgets inside this library
class WeatherApplet : public QObject, public MAppletInterface
{
    Q_OBJECT
    Q_INTERFACES(MAppletInterface)

public:
    // methods derived from MAppletInterface
    virtual QGraphicsWidget *constructWidget(const MAppletMetaData &metaData, MDataStore &instanceData, MDataAccess &settings);
};

#endif // WEATHERAPPLET_H
