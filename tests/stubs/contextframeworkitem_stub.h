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
#ifndef CONTEXTFRAMEWORKITEM_STUB
#define CONTEXTFRAMEWORKITEM_STUB

#include "contextframeworkcontext.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ContextFrameworkItemStub : public StubBase
{
public:
    virtual void ContextFrameworkItemConstructor(const QString &key);
    virtual QVariant value() const;
    ContextProperty property ;
};

// 2. IMPLEMENT STUB
void ContextFrameworkItemStub::ContextFrameworkItemConstructor(const QString &key)
{
    Q_UNUSED(key);

}
QVariant ContextFrameworkItemStub::value() const
{
    stubMethodEntered("value");
    return stubReturnValue<QVariant>("value");
}



// 3. CREATE A STUB INSTANCE
ContextFrameworkItemStub gDefaultContextFrameworkItemStub;
ContextFrameworkItemStub *gContextFrameworkItemStub = &gDefaultContextFrameworkItemStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ContextFrameworkItem::ContextFrameworkItem(const QString &key)
{
    gContextFrameworkItemStub->ContextFrameworkItemConstructor(key);
}

QVariant ContextFrameworkItem::value() const
{
    return gContextFrameworkItemStub->value();
}


#endif
