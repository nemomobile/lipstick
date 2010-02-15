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

#include <QFlags>
#include <QDir>
#include <DuiActionProvider>
#include <DuiApplicationIfProxy>
#include <duiwidgetcontroller.h>

#ifdef ENABLE_QTTRACKER
#include <QtTracker/ontologies/nfo.h>
#endif

#include "launcher.h"
#include "launcherbutton.h"

Launcher::Launcher(DuiWidget *parent) :
    DuiWidgetController(new LauncherModel, parent),
    initialized(false)
{
}

Launcher::~Launcher()
{
    // Destroy the remaining applet buttons
    foreach(DuiWidget * w, model()->widgets()) {
        delete w;
    }

    // Destroy the application desktop entries
    foreach(DuiDesktopEntry * e, applicationDesktopEntries) {
        delete e;
    }

    // Destroy the category desktop entries
    foreach(DuiDesktopEntry * e, categoryDesktopEntries) {
        delete e;
    }
}

void Launcher::initializeIfNecessary()
{
    if (!initialized) {
        // Read the contents of the applications directory
        directoryChanged(APPLICATIONS_DIRECTORY, false);
        directoryChanged(CATEGORIES_DIRECTORY, false);
#ifdef TESTABILITY_ON
        directoryChanged(QDir::tempPath(), false);
#endif
#ifdef ENABLE_QTTRACKER
        // Query tracker for bookmarks (shortcuts)
        SopranoLive::RDFSelect select;
        select.addColumn("iri", SopranoLive::RDFVariable::fromType<SopranoLive::nfo::Bookmark>());
        shortcutItemModel = tracker()->modelQuery(select);
#endif

        // Update the widget list
        updateWidgetList();

        // Start watching the applications directory for changes
        connect(&watcher, SIGNAL(directoryChanged(const QString)), this, SLOT(directoryChanged(const QString)));
        watcher.addPath(APPLICATIONS_DIRECTORY);
        watcher.addPath(CATEGORIES_DIRECTORY);
#ifdef TESTABILITY_ON
        watcher.addPath(QDir::tempPath());
#endif
        // The launcher has now been initialized
        initialized = true;
    }
}

void Launcher::setCategory(const QString &category, const QString &title, const QString &iconId)
{
    Q_UNUSED(title);
    Q_UNUSED(iconId);

    initializeIfNecessary();

    model()->setCategory(category.isEmpty() ? LauncherModel::RootCategory : LauncherModel::SubCategory);

    if (currentCategory != category) {
        // Set the new category and update the widget list
        currentCategory = category;
        updateWidgetList();
    }
}

void Launcher::updateWidgetList()
{
    // Remove all widgets from the model
    QList<DuiWidget *> widgets;
    QList<DuiWidget *> oldWidgets(model()->widgets());

    // Create application launcher buttons
    foreach(DuiDesktopEntry * e, applicationDesktopEntries) {
        if (((currentCategory.isEmpty() && (e->categories().count() == 0
                                            || (e->categories().count() == 1 && e->categories().at(0) == "DUI")) // TODO: remove the test on this line when the Category DUI feature is removed
             )) ||
                (!currentCategory.isEmpty() && e->categories().contains(currentCategory))) {
            widgets.append(createLauncherButton(*e));
        }
    }

    // Categories and shortcuts are currently only supported in the root
    if (currentCategory.isEmpty()) {
        // Create category launcher buttons
        foreach(DuiDesktopEntry * e, categoryDesktopEntries) {
            widgets.append(createLauncherButton(*e));
        }

#ifdef ENABLE_QTTRACKER
        // Create shortcut launcher buttons
        foreach(SopranoLive::LiveNode shortcut, shortcutItemModel.nodes()) {
            widgets.append(createShortcutLauncherButton(shortcut));
        }
#endif
    }

    // Let the view know that the items have changed
    model()->setWidgets(widgets);

    // Destroy the old widgets. This MUST be done AFTER the widgets have been removed from the layout.
    foreach(DuiWidget * widget, oldWidgets) {
        delete widget;
    }
}

void Launcher::directoryChanged(const QString &path, bool updateWidgetList)
{
    if (path == APPLICATIONS_DIRECTORY) {
        updateDesktopEntryList(applicationDesktopEntries, path, "*.desktop", QStringList() << "Application" << "Link");
    } else if (path == CATEGORIES_DIRECTORY) {
        updateDesktopEntryList(categoryDesktopEntries, path, "*.directory", QStringList() << "Directory");
    }

#ifdef TESTABILITY_ON
    else if (path == QDir::tempPath()) {
        updateDesktopEntryList(applicationDesktopEntries, path, "*.desktop", QStringList() << "Application" << "Link");
    }
#endif


    if (updateWidgetList) {
        this->updateWidgetList();
    }
}

void Launcher::updateDesktopEntryList(DesktopEntryContainer &desktopEntryContainer, const QString &path, const QString &nameFilter, const QStringList &acceptedTypes) const
{
    // Destroy the old desktop entries
    foreach(DuiDesktopEntry * e, desktopEntryContainer) {
        delete e;
    }
    desktopEntryContainer.clear();

    // Read in the new desktop entries
    foreach(QFileInfo fileInfo, QDir(path, nameFilter).entryInfoList(QDir::Files)) {
        DuiDesktopEntry *e = new DuiDesktopEntry(fileInfo.absoluteFilePath());
        if (e->isValid() &&
                acceptedTypes.contains(e->type()) &&
                (e->onlyShowIn().count() == 0 || e->onlyShowIn().contains("X-DUI") || e->onlyShowIn().contains("DUI")) &&
                (e->notShowIn().count() == 0 || !e->notShowIn().contains("X-DUI"))) {
            desktopEntryContainer.append(e);
        } else {
            delete e;
        }
    }
}

DuiWidget *Launcher::createLauncherButton(const DuiDesktopEntry &entry)
{
    LauncherButton *ret = new LauncherButton(NULL);
    ret->setObjectName("LauncherButton");
    ret->setTargetType(entry.type());
    ret->setText(entry.name());
    if (!entry.icon().isEmpty()) {
        ret->setIconID(entry.icon());
    } else {
        // FIXME: change to use correct default icon id when available
        // as incorrect id icon-Application-Default will load default icon
        // (at the moment default icon seems to be icon-l-video)
        ret->setIconID("icon-Application-Default");
    }

    // Set target based on type
    if (entry.type() == "Application") {
        QString thisXMaemoService = entry.xMaemoService();

        if (thisXMaemoService.isEmpty()) {
            ret->setTarget(entry.exec());
            connect(ret, SIGNAL(applicationLaunched(const QString &)), this, SLOT(applicationLaunched(const QString &)), Qt::QueuedConnection);
        } else {
            ret->setTarget(thisXMaemoService);
            connect(ret, SIGNAL(applicationLaunched(const QString &)), this, SLOT(duiApplicationLaunched(const QString &)), Qt::QueuedConnection);
        }
    } else if (entry.type() == "Link") {
        ret->setTarget(entry.url());
        connect(ret, SIGNAL(linkLaunched(const QString &)), this, SLOT(linkLaunched(const QString &)), Qt::QueuedConnection);
    } else if (entry.type() == "Directory") {
        ret->setTarget(entry.nameUnlocalized());
        connect(ret, SIGNAL(directoryLaunched(const QString &, const QString &, const QString &)), this, SLOT(directoryLaunched(const QString &, const QString &, const QString &)), Qt::QueuedConnection);
    }

    return ret;
}

#ifdef ENABLE_QTTRACKER
DuiWidget *Launcher::createShortcutLauncherButton(SopranoLive::LiveNode shortcut)
{
    SopranoLive::Live<SopranoLive::nfo::Bookmark> bookmark = shortcut;

    LauncherButton *ret = new LauncherButton(NULL);
    ret->setViewType("launcherbutton");
    ret->setObjectName("LauncherButton");
    ret->setTargetType("Link");
    ret->setText(bookmark->getTitle());
    ret->setTarget(bookmark->getBookmarks().toString());

    // TODO: We could set the icon of the launcher button here if the shortcuts supported
    // them.

    QList<SopranoLive::LiveNode> l = bookmark->getMaemoBookmarkThumbnails().nodes();
    if (l.count() > 0) {
        // Use the first thumbnail
        ret->setThumbnail(l[0].toString());
    }

    // Add the default action to the launcher button
    DuiAction *action = DuiActionProvider::getDefaultAction(QUrl(ret->target()));
    if (action != NULL) {
        action->setVisible(false);
        ret->addAction(action);
        connect(ret, SIGNAL(clicked()), action, SLOT(trigger()));
    }

    // Connect the link launched signal for closing the launcher
    connect(ret, SIGNAL(linkLaunched(const QString &)), this, SLOT(linkLaunched(const QString &)), Qt::QueuedConnection);

    return ret;
}
#endif

void Launcher::applicationLaunched(const QString &application)
{
    if (!QProcess::startDetached(application)) {
        qWarning() << "Failed to start application:" << application;
    } else {
        qDebug() << "Started application succesfully:" << application;
    }

    openRootCategory();
}

void Launcher::duiApplicationLaunched(const QString &serviceName)
{
    qDebug() << "Attempting to launch " << serviceName;

    DuiApplicationIfProxy duiApplicationIfProxy(serviceName, this);

    if (duiApplicationIfProxy.connection().isConnected()) {
        qDebug() << "Launching " << serviceName;
        duiApplicationIfProxy.launch();
    } else {
        qWarning() << "Could not launch" << serviceName;
        qWarning() << "DBus not connected?";
    }

    openRootCategory();
}

void Launcher::linkLaunched(const QString &link)
{
    Q_UNUSED(link);

    openRootCategory();
}

void Launcher::directoryLaunched(const QString &directory, const QString &title, const QString &iconId)
{
    setCategory(directory, title, iconId);
}

void Launcher::openRootCategory()
{
    setCategory(QString());
}

void Launcher::setEnabled(bool enabled)
{
    QGraphicsItem::setEnabled(enabled);
}
