/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef _QMUSBMODE_STUB_H
#define _QMUSBMODE_STUB_H

#include <QObject>

MeeGo::QmUSBMode::Mode testMode = MeeGo::QmUSBMode::Undefined;
MeeGo::QmUSBMode::Mode testDefaultMode = MeeGo::QmUSBMode::Undefined;

MeeGo::QmUSBMode::QmUSBMode(QObject *parent)
{
    Q_UNUSED (parent);
}

MeeGo::QmUSBMode::~QmUSBMode()
{
}

MeeGo::QmUSBMode::Mode MeeGo::QmUSBMode::getMode()
{
    return testMode;
}

bool MeeGo::QmUSBMode::setMode(MeeGo::QmUSBMode::Mode mode)
{
    testMode = mode;
    return true;
}

bool MeeGo::QmUSBMode::setDefaultMode(MeeGo::QmUSBMode::Mode mode)
{
    testDefaultMode = mode;
    return true;
}

MeeGo::QmUSBMode::Mode MeeGo::QmUSBMode::getDefaultMode()
{
    return testDefaultMode;
}

void MeeGo::QmUSBMode::connectNotify(const char *)
{
}
void MeeGo::QmUSBMode::disconnectNotify(const char *)
{
}
#endif
