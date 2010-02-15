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
#ifndef BLUETOOTHSTATUSINDICATOR_STUB
#define BLUETOOTHSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class BluetoothStatusIndicatorStub : public StubBase
{
public:
    virtual void BluetoothStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent);
    virtual void BluetoothStatusIndicatorDestructor();
    virtual void bluetoothChanged();
    ContextItem *bluetooth ;
};

// 2. IMPLEMENT STUB
void BluetoothStatusIndicatorStub::BluetoothStatusIndicatorConstructor(ApplicationContext &context, DuiWidget *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void BluetoothStatusIndicatorStub::BluetoothStatusIndicatorDestructor()
{

}
void BluetoothStatusIndicatorStub::bluetoothChanged()
{
    stubMethodEntered("bluetoothChanged");
}



// 3. CREATE A STUB INSTANCE
BluetoothStatusIndicatorStub gDefaultBluetoothStatusIndicatorStub;
BluetoothStatusIndicatorStub *gBluetoothStatusIndicatorStub = &gDefaultBluetoothStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
BluetoothStatusIndicator::BluetoothStatusIndicator(ApplicationContext &context, DuiWidget *parent)
{
    gBluetoothStatusIndicatorStub->BluetoothStatusIndicatorConstructor(context, parent);
}

BluetoothStatusIndicator::~BluetoothStatusIndicator()
{
    gBluetoothStatusIndicatorStub->BluetoothStatusIndicatorDestructor();
}

void BluetoothStatusIndicator::bluetoothChanged()
{
    gBluetoothStatusIndicatorStub->bluetoothChanged();
}


#endif
