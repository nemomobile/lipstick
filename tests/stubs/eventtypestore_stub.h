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
#ifndef EVENTTYPESTORE_STUB
#define EVENTTYPESTORE_STUB

#include "eventtypestore.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class EventTypeStoreStub : public StubBase
{
public:
    virtual void EventTypeStoreConstructor(const QString &eventTypesPath, uint maxStoredEventTypes);
    virtual const QSettings *settingsForEventType(const QString &eventType) const;
    virtual void loadSettings(const QString &eventType);
};

// 2. IMPLEMENT STUB
void EventTypeStoreStub::EventTypeStoreConstructor(const QString &eventTypesPath, uint maxStoredEventTypes)
{
    Q_UNUSED(eventTypesPath);
    Q_UNUSED(maxStoredEventTypes);

}
const QSettings *EventTypeStoreStub::settingsForEventType(const QString &eventType) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(eventType));
    stubMethodEntered("settingsForEventType", params);
    return stubReturnValue<const QSettings *>("settingsForEventType");
}

void EventTypeStoreStub::loadSettings(const QString &eventType)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(eventType));
    stubMethodEntered("loadSettings", params);
}



// 3. CREATE A STUB INSTANCE
EventTypeStoreStub gDefaultEventTypeStoreStub;
EventTypeStoreStub *gEventTypeStoreStub = &gDefaultEventTypeStoreStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
EventTypeStore::EventTypeStore(const QString &eventTypesPath, uint maxStoredEventTypes)
{
    gEventTypeStoreStub->EventTypeStoreConstructor(eventTypesPath, maxStoredEventTypes);
}

const QSettings *EventTypeStore::settingsForEventType(const QString &eventType) const
{
    return gEventTypeStoreStub->settingsForEventType(eventType);
}

void EventTypeStore::loadSettings(const QString &eventType)
{
    gEventTypeStoreStub->loadSettings(eventType);
}


#endif
