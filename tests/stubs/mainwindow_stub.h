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
#ifndef MAINWINDOW_STUB
#define MAINWINDOW_STUB

#include "mainwindow.h"
#include <stubbase.h>


// 1. DECLARE STUB
class MainWindowStub : public StubBase
{
public:
    virtual void MainWindowDestructor();
    virtual MainWindow *instance(bool create);
    virtual QGLContext *glContext();
    virtual void MainWindowConstructor(QWidget *parent);
    virtual void excludeFromTaskBar();
    virtual void changeNetWmState(bool set, Atom one, Atom two);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void markKeyPressesSentAndSendRemainingKeyPresses();
    virtual void markKeyPressesNotSent();
};

// 2. IMPLEMENT STUB
void MainWindowStub::MainWindowDestructor()
{

}
MainWindow *MainWindowStub::instance(bool create)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(create));
    stubMethodEntered("instance", params);
    return stubReturnValue<MainWindow *>("instance");
}

QGLContext *MainWindowStub::glContext()
{
    stubMethodEntered("glContext");
    return stubReturnValue<QGLContext *>("glContext");
}

void MainWindowStub::MainWindowConstructor(QWidget *parent)
{
    Q_UNUSED(parent);

}
void MainWindowStub::excludeFromTaskBar()
{
    stubMethodEntered("excludeFromTaskBar");
}

void MainWindowStub::changeNetWmState(bool set, Atom one, Atom two)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(set));
    params.append(new Parameter<Atom >(one));
    params.append(new Parameter<Atom >(two));
    stubMethodEntered("changeNetWmState", params);
}

void MainWindowStub::keyPressEvent(QKeyEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QKeyEvent*>(event));
    stubMethodEntered("keyPressEvent", params);
}

void MainWindowStub::closeEvent(QCloseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QCloseEvent*>(event));
    stubMethodEntered("closeEvent", params);
}

void MainWindowStub::markKeyPressesSentAndSendRemainingKeyPresses()
{
    stubMethodEntered("markKeyPressesSentAndSendRemainingKeyPresses");
}

void MainWindowStub::markKeyPressesNotSent()
{
    stubMethodEntered("markKeyPressesNotSent");
}

// 3. CREATE A STUB INSTANCE
MainWindowStub gDefaultMainWindowStub;
MainWindowStub *gMainWindowStub = &gDefaultMainWindowStub;
MainWindow *MainWindow::mainWindowInstance = NULL;

// 4. CREATE A PROXY WHICH CALLS THE STUB
MainWindow::~MainWindow()
{
    gMainWindowStub->MainWindowDestructor();
    mainWindowInstance = NULL;
}

MainWindow *MainWindow::instance(bool create)
{
    if (mainWindowInstance == NULL && create) {
        new MainWindow;
    }

    return mainWindowInstance;
}

QGLContext *MainWindow::glContext()
{
    return gMainWindowStub->glContext();
}

MainWindow::MainWindow(QWidget *parent)
{
    gMainWindowStub->MainWindowConstructor(parent);
    mainWindowInstance = this;
}

void MainWindow::excludeFromTaskBar()
{
    gMainWindowStub->excludeFromTaskBar();
}

void MainWindow::changeNetWmState(bool set, Atom one, Atom two)
{
    gMainWindowStub->changeNetWmState(set, one, two);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    gMainWindowStub->keyPressEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    gMainWindowStub->closeEvent(event);
}

void MainWindow::markKeyPressesSentAndSendRemainingKeyPresses()
{
    gMainWindowStub->markKeyPressesSentAndSendRemainingKeyPresses();
}

void MainWindow::markKeyPressesNotSent()
{
    gMainWindowStub->markKeyPressesNotSent();
}

#endif
