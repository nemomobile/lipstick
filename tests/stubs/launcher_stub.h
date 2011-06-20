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
  virtual void LauncherConstructor(QGraphicsItem *parent, LauncherModel *model);
  virtual void LauncherDestructor();
  virtual void setLauncherDataStore(LauncherDataStore *dataStore);
  virtual void setApplicationPackageMonitor(ApplicationPackageMonitor *packageMonitor);
  virtual void setMaximumPageSize(int maximumPageSize);
  virtual int focusToButton(const QString &desktopFileEntry);
  virtual void setPage(uint page);
  virtual void updateButtonState(const QSharedPointer<MDesktopEntry> &desktopEntry, const QString &packageName, const QString &state, bool packageRemovable);
  virtual void removePlaceholderButton(const QString &desktopEntryPath);
  virtual void addLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry);
  virtual void removeLauncherButton(const QString &desktopEntryPath);
  virtual bool updateLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry);
  virtual void updatePagesFromDataStore();
  virtual Launcher::Placement entryPlacementInDatastore(const QString &desktopEntryPath);
  virtual void addNewLauncherButtonToPages(const QString &desktopEntryPath, QList<QSharedPointer<LauncherPage> > &pages);
  virtual Launcher::Placement appendButtonToPages(QSharedPointer<LauncherButton> button, QList<QSharedPointer<LauncherPage> > &pages);
  virtual Launcher::Placement buttonPlacement(const QString &desktopEntryFile);
  virtual void addDesktopEntriesWithKnownPlacements(QList<QSharedPointer<LauncherPage> > &pages);
  virtual void addDesktopEntriesWithUnknownPlacements(QList<QSharedPointer<LauncherPage> > &pages);
  virtual void removeEmptyPages(QList<QSharedPointer<LauncherPage> > &pages);
  virtual QSharedPointer<LauncherButton> createLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry);
  virtual QMap<Launcher::Placement, QString> createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements);
  virtual QSharedPointer<LauncherButton> placeholderButton(const QSharedPointer<MDesktopEntry> &desktopEntry);
  virtual void updateButtonPlacementInStore(const QString &desktopEntryPath);
  virtual void removeButtonPlacementFromStore(const QString &desktopEntryPath);
  virtual void setMaximumPageSizeIfNecessary(QSharedPointer<LauncherPage> &page);
  virtual QSharedPointer<LauncherPage> createLauncherPage();
  virtual void updateButtonPlacementsOnPage(LauncherPage *page);
  virtual int pageIndex(LauncherPage *page);
  virtual void updateProgress(const QString& desktopEntryPath, int already, int total);
  virtual LauncherButtonModel::State buttonStateFromPackageState(const QString &packageState);
};

// 2. IMPLEMENT STUB
void LauncherStub::LauncherConstructor(QGraphicsItem *parent, LauncherModel *model) {
  Q_UNUSED(parent);
  Q_UNUSED(model);
}
void LauncherStub::LauncherDestructor() {

}
void LauncherStub::setLauncherDataStore(LauncherDataStore *dataStore) {
  QList<ParameterBase*> params;
  params.append( new Parameter<LauncherDataStore * >(dataStore));
  stubMethodEntered("setLauncherDataStore",params);
}

void LauncherStub::setApplicationPackageMonitor(ApplicationPackageMonitor *packageMonitor) {
  QList<ParameterBase*> params;
  params.append( new Parameter<ApplicationPackageMonitor * >(packageMonitor));
  stubMethodEntered("setApplicationPackageMonitor",params);
}

void LauncherStub::setMaximumPageSize(int maximumPageSize) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(maximumPageSize));
  stubMethodEntered("setMaximumPageSize",params);
}

int LauncherStub::focusToButton(const QString &desktopFileEntry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopFileEntry));
  stubMethodEntered("focusToButton",params);
  return stubReturnValue<int>("focusToButton");
}

void LauncherStub::setPage(uint page) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(page));
  stubMethodEntered("setPage",params);
}

void LauncherStub::updateButtonState(const QSharedPointer<MDesktopEntry> &desktopEntry, const QString &packageName, const QString &state, bool packageRemovable) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QSharedPointer<MDesktopEntry> >(desktopEntry));
  params.append( new Parameter<QString>(packageName));
  params.append( new Parameter<QString>(state));
  params.append( new Parameter<bool>(packageRemovable));
  stubMethodEntered("updateButtonState",params);
}

void LauncherStub::removePlaceholderButton(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("removePlaceholderButton",params);
}

void LauncherStub::addLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QSharedPointer<MDesktopEntry> >(desktopEntry));
  stubMethodEntered("addLauncherButton",params);
}

void LauncherStub::removeLauncherButton(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("removeLauncherButton",params);
}

bool LauncherStub::updateLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QSharedPointer<MDesktopEntry> >(desktopEntry));
  stubMethodEntered("updateLauncherButton",params);
  return stubReturnValue<bool>("updateLauncherButton");
}

void LauncherStub::updatePagesFromDataStore() {
  stubMethodEntered("updatePagesFromDataStore");
}

Launcher::Placement LauncherStub::entryPlacementInDatastore(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("entryPlacementInDatastore",params);
  return stubReturnValue<Launcher::Placement>("entryPlacementInDatastore");
}

void LauncherStub::addNewLauncherButtonToPages(const QString &desktopEntryPath, QList<QSharedPointer<LauncherPage> > &pages) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<QList<QSharedPointer<LauncherPage> > >(pages));
  stubMethodEntered("addNewLauncherButtonToPages",params);
}

Launcher::Placement LauncherStub::appendButtonToPages(QSharedPointer<LauncherButton> button, QList<QSharedPointer<LauncherPage> > &pages) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QSharedPointer<LauncherButton> >(button));
  params.append( new Parameter<QList<QSharedPointer<LauncherPage> > >(pages));
  stubMethodEntered("appendButtonToPages",params);
  return stubReturnValue<Launcher::Placement>("appendButtonToPages");
}

Launcher::Placement LauncherStub::buttonPlacement(const QString &desktopEntryFile) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryFile));
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
  params.append( new Parameter<QList<QSharedPointer<LauncherPage> > >(pages));
  stubMethodEntered("addDesktopEntriesWithUnknownPlacements",params);
}

void LauncherStub::removeEmptyPages(QList<QSharedPointer<LauncherPage> > &pages) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QList<QSharedPointer<LauncherPage> > >(pages));
  stubMethodEntered("removeEmptyPages",params);
}

QSharedPointer<LauncherButton> LauncherStub::createLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QSharedPointer<MDesktopEntry> >(desktopEntry));
  stubMethodEntered("createLauncherButton",params);
  return stubReturnValue<QSharedPointer<LauncherButton> >("createLauncherButton");
}

QMap<Launcher::Placement, QString> LauncherStub::createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QHash<QString, QVariant> >(desktopEntryPlacements));
  stubMethodEntered("createPlacementMap",params);
  return stubReturnValue<QMap<Launcher::Placement, QString> >("createPlacementMap");
}

QSharedPointer<LauncherButton> LauncherStub::placeholderButton(const QSharedPointer<MDesktopEntry> &desktopEntry) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QSharedPointer<MDesktopEntry> >(desktopEntry));
  stubMethodEntered("placeholderButton",params);
  return stubReturnValue<QSharedPointer<LauncherButton> >("placeholderButton");
}

void LauncherStub::updateButtonPlacementsOnPage(LauncherPage *page) {
  QList<ParameterBase*> params;
  params.append( new Parameter<LauncherPage*>(page));
  stubMethodEntered("updateButtonPlacementOnPage",params);
}
  
void LauncherStub::updateButtonPlacementInStore(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("updateButtonPlacementInStore",params);
}

void LauncherStub::removeButtonPlacementFromStore(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("removeButtonPlacementFromStore",params);
}

void LauncherStub::setMaximumPageSizeIfNecessary(QSharedPointer<LauncherPage> &page) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QSharedPointer<LauncherPage> >(page));
  stubMethodEntered("setMaximumPageSizeIfNecessary",params);
}

QSharedPointer<LauncherPage> LauncherStub::createLauncherPage()
{
  stubMethodEntered("createLauncherPage");
  return stubReturnValue<QSharedPointer<LauncherPage> >("createLauncherPage");
}

int LauncherStub::pageIndex(LauncherPage *page) {
  QList<ParameterBase*> params;
  params.append( new Parameter<LauncherPage *>(page));
  stubMethodEntered("pageIndex",params);
  return stubReturnValue<int>("pageIndex");
}

void LauncherStub::updateProgress(const QString& desktopEntryPath, int already, int total)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<int>(already));
  params.append( new Parameter<int>(total));
  stubMethodEntered("updateProgress",params);
}

LauncherButtonModel::State LauncherStub::buttonStateFromPackageState(const QString &packageState)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(packageState));
  stubMethodEntered("buttonStateFromPackageState",params);
  return stubReturnValue<LauncherButtonModel::State>("buttonStateFromPackageState");
}


// 3. CREATE A STUB INSTANCE
LauncherStub gDefaultLauncherStub;
LauncherStub* gLauncherStub = &gDefaultLauncherStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Launcher::Launcher(QGraphicsItem *parent, LauncherModel *model) {
  gLauncherStub->LauncherConstructor(parent, model);
}

Launcher::~Launcher() {
  gLauncherStub->LauncherDestructor();
}

void Launcher::setLauncherDataStore(LauncherDataStore *dataStore) {
  gLauncherStub->setLauncherDataStore(dataStore);
}

void Launcher::setApplicationPackageMonitor(ApplicationPackageMonitor *packageMonitor) {
  gLauncherStub->setApplicationPackageMonitor(packageMonitor);
}

void Launcher::setMaximumPageSize(int maximumPageSize) {
  gLauncherStub->setMaximumPageSize(maximumPageSize);
}

int Launcher::focusToButton(const QString &desktopFileEntry) {
  return gLauncherStub->focusToButton(desktopFileEntry);
}

void Launcher::setPage(uint page) {
  gLauncherStub->setPage(page);
}

void Launcher::updateButtonPlacementsOnPage(LauncherPage *page) {
  gLauncherStub->updateButtonPlacementsOnPage(page);
}

void Launcher::updateButtonState(const QSharedPointer<MDesktopEntry> &desktopEntry, const QString &packageName, const QString &state, bool packageRemovable) {
  gLauncherStub->updateButtonState(desktopEntry, packageName, state, packageRemovable);
}

void Launcher::removePlaceholderButton(const QString &desktopEntryPath) {
  gLauncherStub->removePlaceholderButton(desktopEntryPath);
}

void Launcher::addLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry) {
  gLauncherStub->addLauncherButton(desktopEntry);
}

void Launcher::removeLauncherButton(const QString &desktopEntryPath) {
  gLauncherStub->removeLauncherButton(desktopEntryPath);
}

bool Launcher::updateLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry) {
  return gLauncherStub->updateLauncherButton(desktopEntry);
}

void Launcher::updatePagesFromDataStore() {
  gLauncherStub->updatePagesFromDataStore();
}

Launcher::Placement Launcher::entryPlacementInDatastore(const QString &desktopEntryPath) {
  return gLauncherStub->entryPlacementInDatastore(desktopEntryPath);
}

void Launcher::addNewLauncherButtonToPages(const QString &desktopEntryPath, QList<QSharedPointer<LauncherPage> > &pages) {
  gLauncherStub->addNewLauncherButtonToPages(desktopEntryPath, pages);
}

Launcher::Placement Launcher::appendButtonToPages(QSharedPointer<LauncherButton> button, QList<QSharedPointer<LauncherPage> > &pages) {
  return gLauncherStub->appendButtonToPages(button, pages);
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

QSharedPointer<LauncherButton> Launcher::createLauncherButton(const QSharedPointer<MDesktopEntry> &desktopEntry) {
  return gLauncherStub->createLauncherButton(desktopEntry);
}

QMap<Launcher::Placement, QString> Launcher::createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements) {
  return gLauncherStub->createPlacementMap(desktopEntryPlacements);
}

QSharedPointer<LauncherButton> Launcher::placeholderButton(const QSharedPointer<MDesktopEntry> &desktopEntry) {
  return gLauncherStub->placeholderButton(desktopEntry);
}

void Launcher::updateButtonPlacementInStore(const QString &desktopEntryPath) {
  gLauncherStub->updateButtonPlacementInStore(desktopEntryPath);
}

void Launcher::removeButtonPlacementFromStore(const QString &desktopEntryPath) {
  gLauncherStub->removeButtonPlacementFromStore(desktopEntryPath);
}

void Launcher::setMaximumPageSizeIfNecessary(QSharedPointer<LauncherPage> &page) {
  gLauncherStub->setMaximumPageSizeIfNecessary(page);
}

QSharedPointer<LauncherPage> Launcher::createLauncherPage()
{
    return gLauncherStub->createLauncherPage();
}

Launcher::Placement::Placement() {
}

Launcher::Placement::Placement(const QString &placement) {
    Q_UNUSED(placement);
}

int Launcher::pageIndex(LauncherPage *page) {
    return gLauncherStub->pageIndex(page);
}

void Launcher::updateProgress(const QString& desktopEntryPath, int already, int total)
{
    gLauncherStub->updateProgress(desktopEntryPath, already, total);
}

LauncherButtonModel::State Launcher::buttonStateFromPackageState(const QString &packageState)
{
    return gLauncherStub->buttonStateFromPackageState(packageState);
}

#endif
