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
  virtual void LauncherConstructor(QGraphicsItem *parent);
  virtual void LauncherDestructor();
  virtual void setLauncherDataStore(LauncherDataStore *dataStore);
  virtual void setApplicationPackageMonitorListener(ApplicationPackageMonitorListener *packageMonitorListener);
  virtual void setPage(uint page);
  virtual int focusToPage(const QString &dekstopFileEntry);
  virtual void addLauncherButton(const QString &desktopEntryPath);
  virtual void removeLauncherButton(const QString &desktopEntryPath);
  virtual bool updateLauncherButton(const QString &desktopEntryPath);
  virtual void updatePagesFromDataStore();
  virtual void addNewLauncherButtonToPages(const QString &desktopEntryPath, QList<QSharedPointer<LauncherPage> > &pages);
  virtual void appendButtonToPages(QSharedPointer<LauncherButton> button, QList<QSharedPointer<LauncherPage> > &pages);
  virtual Launcher::Placement buttonPlacement(const QString &desktopEntryFile);
  virtual void addDesktopEntriesWithKnownPlacements(QList<QSharedPointer<LauncherPage> > &pages);
  virtual void addDesktopEntriesWithUnknownPlacements(QList<QSharedPointer<LauncherPage> > &pages);
  virtual void removeEmptyPages(QList<QSharedPointer<LauncherPage> > &pages);
  virtual QSharedPointer<LauncherButton> createLauncherButton(const QString &desktopEntryPath);
  virtual QMap<Launcher::Placement, QString> createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements);
  virtual void updateButtonState(const QString &desktopEntryPath, LauncherButtonModel::State state, int progress);
  virtual void addPlaceholderButton(const QString &desktopEntryPath);
  virtual void updateButtonPlacementInStore(const QString &desktopEntryPath);
  virtual void removePlaceholderButton(const QString &desktopEntryPath);
};

// 2. IMPLEMENT STUB
void LauncherStub::LauncherConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);
}
void LauncherStub::LauncherDestructor() {

}

void LauncherStub::setLauncherDataStore(LauncherDataStore *dataStore)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<LauncherDataStore *>(dataStore));
    stubMethodEntered("setLauncherDataStore", params);
}

void LauncherStub::setApplicationPackageMonitorListener(ApplicationPackageMonitorListener *packageMonitorListener)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<ApplicationPackageMonitorListener *>(packageMonitorListener));
    stubMethodEntered("setApplicationPackageMonitorListener", params);
}

void LauncherStub::setPage(uint page) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint>(page));
  stubMethodEntered("setPage",params);
}

int LauncherStub::focusToPage(const QString &desktopFileEntry)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopFileEntry));
  stubMethodEntered("focusToPage",params);
  return stubReturnValue<int>("focusToPage");
}

void LauncherStub::addLauncherButton(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("addLauncherButton",params);
}

void LauncherStub::removeLauncherButton(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("removeLauncherButton",params);
}

bool LauncherStub::updateLauncherButton(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("updateLauncherButton",params);
  return stubReturnValue<bool>("updateLauncherButton");
}

void LauncherStub::updatePagesFromDataStore() {
  stubMethodEntered("updatePagesFromDataStore");
}

void LauncherStub::addNewLauncherButtonToPages(const QString &desktopEntryPath, QList<QSharedPointer<LauncherPage> > &pages) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  params.append( new Parameter<QList<QSharedPointer<LauncherPage> > & >(pages));
  stubMethodEntered("addNewLauncherButtonToPages",params);
}

void LauncherStub::appendButtonToPages(QSharedPointer<LauncherButton> button, QList<QSharedPointer<LauncherPage> > &pages) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QSharedPointer<LauncherButton> >(button));
  params.append( new Parameter<QList<QSharedPointer<LauncherPage> > >(pages));
  stubMethodEntered("appendButtonToPages",params);
}

Launcher::Placement LauncherStub::buttonPlacement(const QString &desktopEntryFile) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryFile));
  stubMethodEntered("buttonPlacement",params);
  return stubReturnValue<Launcher::Placement>("buttonPlacement");
}

void LauncherStub::addDesktopEntriesWithKnownPlacements(QList<QSharedPointer<LauncherPage> > &pages) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QList<QSharedPointer<LauncherPage> > & >(pages));
  stubMethodEntered("addDesktopEntriesWithKnownPlacements",params);
}

void LauncherStub::addDesktopEntriesWithUnknownPlacements(QList<QSharedPointer<LauncherPage> > &pages) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QList<QSharedPointer<LauncherPage> > & >(pages));
  stubMethodEntered("addDesktopEntriesWithUnknownPlacements",params);
}

void LauncherStub::removeEmptyPages(QList<QSharedPointer<LauncherPage> > &pages) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QList<QSharedPointer<LauncherPage> > & >(pages));
  stubMethodEntered("removeEmptyPages",params);
}

QSharedPointer<LauncherButton> LauncherStub::createLauncherButton(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("createLauncherButton",params);
  return stubReturnValue<QSharedPointer<LauncherButton> >("createLauncherButton");
}

QMap<Launcher::Placement, QString> LauncherStub::createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QHash<QString, QVariant> & >(desktopEntryPlacements));
  stubMethodEntered("createPlacementMap",params);
  return stubReturnValue<QMap<Launcher::Placement, QString> >("createPlacementMap");
}

void LauncherStub::updateButtonState(const QString &desktopEntryPath, LauncherButtonModel::State state, int progress) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  params.append( new Parameter<LauncherButtonModel::State >(state));
  params.append( new Parameter<int >(progress));
  stubMethodEntered("updateButtonState",params);
}

void LauncherStub::addPlaceholderButton(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("addPlaceholderButton",params);
}

void LauncherStub::updateButtonPlacementInStore(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("updateButtonPlacementInStore",params);
}

void LauncherStub::removePlaceholderButton(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("removePlaceholderButton",params);
}


// 3. CREATE A STUB INSTANCE
LauncherStub gDefaultLauncherStub;
LauncherStub* gLauncherStub = &gDefaultLauncherStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Launcher::Launcher(QGraphicsItem *parent) {
  gLauncherStub->LauncherConstructor(parent);
}

Launcher::~Launcher() {
  gLauncherStub->LauncherDestructor();
}

void Launcher::setLauncherDataStore(LauncherDataStore *dataStore)
{
  gLauncherStub->setLauncherDataStore(dataStore);
}

void Launcher::setApplicationPackageMonitorListener(ApplicationPackageMonitorListener *packageMonitorListener)
{
  gLauncherStub->setApplicationPackageMonitorListener(packageMonitorListener);
}

void Launcher::setPage(uint page) {
  gLauncherStub->setPage(page);
}

int Launcher::focusToPage(const QString &desktopEntryFile) {
    return gLauncherStub->focusToPage(desktopEntryFile);
}

void Launcher::addLauncherButton(const QString &desktopEntryPath) {
  gLauncherStub->addLauncherButton(desktopEntryPath);
}

void Launcher::removeLauncherButton(const QString &desktopEntryPath) {
  gLauncherStub->removeLauncherButton(desktopEntryPath);
}

bool Launcher::updateLauncherButton(const QString &desktopEntryPath) {
  return gLauncherStub->updateLauncherButton(desktopEntryPath);
}

void Launcher::updatePagesFromDataStore() {
  gLauncherStub->updatePagesFromDataStore();
}

void Launcher::addNewLauncherButtonToPages(const QString &desktopEntryPath, QList<QSharedPointer<LauncherPage> > &pages) {
  gLauncherStub->addNewLauncherButtonToPages(desktopEntryPath, pages);
}

void Launcher::appendButtonToPages(QSharedPointer<LauncherButton> button, QList<QSharedPointer<LauncherPage> > &pages) {
  gLauncherStub->appendButtonToPages(button, pages);
}

Launcher::Placement Launcher::buttonPlacement(const QString &desktopEntryFile) {
  return gLauncherStub->buttonPlacement(desktopEntryFile);
}

void Launcher::addDesktopEntriesWithKnownPlacements(QList<QSharedPointer<LauncherPage> > &pages) {
  gLauncherStub->addDesktopEntriesWithKnownPlacements(pages);
}

void Launcher::addDesktopEntriesWithUnknownPlacements(QList<QSharedPointer<LauncherPage> > &pages) {
  gLauncherStub->addDesktopEntriesWithUnknownPlacements(pages);
}

void Launcher::removeEmptyPages(QList<QSharedPointer<LauncherPage> > &pages) {
  gLauncherStub->removeEmptyPages(pages);
}

QSharedPointer<LauncherButton> Launcher::createLauncherButton(const QString &desktopEntryPath) {
  return gLauncherStub->createLauncherButton(desktopEntryPath);
}

QMap<Launcher::Placement, QString> Launcher::createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements) {
  return gLauncherStub->createPlacementMap(desktopEntryPlacements);
}

void Launcher::updateButtonState(const QString &desktopEntryPath, LauncherButtonModel::State state, int progress) {
  gLauncherStub->updateButtonState(desktopEntryPath, state, progress);
}

void Launcher::addPlaceholderButton(const QString &desktopEntryPath) {
  gLauncherStub->addPlaceholderButton(desktopEntryPath);
}

void Launcher::updateButtonPlacementInStore(const QString &desktopEntryPath) {
  gLauncherStub->updateButtonPlacementInStore(desktopEntryPath);
}

void Launcher::removePlaceholderButton(const QString &desktopEntryPath) {
  gLauncherStub->removePlaceholderButton(desktopEntryPath);
}

Launcher::Placement::Placement() {
}

Launcher::Placement::Placement(const QString &placement) {
    Q_UNUSED(placement);
}


#endif
