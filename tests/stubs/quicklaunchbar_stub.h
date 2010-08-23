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
  virtual void QuickLaunchBarDestructor();
  virtual void setLauncherDataStore(LauncherDataStore *dataStore);
  virtual void setApplicationPackageMonitor(ApplicationPackageMonitor *packageMonitor);
  virtual void setDownloadProgress(const QString& packageName, const QString& desktopEntryPath, int bytesLoaded, int bytesTotal);
  virtual void setInstallProgress(const QString& packageName, const QString& desktopEntryPath, int percentage);
  virtual void setOperationSuccess(const QString& packageName, const QString& desktopEntryPath);
  virtual void setOperationError(const QString& packageName, const QString& desktopEntryPath, const QString& error);
  virtual void updateButtons();
  virtual LauncherButton * createLauncherButton(const QString &desktopEntryPath);
  virtual QMap<QuickLaunchBar::Placement, QString> createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements);
}; 

// 2. IMPLEMENT STUB
void QuickLaunchBarStub::QuickLaunchBarConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);
}
void QuickLaunchBarStub::QuickLaunchBarDestructor() {

}

void QuickLaunchBarStub::setLauncherDataStore(LauncherDataStore *dataStore)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<LauncherDataStore *>(dataStore));
    stubMethodEntered("setLauncherDataStore", params);
}

void QuickLaunchBarStub::setApplicationPackageMonitor(ApplicationPackageMonitor *packageMonitor)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<ApplicationPackageMonitor *>(packageMonitor));
    stubMethodEntered("setApplicationPackageMonitor", params);
}

void QuickLaunchBarStub::setDownloadProgress(const QString& packageName, const QString& desktopEntryPath, int bytesLoaded, int bytesTotal)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(packageName));
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<int>(bytesLoaded));
  params.append( new Parameter<int>(bytesTotal));
  stubMethodEntered("setDownloadProgress", params);
}

void QuickLaunchBarStub::setInstallProgress(const QString& packageName, const QString& desktopEntryPath, int percentage)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(packageName));
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<int>(percentage));
  stubMethodEntered("setInstallProgress", params);
}

void QuickLaunchBarStub::setOperationSuccess(const QString& packageName, const QString& desktopEntryPath)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(packageName));
  params.append( new Parameter<QString>(desktopEntryPath));
  stubMethodEntered("setOperationSuccess", params);
}

void QuickLaunchBarStub::setOperationError(const QString& packageName, const QString& desktopEntryPath, const QString& error)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QString>(packageName));
  params.append( new Parameter<QString>(desktopEntryPath));
  params.append( new Parameter<QString>(error));
  stubMethodEntered("setOperationError", params);
}

void QuickLaunchBarStub::updateButtons() {
  stubMethodEntered("updateButtons");
}

LauncherButton * QuickLaunchBarStub::createLauncherButton(const QString &desktopEntryPath) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(desktopEntryPath));
  stubMethodEntered("createLauncherButton",params);
  return stubReturnValue<LauncherButton *>("createLauncherButton");
}

QMap<QuickLaunchBar::Placement, QString> QuickLaunchBarStub::createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QHash<QString, QVariant> & >(desktopEntryPlacements));
  stubMethodEntered("createPlacementMap",params);
  return stubReturnValue<QMap<QuickLaunchBar::Placement, QString> >("createPlacementMap");
}



// 3. CREATE A STUB INSTANCE
QuickLaunchBarStub gDefaultQuickLaunchBarStub;
QuickLaunchBarStub* gQuickLaunchBarStub = &gDefaultQuickLaunchBarStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
QuickLaunchBar::QuickLaunchBar(QGraphicsItem *parent) {
  gQuickLaunchBarStub->QuickLaunchBarConstructor(parent);
}

QuickLaunchBar::~QuickLaunchBar() {
  gQuickLaunchBarStub->QuickLaunchBarDestructor();
}

void QuickLaunchBar::setLauncherDataStore(LauncherDataStore *dataStore)
{
  gQuickLaunchBarStub->setLauncherDataStore(dataStore);
}

void QuickLaunchBar::setApplicationPackageMonitor(ApplicationPackageMonitor *packageMonitor)
{
  gQuickLaunchBarStub->setApplicationPackageMonitor(packageMonitor);
}

void QuickLaunchBar::setDownloadProgress(const QString& packageName, const QString& desktopEntryPath, int bytesLoaded, int bytesTotal)
{
  gQuickLaunchBarStub->setDownloadProgress(packageName, desktopEntryPath, bytesLoaded, bytesTotal);
}

void QuickLaunchBar::setInstallProgress(const QString& packageName, const QString& desktopEntryPath, int percentage)
{
  gQuickLaunchBarStub->setInstallProgress(packageName, desktopEntryPath, percentage);
}

void QuickLaunchBar::setOperationSuccess(const QString& packageName, const QString& desktopEntryPath)
{
  gQuickLaunchBarStub->setOperationSuccess(packageName, desktopEntryPath);
}

void QuickLaunchBar::setOperationError(const QString& packageName, const QString& desktopEntryPath, const QString& error)
{
  gQuickLaunchBarStub->setOperationError(packageName, desktopEntryPath, error);
}

void QuickLaunchBar::updateButtons() {
  gQuickLaunchBarStub->updateButtons();
}

LauncherButton * QuickLaunchBar::createLauncherButton(const QString &desktopEntryPath) {
  return gQuickLaunchBarStub->createLauncherButton(desktopEntryPath);
}

QMap<QuickLaunchBar::Placement, QString> QuickLaunchBar::createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements) {
  return gQuickLaunchBarStub->createPlacementMap(desktopEntryPlacements);
}


#endif
