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
#ifndef SWITCHER_STUB_H
#define SWITCHER_STUB_H

#include <stubbase.h>
#include <switcher.h>
#include <windowinfo.h>

class SwitcherStub : public StubBase
{
public:
    virtual void switcherConstructor(MWidget *parent = NULL);
    virtual void switcherDestructor();
    virtual void updateWindowList(const QList<WindowInfo> &windowList);
    virtual void windowToFront(Window window);
    virtual void closeWindow(Window window);
    virtual void changeWindowTitle(Window window,  const QString &title);
    virtual void updateButtons();
    virtual void scheduleUpdate();
};

void SwitcherStub::switcherConstructor(MWidget *parent)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidget *>(parent));
    stubMethodEntered("switcherConstructor", params);
}

void SwitcherStub::switcherDestructor()
{
    stubMethodEntered("switcherDestructor");
}

void SwitcherStub::updateWindowList(const QList<WindowInfo> &windowList)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QList<WindowInfo> >(windowList));
    stubMethodEntered("updateWindowList", params);
}

void SwitcherStub::windowToFront(Window window)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Window>(window));
    stubMethodEntered("windowToFront", params);
}

void SwitcherStub::closeWindow(Window window)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Window>(window));
    stubMethodEntered("closeWindow", params);
}

void SwitcherStub::changeWindowTitle(Window window,  const QString &title)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Window>(window));
    params.append(new Parameter<QString>(title));
    stubMethodEntered("changeWindowTitle", params);
}

void SwitcherStub::updateButtons()
{
    stubMethodEntered("updateButtons");
}

void SwitcherStub::scheduleUpdate()
{
    stubMethodEntered("scheduleUpdate");
}

SwitcherStub gDefaultSwitcherStub;
SwitcherStub *gSwitcherStub = &gDefaultSwitcherStub;
Switcher *gSwitcherInstance = NULL;

Switcher::Switcher(MWidget *parent) : MWidgetController(parent)
{
    gSwitcherStub->switcherConstructor(parent);
}

Switcher::~Switcher()
{
    gSwitcherStub->switcherDestructor();
    gSwitcherInstance = NULL;
}

Switcher *Switcher::instance()
{
    if (gSwitcherInstance == NULL) {
        gSwitcherInstance = new Switcher;
    }
    return gSwitcherInstance;
}

void Switcher::updateWindowList(const QList<WindowInfo> &windowList)
{
    gSwitcherStub->updateWindowList(windowList);
}

void Switcher::windowToFront(Window window)
{
    gSwitcherStub->windowToFront(window);
}

void Switcher::closeWindow(Window window)
{
    gSwitcherStub->closeWindow(window);
}

void Switcher::changeWindowTitle(Window window,  const QString &title)
{
    gSwitcherStub->changeWindowTitle(window, title);
}

void Switcher::updateButtons()
{
    gSwitcherStub->updateButtons();
}

void Switcher::scheduleUpdate()
{
    gSwitcherStub->scheduleUpdate();
}

#endif
