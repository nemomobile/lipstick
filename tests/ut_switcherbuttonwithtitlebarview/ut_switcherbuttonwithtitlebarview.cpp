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
#include <QtTest/QtTest>

#include "ut_switcherbuttonwithtitlebarview.h"
#include <MCancelEvent>
#include "mscenemanager_stub.h"
#include <MScene>
#include <MLabel>
#include "mainwindow_stub.h"
#include "homeapplication_stub.h"
#include "switcher_stub.h"
#include "windowinfo_stub.h"
#include "x11wrapper_stub.h"

MainWindow *Ut_SwitcherButtonWithTitleBarView::mainWindow = NULL;

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

// Test switcher button implementation
TestSwitcherButton::TestSwitcherButton(QGraphicsItem *parent) :
    SwitcherButton(parent)
{
    view = new TestSwitcherButtonView(*this);
    setView(view);
}

TestSwitcherButtonView *TestSwitcherButton::getView()
{
    return dynamic_cast<TestSwitcherButtonView *>(view);
}

// Test switcher button view implementation
TestSwitcherButtonView::TestSwitcherButtonView(SwitcherButton &button) :
    SwitcherButtonWithTitleBarView(&button)
{
}

// Test home application
class TestHomeApplication : public HomeApplication
{
public:
    TestHomeApplication(int &argc, char **argv);
};

TestHomeApplication::TestHomeApplication(int &argc, char **argv) : HomeApplication(argc, argv)
{
}

void Ut_SwitcherButtonWithTitleBarView::init()
{
    button = new TestSwitcherButton;
    button->setText("Test");
    m_subject = button->getView();
    m_subject->modifiableStyle()->setCroppingTop(0);
    croppingTop = m_subject->modifiableStyle()->croppingTop();
}

void Ut_SwitcherButtonWithTitleBarView::cleanup()
{
    // Destroy the controller which will also destroy the view
    delete button;

    gHomeApplicationStub->stubReset();
}

void Ut_SwitcherButtonWithTitleBarView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_switcherbuttonview";
    app = new TestHomeApplication(argc, &app_name);
    mainWindow = MainWindow::instance(true);
    gHomeApplicationStub->stubSetReturnValue("mainWindow", mainWindow);
}

void Ut_SwitcherButtonWithTitleBarView::cleanupTestCase()
{
    delete mainWindow;
    delete app;
}

void Ut_SwitcherButtonWithTitleBarView::testClosing()
{
    QSignalSpy closeSpy(button, SIGNAL(closeWindow(Window)));

    MWidget *parent = new MWidget();
    MWidget *switcher = new MWidget(parent);
    button->setParentItem(switcher);

    // "Click" the close button: check that a closeWindow signal is fired
    button->getView()->closeButton->click();

    QCOMPARE(closeSpy.count(), 1);
}

void Ut_SwitcherButtonWithTitleBarView::testApplyingStyle()
{
    QVERIFY(!m_subject->closeButton->isVisible());
    QVERIFY(!m_subject->titleLabel->isVisible());

    button->setObjectName("OverviewButton");
    m_subject->modifiableStyle()->setCroppingTop(100);
    button->getView()->applyStyle();
    QVERIFY(m_subject->closeButton->isVisible());
    QVERIFY(m_subject->titleLabel->isVisible());
    QCOMPARE(m_subject->closeButton->objectName(), QString("CloseButtonOverviewLarge"));
    QCOMPARE(m_subject->titleLabel->objectName(), QString("SwitcherButtonTitleLabelOverviewLarge"));

    button->setObjectName("DetailviewButton");
    button->model()->setViewMode(SwitcherButtonModel::Large);
    m_subject->modifiableStyle()->setCroppingTop(100);
    button->getView()->applyStyle();
    QVERIFY(m_subject->closeButton->isVisible());
    QVERIFY(m_subject->titleLabel->isVisible());
    QCOMPARE(m_subject->closeButton->objectName(), QString("CloseButtonDetailview"));
    QCOMPARE(m_subject->titleLabel->objectName(), QString("SwitcherButtonTitleLabelDetailview"));

    button->model()->setViewMode(SwitcherButtonModel::Medium);
    m_subject->modifiableStyle()->setCroppingTop(100);
    button->getView()->applyStyle();
    QVERIFY(!m_subject->closeButton->isVisible());
    QVERIFY(m_subject->titleLabel->isVisible());
}

void Ut_SwitcherButtonWithTitleBarView::testThumbnailPosition()
{
    const int titleHeight = m_subject->titleLabel->size().height();

    QPoint expectedPosition(0, titleHeight);
    QCOMPARE(m_subject->thumbnailPosition(), expectedPosition);
}

QTEST_APPLESS_MAIN(Ut_SwitcherButtonWithTitleBarView)
