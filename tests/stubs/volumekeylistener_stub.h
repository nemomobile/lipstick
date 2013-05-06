/***************************************************************************
**
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
#ifndef VOLUMEKEYLISTENER_STUB
#define VOLUMEKEYLISTENER_STUB

#include "volumekeylistener.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class VolumeKeyListenerStub : public StubBase {
  public:
  virtual void VolumeKeyListenerConstructor(QObject *parent);
}; 

// 2. IMPLEMENT STUB
void VolumeKeyListenerStub::VolumeKeyListenerConstructor(QObject *parent) {
  Q_UNUSED(parent);

}



// 3. CREATE A STUB INSTANCE
VolumeKeyListenerStub gDefaultVolumeKeyListenerStub;
VolumeKeyListenerStub* gVolumeKeyListenerStub = &gDefaultVolumeKeyListenerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
VolumeKeyListener::VolumeKeyListener(QObject *parent) {
  gVolumeKeyListenerStub->VolumeKeyListenerConstructor(parent);
}


#endif
