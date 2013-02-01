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
#ifndef QMLED_STUB
#define QMLED_STUB

#include <qmled.h>
#include "stubbase.h"

/*
 * Declare stub
 */
class QmLEDStub : public StubBase
{
public:
    virtual void activate (const QString pattern);
    virtual void deactivate (const QString pattern);
    virtual void enable ();
    virtual void disable ();
};

void
QmLEDStub::activate (const QString pattern)
{
    QList<ParameterBase*> params;
    params.append (new Parameter<QString> (pattern));
    stubMethodEntered ("activate", params);
}

void
QmLEDStub::deactivate (const QString pattern)
{
    QList<ParameterBase*> params;
    params.append (new Parameter<QString> (pattern));
    stubMethodEntered ("deactivate", params);
}

void
QmLEDStub::enable ()
{
    stubMethodEntered ("enable");
}

void
QmLEDStub::disable ()
{
    stubMethodEntered ("disable");
}

/*
 * Create a stub instance
 */
QmLEDStub gDefaultQmLEDStub;
QmLEDStub *gQmLEDStub = &gDefaultQmLEDStub;

/*
 * Create a proxy which calls the stub
 */
namespace MeeGo
{

QmLED::QmLED (QObject *parent)
{
    Q_UNUSED (parent);
    /*
     * Do nothing, we don't want to instantiate a real QmLED...
     */
    priv_ptr = 0;
}

QmLED::~QmLED ()
{

}

bool
QmLED::activate (const QString &pattern)
{
    gQmLEDStub->activate (pattern);
    return true;
}

bool
QmLED::deactivate (const QString &pattern)
{
    gQmLEDStub->deactivate (pattern);
    return true;
}

bool
QmLED::enable ()
{
    gQmLEDStub->enable ();
    return true;
}

bool
QmLED::disable ()
{
    gQmLEDStub->disable ();
    return true;
}

} /* namespace MeeGo */

#endif
