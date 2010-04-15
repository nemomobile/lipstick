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

#ifndef QUICKLAUNCHBAR_STUB
#define QUICKLAUNCHBAR_STUB

#include "quicklaunchbar.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class QuickLaunchBarStub : public StubBase {
  public:
  virtual void QuickLaunchBarConstructor(QGraphicsItem *parent);
  virtual void QuickLaunchBarConstructor(LauncherDataStore* configuration, QGraphicsItem *parent);
  virtual void QuickLaunchBarDestructor();
  virtual void launchApplication(const QString &application);
  virtual void launchMApplication(const QString &service);
  virtual void initializeDataStore();
  virtual void updateWidgetList();
};

// 2. IMPLEMENT STUB
void QuickLaunchBarStub::QuickLaunchBarConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);
}

void QuickLaunchBarStub::QuickLaunchBarConstructor(LauncherDataStore *configuration, QGraphicsItem *parent) {
    Q_UNUSED(configuration);
    Q_UNUSED(parent);
}

void QuickLaunchBarStub::QuickLaunchBarDestructor() {

}
void QuickLaunchBarStub::launchApplication(const QString &application) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(application));
  stubMethodEntered("launchApplication",params);
}

void QuickLaunchBarStub::launchMApplication(const QString &service) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(service));
  stubMethodEntered("launchMApplication",params);
}

void QuickLaunchBarStub::initializeDataStore() {
  stubMethodEntered("initializeDataStore");
}

void QuickLaunchBarStub::updateWidgetList() {
  stubMethodEntered("updateWidgetList");
}



// 3. CREATE A STUB INSTANCE
QuickLaunchBarStub gDefaultQuickLaunchBarStub;
QuickLaunchBarStub* gQuickLaunchBarStub = &gDefaultQuickLaunchBarStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
QuickLaunchBar::QuickLaunchBar(QGraphicsItem *parent) {
  gQuickLaunchBarStub->QuickLaunchBarConstructor(parent);
}

QuickLaunchBar::QuickLaunchBar(LauncherDataStore *configuration, QGraphicsItem *parent) {
  gQuickLaunchBarStub->QuickLaunchBarConstructor(configuration, parent);
}

QuickLaunchBar::~QuickLaunchBar() {
  gQuickLaunchBarStub->QuickLaunchBarDestructor();
}

void QuickLaunchBar::launchApplication(const QString &application) {
  gQuickLaunchBarStub->launchApplication(application);
}

void QuickLaunchBar::launchMApplication(const QString &service) {
  gQuickLaunchBarStub->launchMApplication(service);
}

void QuickLaunchBar::initializeDataStore() {
  gQuickLaunchBarStub->initializeDataStore();
}

void QuickLaunchBar::updateWidgetList() {
  gQuickLaunchBarStub->updateWidgetList();
}


#endif
