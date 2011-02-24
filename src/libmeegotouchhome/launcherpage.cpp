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
#include <QFileInfo>

#include <MWidgetCreator>
M_REGISTER_WIDGET(LauncherPage)

LauncherPage::LauncherPage(MWidget *parent) : MWidgetController(new LauncherPageModel, parent)
{
}

void LauncherPage::setMaximumButtonCount(int maximumButtonCount)
{
    model()->setMaxButtons(maximumButtonCount);
}

int LauncherPage::insertButton(QSharedPointer<LauncherButton> button, int positionIndex)
{
    QList< QSharedPointer<LauncherButton> > buttons(model()->launcherButtons());
    int insertIndex = -1;
    int buttonsCount = buttons.count();
    // button can be added if page is not full and the index is sane
    if (buttonsCount < model()->maxButtons() && positionIndex < model()->maxButtons()) {
        buttons.insert(positionIndex, button);

        if (positionIndex < buttonsCount) {
            insertIndex = positionIndex;
        } else {
            insertIndex = buttonsCount;
        }
        model()->setLauncherButtons(buttons);
    }
    return insertIndex;
}

bool LauncherPage::appendButton(QSharedPointer<LauncherButton> button)
{
    bool canBeAdded = false;
    if (insertButton(button, model()->launcherButtons().size()) >= 0) {
        canBeAdded = true;
    }

    return canBeAdded;
}

void LauncherPage::removeButton(QSharedPointer<LauncherButton> button)
{
    QList< QSharedPointer<LauncherButton> > buttons(model()->launcherButtons());
    buttons.removeOne(button);
    model()->setLauncherButtons(buttons);
}

bool LauncherPage::removeButton(const QString &desktopEntryPath)
{
    bool contains = false;
    int buttonPosition = launcherButtonPosition(desktopEntryPath);
    if (buttonPosition >= 0) {
        contains = true;
        removeButton(model()->launcherButtons().at(buttonPosition));
    }

    return contains;
}

bool LauncherPage::updateButton(const QString &desktopEntryPath)
{
    bool contains = false;
    int buttonPosition = launcherButtonPosition(desktopEntryPath);
    if (buttonPosition >= 0) {
        contains = true;
        model()->launcherButtons().at(buttonPosition)->updateFromDesktopEntry(desktopEntryPath);
    }
    return contains;
}

int LauncherPage::launcherButtonPosition(const QString &desktopEntryPath)
{
    int position = -1;
    int buttonIndex = 0;

    QString desktopFileName = QFileInfo(desktopEntryPath).fileName();
    foreach(const QSharedPointer<LauncherButton> &button, model()->launcherButtons()) {
        if (QFileInfo(button->desktopEntry()).fileName() == desktopFileName) {
            position = buttonIndex;
            break;
        }
        buttonIndex++;
    }

    return position;
}

QSharedPointer<LauncherButton> LauncherPage::button(const QString &desktopEntryPath)
{
    int buttonPosition = launcherButtonPosition(desktopEntryPath);
    QSharedPointer<LauncherButton> button;
    if (buttonPosition > -1) {
        button = model()->launcherButtons().at(buttonPosition);
    }
    return button;
}
