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
#include "xeventlistener_stub.h"

class SwitcherStub : public StubBase
{
public:
    virtual void switcherConstructor(MWidget *parent = NULL, SwitcherModel *model = NULL);
    virtual void switcherDestructor();
    virtual bool handleXEvent(const XEvent &event);
    virtual void updateButtons();
    virtual void windowToFront(Window window);
    virtual void closeWindow(Window window);
    virtual void closeAllWindows();
    virtual void scheduleUpdateButtons();
    virtual void handleWindowInfoList(QList<WindowInfo> newWindowList);
    virtual bool sceneEvent(QEvent *event);
    virtual void updateAnimationStatus(bool animating);
    virtual QSharedPointer<SwitcherButton> createSwitcherButton();
    virtual bool restoreButtonBeingRemoved(Window window, bool forceUpdateButtons);
};

void SwitcherStub::switcherConstructor(MWidget *parent, SwitcherModel *model)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidget *>(parent));
    params.append(new Parameter<SwitcherModel *>(model));
    stubMethodEntered("switcherConstructor", params);
}

void SwitcherStub::switcherDestructor()
{
    stubMethodEntered("switcherDestructor");
}

bool SwitcherStub::handleXEvent(const XEvent &event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const XEvent&>(event));
    stubMethodEntered("handleXEvent", params);
    return stubReturnValue<bool>("handleXEvent");
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

void SwitcherStub::closeAllWindows()
{
    stubMethodEntered("closeAllWindows");
}

void SwitcherStub::updateButtons()
{
    stubMethodEntered("updateButtons");
}

void SwitcherStub::scheduleUpdateButtons()
{
    stubMethodEntered("scheduleUpdateButtons");
}

void SwitcherStub::handleWindowInfoList(QList<WindowInfo> windowList)
{
    QList<ParameterBase *> params;
    params.append(new Parameter< QList<WindowInfo> > (windowList));
    stubMethodEntered("handleWindowInfoList", params);
}

bool SwitcherStub::sceneEvent(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter< QEvent* > (event));
    stubMethodEntered("sceneEvent", params);
    return stubReturnValue<bool>("sceneEvent");
}

void SwitcherStub::updateAnimationStatus(bool animating)
{
    QList<ParameterBase *> params;
    params.append(new Parameter< bool > (animating));
    stubMethodEntered("updateAnimationStatus", params);
}

QSharedPointer<SwitcherButton> SwitcherStub::createSwitcherButton()
{
    stubMethodEntered("createSwitcherButton");
    return stubReturnValue<QSharedPointer<SwitcherButton> >("createSwitcherButton");
}

bool SwitcherStub::restoreButtonBeingRemoved(Window window, bool forceUpdateButtons)
{
    QList<ParameterBase *> params;
    params.append(new Parameter< Window > (window));
    params.append(new Parameter< bool > (forceUpdateButtons));
    stubMethodEntered("restoreButtonBeingRemoved", params);
    return stubReturnValue<bool>("restoreButtonBeingRemoved");
}


SwitcherStub gDefaultSwitcherStub;
SwitcherStub *gSwitcherStub = &gDefaultSwitcherStub;

Switcher::Switcher(MWidget *parent, SwitcherModel *model) : MWidgetController(parent)
{
    gSwitcherStub->switcherConstructor(parent, model);
}

Switcher::~Switcher()
{
    gSwitcherStub->switcherDestructor();
}

bool Switcher::handleXEvent(const XEvent &event)
{
    return gSwitcherStub->handleXEvent(event);
}

void Switcher::windowToFront(Window window)
{
    gSwitcherStub->windowToFront(window);
}

void Switcher::closeWindow(Window window)
{
    gSwitcherStub->closeWindow(window);
}

void Switcher::closeAllWindows()
{
    gSwitcherStub->closeAllWindows();
}

void Switcher::updateButtons()
{
    gSwitcherStub->updateButtons();
}

void Switcher::handleWindowInfoList(QList<WindowInfo> windowList)
{
    gSwitcherStub->handleWindowInfoList(windowList);
}

void Switcher::scheduleUpdateButtons()
{
    gSwitcherStub->scheduleUpdateButtons();
}

bool Switcher::sceneEvent(QEvent *event)
{
    return gSwitcherStub->sceneEvent(event);
}

void Switcher::updateAnimationStatus(bool animating)
{
    gSwitcherStub->updateAnimationStatus(animating);
}

QSharedPointer<SwitcherButton> Switcher::createSwitcherButton()
{
    return gSwitcherStub->createSwitcherButton();
}

bool Switcher::restoreButtonBeingRemoved(Window window, bool forceUpdateButtons)
{
    return gSwitcherStub->restoreButtonBeingRemoved(window, forceUpdateButtons);
}

#endif
