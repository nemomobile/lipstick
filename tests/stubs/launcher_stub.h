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
#ifndef LAUNCHER_STUB
#define LAUNCHER_STUB

#include "launcher.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LauncherStub : public StubBase {
  public:
  virtual void LauncherConstructor(LauncherDataStore *dataStore, QGraphicsItem *parent);
  virtual void LauncherDestructor();
  virtual bool startApplication(const QString &application);
  virtual bool startMApplication(const QString &serviceName);
  virtual void updatePagesFromDataStore();
  virtual int panToPage(const QString &fileEntryPath);
  virtual QSharedPointer<LauncherButton> createLauncherButton(const QString &desktopEntryPath);
  virtual QMap<Launcher::Placement, QString> createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements);
}; 

// 2. IMPLEMENT STUB
void LauncherStub::LauncherConstructor(LauncherDataStore *dataStore, QGraphicsItem *parent) {
  Q_UNUSED(dataStore);
  Q_UNUSED(parent);

}
void LauncherStub::LauncherDestructor() {

}
bool LauncherStub::startApplication(const QString &application) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(application));
  stubMethodEntered("startApplication",params);
  return stubReturnValue<bool>("startApplication");
}

bool LauncherStub::startMApplication(const QString &serviceName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(serviceName));
  stubMethodEntered("startMApplication",params);
  return stubReturnValue<bool>("startMApplication");
}

void LauncherStub::updatePagesFromDataStore() {
  stubMethodEntered("updatePagesFromDataStore");
}

QSharedPointer<LauncherButton> LauncherStub::createLauncherButton(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("createLauncherButton",params);
  return stubReturnValue<QSharedPointer<LauncherButton> >("createLauncherButton");
}

QMap<Launcher::Placement, QString> LauncherStub::createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QHash<QString, QVariant> & >(desktopEntryPlacements));
  stubMethodEntered("createPlacementMap",params);
  return stubReturnValue<QMap<Launcher::Placement, QString> >("createPlacementMap");
}

int LauncherStub::panToPage(const QString &fileEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(fileEntryPath));
  stubMethodEntered("panToPage",params);
  return stubReturnValue<int>("panToPage");
}

// 3. CREATE A STUB INSTANCE
LauncherStub gDefaultLauncherStub;
LauncherStub* gLauncherStub = &gDefaultLauncherStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Launcher::Launcher(LauncherDataStore *dataStore, QGraphicsItem *parent) {
  gLauncherStub->LauncherConstructor(dataStore, parent);
}

Launcher::~Launcher() {
  gLauncherStub->LauncherDestructor();
}

bool Launcher::startApplication(const QString &application) {
  return gLauncherStub->startApplication(application);
}

bool Launcher::startMApplication(const QString &serviceName) {
  return gLauncherStub->startMApplication(serviceName);
}

void Launcher::updatePagesFromDataStore() {
  gLauncherStub->updatePagesFromDataStore();
}

QSharedPointer<LauncherButton> Launcher::createLauncherButton(const QString &desktopEntryPath) {
  return gLauncherStub->createLauncherButton(desktopEntryPath);
}

QMap<Launcher::Placement, QString> Launcher::createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements) {
  return gLauncherStub->createPlacementMap(desktopEntryPlacements);
}

int Launcher::panToPage(const QString &fileEntryPath) {
  return gLauncherStub->panToPage(fileEntryPath);
}

#endif
