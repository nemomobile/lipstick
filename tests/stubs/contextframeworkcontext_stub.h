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
#ifndef CONTEXTFRAMEWORKCONTEXT_STUB
#define CONTEXTFRAMEWORKCONTEXT_STUB

#include "contextframeworkcontext.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ContextFrameworkContextStub : public StubBase
{
public:
    virtual ContextItem *createContextItem(const QString &key);
};

// 2. IMPLEMENT STUB
ContextItem *ContextFrameworkContextStub::createContextItem(const QString &key)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(key));
    stubMethodEntered("createContextItem", params);
    return stubReturnValue<ContextItem *>("createContextItem");
}



// 3. CREATE A STUB INSTANCE
ContextFrameworkContextStub gDefaultContextFrameworkContextStub;
ContextFrameworkContextStub *gContextFrameworkContextStub = &gDefaultContextFrameworkContextStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ContextItem *ContextFrameworkContext::createContextItem(const QString &key)
{
    return gContextFrameworkContextStub->createContextItem(key);
}


#endif
