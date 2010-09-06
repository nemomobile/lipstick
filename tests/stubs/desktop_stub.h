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

#ifndef DESKTOP_STUB
#define DESKTOP_STUB

#include "desktop.h"
#include <stubbase.h>

// 1. DECLARE STUB
class DesktopStub : public StubBase {
public:
    virtual void DesktopConstructor(MWidget *parent);
    virtual void DesktopDestructor();
};

// 2. IMPLEMENT STUB
void DesktopStub::DesktopConstructor(MWidget *parent) {
    QList<ParameterBase*> params;
    params.append(new Parameter<MWidget*>(parent));
    stubMethodEntered("Constructor(MWidget)", params);
}

void DesktopStub::DesktopDestructor() {
    stubMethodEntered("Destructor");
}

// 3. CREATE A STUB INSTANCE
DesktopStub gDefaultDesktopStub;
DesktopStub* gDesktopStub = &gDefaultDesktopStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
Desktop::Desktop(MWidget *parent) {
    gDesktopStub->DesktopConstructor(parent);
}

Desktop::~Desktop() {
    gDesktopStub->DesktopDestructor();
}


#endif
