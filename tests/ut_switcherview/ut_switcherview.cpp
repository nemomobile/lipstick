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

#include <DuiApplication>
#include <DuiWindow>
#include "ut_switcherview.h"
#include "switcherview.h"
#include "switcher_stub.h"
#include "switcherbutton.h"

SwitcherModel* g_switcherModel;
QMap<SwitcherButton *, Window> g_windowButtonMap;

// SwitcherButton stubs
SwitcherButton::SwitcherButton(const QString &title, DuiWidget *parent, Window window, WindowInfo::WindowPriority windowPriority) :
    DuiButton(title, parent),
    priority(windowPriority)
{
    Q_UNUSED(title);
    Q_UNUSED(parent);
    Q_UNUSED(window);

    g_windowButtonMap[this] = window;
}

SwitcherButton::~SwitcherButton()
{
    g_windowButtonMap.remove(this);
}

void SwitcherButton::setEmphasis()
{
}

void SwitcherButton::unsetEmphasis()
{
}

void SwitcherButton::switchToWindow()
{
}

void SwitcherButton::close()
{
}

void SwitcherButton::prepareGeometryChange()
{
}

void SwitcherButton::setGeometry(const QRectF &)
{
}


void SwitcherButton::updateIconGeometry()
{
}

WindowInfo::WindowPriority SwitcherButton::windowPriority() const
{
    return priority;
}

void SwitcherButton::setWindowPriority(WindowInfo::WindowPriority windowPriority)
{
    priority = windowPriority;
}

Window SwitcherButton::xWindow()
{
    return g_windowButtonMap[this];
}

bool g_panRequested;
uint g_panRequestIndex;

void SwitcherPhysicsIntegrationStrategy::panToItem(uint itemIndex) {
    g_panRequested = true;
    g_panRequestIndex = itemIndex;
}

SwitcherPhysicsIntegrationStrategy::SwitcherPhysicsIntegrationStrategy() {
}

SwitcherPhysicsIntegrationStrategy::~SwitcherPhysicsIntegrationStrategy() {
}

void SwitcherPhysicsIntegrationStrategy::integrate(qreal &, qreal &, qreal &, qreal &, qreal, qreal, DuiPhysics2DIntegrationStrategy::IntegrationData &) {
}

void SwitcherPhysicsIntegrationStrategy::setSnapInterval(uint) {
}

uint SwitcherPhysicsIntegrationStrategy::snapInterval() const {
    return 0;
}

void SwitcherPhysicsIntegrationStrategy::snapIntegrate(qreal &, qreal &, qreal &, qreal &, qreal, qreal, DuiPhysics2DIntegrationStrategy::IntegrationData &) {
}

void SwitcherPhysicsIntegrationStrategy::autoPanIntegrate(qreal &, qreal &, qreal &, qreal &, DuiPhysics2DIntegrationStrategy::IntegrationData &) {
}

void Ut_SwitcherView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_switcherview";
    app = new DuiApplication(argc, &app_name);
    window = new DuiWindow;
    g_switcherModel = new SwitcherModel;
}

void Ut_SwitcherView::cleanupTestCase()
{
    delete window;
    delete app;
}

QList< QSharedPointer<SwitcherButton> > createButtonList()
{
    QList< QSharedPointer<SwitcherButton> > buttonList;

    buttonList.append(QSharedPointer<SwitcherButton>(new SwitcherButton("Title 1", NULL, 1)));
    buttonList.append(QSharedPointer<SwitcherButton>(new SwitcherButton("Title 2", NULL, 2)));
    buttonList.append(QSharedPointer<SwitcherButton>(new SwitcherButton("Title 3", NULL, 3)));
    buttonList.append(QSharedPointer<SwitcherButton>(new SwitcherButton("Title 4", NULL, 4)));

    return buttonList;
}

void Ut_SwitcherView::init()
{
    g_panRequested = false;

    // Create test switcher
    switcher = new Switcher();
    switcher->setModel(g_switcherModel);
    m_subject = new SwitcherView(switcher);
    switcher->setView(m_subject);
    gSwitcherStub->stubReset();

    g_switcherModel->setButtons(createButtonList());
}

void Ut_SwitcherView::cleanup()
{
    delete m_subject;
    delete g_switcherModel;
}

void Ut_SwitcherView::testAutoPanning()
{
    QCOMPARE(g_panRequested, false);

    QList< QSharedPointer<SwitcherButton> > buttonList = createButtonList();
    // Change the first button's priority to
    // trigger the panning of the view to show it
    buttonList.first().data()->setWindowPriority(WindowInfo::Call);
    // Update the model with the modified list
    g_switcherModel->setButtons(buttonList);

    // SwitcherView should have called the physics integrator's pan method
    QCOMPARE(g_panRequested, true);
}

QTEST_APPLESS_MAIN(Ut_SwitcherView)
