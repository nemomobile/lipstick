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

#include "launcherpage.h"
#include "launcherpagemodel.h"
#include "launcherpageview.h"
#include "launcherbutton.h"
#include <MDesktopEntry>

LauncherPage::LauncherPage(MWidget *parent) : MWidgetController(new LauncherPageModel, parent)
{
}

bool LauncherPage::insertButton(QSharedPointer<LauncherButton> button, int positionIndex)
{
    QList< QSharedPointer<LauncherButton> > buttons(model()->launcherButtons());
    bool canBeAdded = false;
    // button can be added if page is not full and the index is sane
    if (buttons.count() < model()->maxButtons() && positionIndex < model()->maxButtons()) {
        canBeAdded = true;
	buttons.insert(positionIndex, button);
        model()->setLauncherButtons(buttons);
    }
    return canBeAdded;
}

bool LauncherPage::appendButton(QSharedPointer<LauncherButton> button)
{
    return insertButton(button, model()->launcherButtons().size());
}

void LauncherPage::removeButton(QSharedPointer<LauncherButton> button)
{
   QList< QSharedPointer<LauncherButton> > buttons(model()->launcherButtons());
   buttons.removeOne(button);
   model()->setLauncherButtons(buttons);
}

bool LauncherPage::contains(const QString &desktopEntryFile) const
{
    bool containsButton = false;
    foreach (QSharedPointer<LauncherButton> button, model()->launcherButtons()) {
       containsButton = desktopEntryFile == button->model()->desktopEntryFile();
       if (containsButton) {
           break;
       }
    }
    return containsButton;
}

static bool fileNameMatchesPaths(const QString &fileName,
                                 const QStringList &paths) {
    foreach (const QString& dir, paths) {
        if (fileName.startsWith(dir)) {
            return true;
        }
    }
    return false;
}

bool LauncherPage::prune(QStringList entryList, QStringList directories)
{
    QList< QSharedPointer<LauncherButton> > newButtons(model()->launcherButtons());
    foreach (QSharedPointer<LauncherButton> button, newButtons) {
        QString fileName = button->model()->desktopEntryFile();
        // check against directory so that we dont prune
        // entries from other directories
        bool fileNameMatches = fileNameMatchesPaths(fileName, directories);
        if (!fileNameMatches
            || (!entryList.contains(fileName)
                && fileNameMatches)) {
            newButtons.removeOne(button);
        }
    }
    model()->setLauncherButtons(newButtons);

    return !newButtons.isEmpty();
}
