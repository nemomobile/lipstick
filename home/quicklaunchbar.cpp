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

#include "quicklaunchbar.h"
#include "launcherbutton.h"
#include "launcher.h"
#include <DuiDesktopEntry>
#include <duifiledatastore.h>
#include <QDir>

QuickLaunchBar::QuickLaunchBar(QGraphicsItem *parent) : DuiWidgetController(new QuickLaunchBarModel, parent),
    fileDataStore(NULL)
{
    initializeDataStore();
    updateWidgetList();
}

QuickLaunchBar::~QuickLaunchBar()
{
    delete fileDataStore;
}

void QuickLaunchBar::initializeDataStore()
{
    if (!QDir::root().exists(QDir::homePath() + "/.config/duihome")) {
        QDir::root().mkpath(QDir::homePath() + "/.config/duihome");
    }

    fileDataStore = new DuiFileDataStore(QDir::homePath() + "/.config/duihome/quicklaunchbar.data");
}

void QuickLaunchBar::updateWidgetList()
{
    QList<DuiWidget *> widgets;
    for (int i = 1; i <= 4; i++) {
        DuiWidget *widget = NULL;

        QString key;
        key.sprintf("%d/desktopFile", i);
        if (fileDataStore->contains(key)) {
            DuiDesktopEntry desktopEntry(fileDataStore->value(key).toString());
            if (desktopEntry.isValid()) {
                widget = new LauncherButton(desktopEntry);
                widget->setObjectName("QuickLaunchBarButton");

                connect(widget, SIGNAL(applicationLaunched(const QString &)), this, SLOT(launchApplication(const QString &)), Qt::QueuedConnection);
                connect(widget, SIGNAL(duiApplicationLaunched(const QString &)), this, SLOT(launchDuiApplication(const QString &)), Qt::QueuedConnection);
            }
        }

        if (widget == NULL) {
            widget = new DuiWidget;
        }

        widgets.append(widget);
    }

    model()->setWidgets(widgets);
}

void QuickLaunchBar::launchApplication(const QString &application)
{
    Launcher::startApplication(application);
}

void QuickLaunchBar::launchDuiApplication(const QString &serviceName)
{
    Launcher::startDuiApplication(serviceName);
}
