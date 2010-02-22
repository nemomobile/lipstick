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

#ifndef LAUNCHER_STUB
#define LAUNCHER_STUB

#include "launcher.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LauncherStub : public StubBase {
  //! A type for a container that can hold DuiDesktopEntry objects
  typedef QList<DuiDesktopEntry *> DesktopEntryContainer;
  public:
  virtual void LauncherConstructor(DuiWidget *parent);
  virtual void LauncherDestructor();
  virtual void setEnabled(bool enabled);
  virtual bool startApplication(const QString &application);
  virtual bool startDuiApplication(const QString &serviceName);
  virtual void openRootCategory();
  virtual void readDirectory(const QString &path, bool updateWidgetList);
  virtual void launchApplication(const QString &application);
  virtual void launchDuiApplication(const QString &service);
  virtual void launchLink(const QString &link);
  virtual void launchDirectory(const QString &directory, const QString &title, const QString &iconId);
  virtual void initializeIfNecessary();
  virtual void setCategory(const QString &category, const QString &title, const QString &iconId);
  virtual DuiWidget * createLauncherButton(const DuiDesktopEntry &entry);
  virtual void updateDesktopEntryList(DesktopEntryContainer &desktopEntryContainer, const QString &path, const QString &nameFilter, const QStringList &acceptedTypes) const;
#ifdef ENABLE_QTTRACKER
  virtual DuiWidget * createShortcutLauncherButton(SopranoLive::LiveNode shortcut);
#endif
  virtual void updateWidgetList();
}; 

// 2. IMPLEMENT STUB
void LauncherStub::LauncherConstructor(DuiWidget *parent) {
  Q_UNUSED(parent);

}
void LauncherStub::LauncherDestructor() {

}
void LauncherStub::setEnabled(bool enabled) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(enabled));
  stubMethodEntered("setEnabled",params);
}

bool LauncherStub::startApplication(const QString &application) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(application));
  stubMethodEntered("startApplication",params);
  return stubReturnValue<bool>("startApplication");
}

bool LauncherStub::startDuiApplication(const QString &serviceName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(serviceName));
  stubMethodEntered("startDuiApplication",params);
  return stubReturnValue<bool>("startDuiApplication");
}

void LauncherStub::openRootCategory() {
  stubMethodEntered("openRootCategory");
}

void LauncherStub::readDirectory(const QString &path, bool updateWidgetList) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(path));
  params.append( new Parameter<bool >(updateWidgetList));
  stubMethodEntered("readDirectory",params);
}

void LauncherStub::launchApplication(const QString &application) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(application));
  stubMethodEntered("launchApplication",params);
}

void LauncherStub::launchDuiApplication(const QString &service) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(service));
  stubMethodEntered("launchDuiApplication",params);
}

void LauncherStub::launchLink(const QString &link) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(link));
  stubMethodEntered("launchLink",params);
}

void LauncherStub::launchDirectory(const QString &directory, const QString &title, const QString &iconId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(directory));
  params.append( new Parameter<const QString & >(title));
  params.append( new Parameter<const QString & >(iconId));
  stubMethodEntered("launchDirectory",params);
}

void LauncherStub::initializeIfNecessary() {
  stubMethodEntered("initializeIfNecessary");
}

void LauncherStub::setCategory(const QString &category, const QString &title, const QString &iconId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(category));
  params.append( new Parameter<const QString & >(title));
  params.append( new Parameter<const QString & >(iconId));
  stubMethodEntered("setCategory",params);
}

DuiWidget * LauncherStub::createLauncherButton(const DuiDesktopEntry &entry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiDesktopEntry & >(entry));
  stubMethodEntered("createLauncherButton",params);
  return stubReturnValue<DuiWidget *>("createLauncherButton");
}

void LauncherStub::updateDesktopEntryList(DesktopEntryContainer &desktopEntryContainer, const QString &path, const QString &nameFilter, const QStringList &acceptedTypes) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<DesktopEntryContainer & >(desktopEntryContainer));
  params.append( new Parameter<const QString & >(path));
  params.append( new Parameter<const QString & >(nameFilter));
  params.append( new Parameter<const QStringList & >(acceptedTypes));
  stubMethodEntered("updateDesktopEntryList",params);
}

#ifdef ENABLE_QTTRACKER
DuiWidget * LauncherStub::createShortcutLauncherButton(SopranoLive::LiveNode shortcut) {
  QList<ParameterBase*> params;
  params.append( new Parameter<SopranoLive::LiveNode >(shortcut));
  stubMethodEntered("createShortcutLauncherButton",params);
  return stubReturnValue<DuiWidget *>("createShortcutLauncherButton");
}
#endif

void LauncherStub::updateWidgetList() {
  stubMethodEntered("updateWidgetList");
}



// 3. CREATE A STUB INSTANCE
LauncherStub gDefaultLauncherStub;
LauncherStub* gLauncherStub = &gDefaultLauncherStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Launcher::Launcher(DuiWidget *parent) {
  gLauncherStub->LauncherConstructor(parent);
}

Launcher::~Launcher() {
  gLauncherStub->LauncherDestructor();
}

void Launcher::setEnabled(bool enabled) {
  gLauncherStub->setEnabled(enabled);
}

bool Launcher::startApplication(const QString &application) {
  return gLauncherStub->startApplication(application);
}

bool Launcher::startDuiApplication(const QString &serviceName) {
  return gLauncherStub->startDuiApplication(serviceName);
}

void Launcher::openRootCategory() {
  gLauncherStub->openRootCategory();
}

void Launcher::readDirectory(const QString &path, bool updateWidgetList) {
  gLauncherStub->readDirectory(path, updateWidgetList);
}

void Launcher::launchApplication(const QString &application) {
  gLauncherStub->launchApplication(application);
}

void Launcher::launchDuiApplication(const QString &service) {
  gLauncherStub->launchDuiApplication(service);
}

void Launcher::launchLink(const QString &link) {
  gLauncherStub->launchLink(link);
}

void Launcher::launchDirectory(const QString &directory, const QString &title, const QString &iconId) {
  gLauncherStub->launchDirectory(directory, title, iconId);
}

void Launcher::initializeIfNecessary() {
  gLauncherStub->initializeIfNecessary();
}

void Launcher::setCategory(const QString &category, const QString &title, const QString &iconId) {
  gLauncherStub->setCategory(category, title, iconId);
}

DuiWidget * Launcher::createLauncherButton(const DuiDesktopEntry &entry) {
  return gLauncherStub->createLauncherButton(entry);
}

void Launcher::updateDesktopEntryList(DesktopEntryContainer &desktopEntryContainer, const QString &path, const QString &nameFilter, const QStringList &acceptedTypes) const {
  gLauncherStub->updateDesktopEntryList(desktopEntryContainer, path, nameFilter, acceptedTypes);
}

#ifdef ENABLE_QTTRACKER
DuiWidget * Launcher::createShortcutLauncherButton(SopranoLive::LiveNode shortcut) {
  return gLauncherStub->createShortcutLauncherButton(shortcut);
}
#endif

void Launcher::updateWidgetList() {
  gLauncherStub->updateWidgetList();
}


#endif
