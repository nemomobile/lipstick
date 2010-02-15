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
#ifndef NOTIFIER_STUB
#define NOTIFIER_STUB

#include "notifier.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotifierStub : public StubBase
{
public:
    virtual void NotifierConstructor(DuiWidget *parent);
    virtual void NotifierDestructor();
    virtual void notificationCountChanged(uint count);
};

// 2. IMPLEMENT STUB
void NotifierStub::NotifierConstructor(DuiWidget *parent)
{
    Q_UNUSED(parent);

}
void NotifierStub::NotifierDestructor()
{

}
void NotifierStub::notificationCountChanged(uint count)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(count));
    stubMethodEntered("notificationCountChanged", params);
}



// 3. CREATE A STUB INSTANCE
NotifierStub gDefaultNotifierStub;
NotifierStub *gNotifierStub = &gDefaultNotifierStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Notifier::Notifier(DuiWidget *parent)
{
    gNotifierStub->NotifierConstructor(parent);
}

Notifier::~Notifier()
{
    gNotifierStub->NotifierDestructor();
}

void Notifier::notificationCountChanged(uint count)
{
    gNotifierStub->notificationCountChanged(count);
}


#endif
