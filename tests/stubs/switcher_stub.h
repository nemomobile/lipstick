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
    virtual void windowListUpdated(const QList<WindowInfo> &windowList);
    virtual void windowToFront(Window window);
    virtual void closeWindow(Window window);
    virtual void viewportSizePosChanged(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos);
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

void SwitcherStub::windowListUpdated(const QList<WindowInfo> &windowList)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QList<WindowInfo> >(windowList));
    stubMethodEntered("windowListUpdated", params);
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

void SwitcherStub::viewportSizePosChanged(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QSizeF>(viewportSize));
    params.append(new Parameter<QRectF>(pannedRange));
    params.append(new Parameter<QPointF>(pannedPos));
    stubMethodEntered("viewportSizePosChanged", params);
}


SwitcherStub gDefaultSwitcherStub;
SwitcherStub *gSwitcherStub = &gDefaultSwitcherStub;

Switcher::Switcher(MWidget *parent)
{
    gSwitcherStub->switcherConstructor(parent);
}

Switcher::~Switcher()
{
    gSwitcherStub->switcherDestructor();
}

void Switcher::windowListUpdated(const QList<WindowInfo> &windowList)
{
    gSwitcherStub->windowListUpdated(windowList);
}

void Switcher::windowToFront(Window window)
{
    gSwitcherStub->windowToFront(window);
}

void Switcher::closeWindow(Window window)
{
    gSwitcherStub->closeWindow(window);
}

void Switcher::viewportSizePosChanged(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos)
{
    gSwitcherStub->viewportSizePosChanged(viewportSize, pannedRange, pannedPos);
}

#endif
