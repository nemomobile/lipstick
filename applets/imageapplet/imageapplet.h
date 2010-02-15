/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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
#ifndef IMAGEAPPLET_H
#define IMAGEAPPLET_H

#include <duiappletinterface.h>
#include <duiappletmetadata.h>
#include <QObject>

class DuiDataStore;
class DuiDataAccess;

// This class doesn't do anything else but creates widgets inside this library
class ImageApplet : public QObject, public DuiAppletInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiAppletInterface)

public: // methods derived from DuiAppletInterface

    virtual DuiWidget *constructWidget(const DuiAppletMetaData &metadata, DuiDataStore &instanceData, DuiDataAccess &appletSettings);
};

#endif // IMAGEAPPLET_H
