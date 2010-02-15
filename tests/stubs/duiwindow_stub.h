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
#ifndef DUIWINDOW_STUB
#define DUIWINDOW_STUB

#include <DuiWindow>
#include "stubbase.h"


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiWindowStub : public StubBase
{
public:
    virtual void DuiWindowConstructor(QWidget *parent);
    virtual void DuiWindowConstructor(DuiSceneManager *sceneManager, QWidget *parent);
    virtual void DuiWindowDestructor();
    virtual void setTranslucentBackground(bool enable);
    virtual DuiScene *scene();
    virtual void setSceneManager(DuiSceneManager *sceneManager);
    virtual DuiSceneManager *sceneManager() const;
    virtual DuiSceneManager *sceneManager();
    virtual Dui::Orientation orientation() const;
    virtual Dui::OrientationAngle orientationAngle() const;
    virtual QSize visibleSceneSize(Dui::Orientation) const;
    virtual QSize visibleSceneSize() const;
    virtual bool keepCurrentOrientation() const;
    virtual void setKeepCurrentOrientation(bool enabled);
    virtual bool isOnDisplay() const;
    virtual void setOrientationAngle(Dui::OrientationAngle angle, Dui::OrientationChangeMode mode);
    virtual bool event(QEvent *event);
    virtual void onDisplayChangeEvent(DuiOnDisplayChangeEvent *event);
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    virtual void DuiWindowConstructor(DuiWindowPrivate &dd, QWidget *parent);
    virtual void DuiWindowConstructor(DuiWindowPrivate &dd, DuiScene *scene, QWidget *parent);
    virtual void DuiWindowConstructor(DuiWindowPrivate &dd, DuiSceneManager *sceneManager, QWidget *parent);
};

// 2. IMPLEMENT STUB
void DuiWindowStub::DuiWindowConstructor(QWidget *parent)
{
    Q_UNUSED(parent);

}
void DuiWindowStub::DuiWindowConstructor(DuiSceneManager *sceneManager, QWidget *parent)
{
    Q_UNUSED(sceneManager);
    Q_UNUSED(parent);

}
void DuiWindowStub::DuiWindowDestructor()
{

}
void DuiWindowStub::setTranslucentBackground(bool enable)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(enable));
    stubMethodEntered("setTranslucentBackground", params);
}

DuiScene *DuiWindowStub::scene()
{
    stubMethodEntered("scene");
    return stubReturnValue<DuiScene *>("scene");
}

void DuiWindowStub::setSceneManager(DuiSceneManager *sceneManager)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiSceneManager * >(sceneManager));
    stubMethodEntered("setSceneManager", params);
}

DuiSceneManager *DuiWindowStub::sceneManager() const
{
    stubMethodEntered("sceneManager");
    return stubReturnValue<DuiSceneManager *>("sceneManager");
}

DuiSceneManager *DuiWindowStub::sceneManager()
{
    stubMethodEntered("sceneManager");
    return stubReturnValue<DuiSceneManager *>("sceneManager");
}

Dui::Orientation DuiWindowStub::orientation() const
{
    stubMethodEntered("orientation");
    return stubReturnValue<Dui::Orientation>("orientation");
}

Dui::OrientationAngle DuiWindowStub::orientationAngle() const
{
    stubMethodEntered("orientationAngle");
    return stubReturnValue<Dui::OrientationAngle>("orientationAngle");
}

QSize DuiWindowStub::visibleSceneSize(Dui::Orientation orientation) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Dui::Orientation >((orientation)));
    stubMethodEntered("visibleSceneSize", params);
    return stubReturnValue<QSize>("visibleSceneSize");
}

QSize DuiWindowStub::visibleSceneSize() const
{
    stubMethodEntered("visibleSceneSize");
    return stubReturnValue<QSize>("visibleSceneSize");
}

bool DuiWindowStub::keepCurrentOrientation() const
{
    stubMethodEntered("keepCurrentOrientation");
    return stubReturnValue<bool>("keepCurrentOrientation");
}

void DuiWindowStub::setKeepCurrentOrientation(bool enabled)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(enabled));
    stubMethodEntered("setKeepCurrentOrientation", params);
}

bool DuiWindowStub::isOnDisplay() const
{
    stubMethodEntered("isOnDisplay");
    return stubReturnValue<bool>("isOnDisplay");
}

void DuiWindowStub::setOrientationAngle(Dui::OrientationAngle angle, Dui::OrientationChangeMode mode)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Dui::OrientationAngle >(angle));
    params.append(new Parameter<Dui::OrientationChangeMode >(mode));
    stubMethodEntered("setOrientationAngle", params);
}

bool DuiWindowStub::event(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("event", params);
    return stubReturnValue<bool>("event");
}

void DuiWindowStub::onDisplayChangeEvent(DuiOnDisplayChangeEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiOnDisplayChangeEvent * >(event));
    stubMethodEntered("onDisplayChangeEvent", params);
}

void DuiWindowStub::enterDisplayEvent()
{
    stubMethodEntered("enterDisplayEvent");
}

void DuiWindowStub::exitDisplayEvent()
{
    stubMethodEntered("exitDisplayEvent");
}

void DuiWindowStub::DuiWindowConstructor(DuiWindowPrivate &dd, QWidget *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(parent);

}
void DuiWindowStub::DuiWindowConstructor(DuiWindowPrivate &dd, DuiScene *scene, QWidget *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(scene);
    Q_UNUSED(parent);

}
void DuiWindowStub::DuiWindowConstructor(DuiWindowPrivate &dd, DuiSceneManager *sceneManager, QWidget *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(sceneManager);
    Q_UNUSED(parent);

}


// 3. CREATE A STUB INSTANCE
DuiWindowStub gDefaultDuiWindowStub;
DuiWindowStub *gDuiWindowStub = &gDefaultDuiWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiWindow::DuiWindow(QWidget *parent) :
    d_ptr(NULL)
{
    gDuiWindowStub->DuiWindowConstructor(parent);
}

DuiWindow::DuiWindow(DuiSceneManager *sceneManager, QWidget *parent) :
    d_ptr(NULL)
{
    gDuiWindowStub->DuiWindowConstructor(sceneManager, parent);
}

DuiWindow::~DuiWindow()
{
    gDuiWindowStub->DuiWindowDestructor();
}

void DuiWindow::setTranslucentBackground(bool enable)
{
    gDuiWindowStub->setTranslucentBackground(enable);
}

DuiScene *DuiWindow::scene()
{
    return gDuiWindowStub->scene();
}

void DuiWindow::setSceneManager(DuiSceneManager *sceneManager)
{
    gDuiWindowStub->setSceneManager(sceneManager);
}

DuiSceneManager *DuiWindow::sceneManager() const
{
    return gDuiWindowStub->sceneManager();
}

DuiSceneManager *DuiWindow::sceneManager()
{
    return gDuiWindowStub->sceneManager();
}

Dui::Orientation DuiWindow::orientation() const
{
    return gDuiWindowStub->orientation();
}

Dui::OrientationAngle DuiWindow::orientationAngle() const
{
    return gDuiWindowStub->orientationAngle();
}

QSize DuiWindow::visibleSceneSize(Dui::Orientation) const
{
    return gDuiWindowStub->visibleSceneSize();
}

QSize DuiWindow::visibleSceneSize() const
{
    return gDuiWindowStub->visibleSceneSize();
}

bool DuiWindow::keepCurrentOrientation() const
{
    return gDuiWindowStub->keepCurrentOrientation();
}

void DuiWindow::setKeepCurrentOrientation(bool enabled)
{
    gDuiWindowStub->setKeepCurrentOrientation(enabled);
}

bool DuiWindow::isOnDisplay() const
{
    return gDuiWindowStub->isOnDisplay();
}

void DuiWindow::setOrientationAngle(Dui::OrientationAngle angle, Dui::OrientationChangeMode mode)
{
    gDuiWindowStub->setOrientationAngle(angle, mode);
}

bool DuiWindow::event(QEvent *event)
{
    return gDuiWindowStub->event(event);
}

void DuiWindow::onDisplayChangeEvent(DuiOnDisplayChangeEvent *event)
{
    gDuiWindowStub->onDisplayChangeEvent(event);
}

void DuiWindow::enterDisplayEvent()
{
    gDuiWindowStub->enterDisplayEvent();
}

void DuiWindow::exitDisplayEvent()
{
    gDuiWindowStub->exitDisplayEvent();
}

DuiWindow::DuiWindow(DuiWindowPrivate &dd, QWidget *parent) :
    d_ptr(NULL)
{
    gDuiWindowStub->DuiWindowConstructor(dd, parent);
}

DuiWindow::DuiWindow(DuiWindowPrivate &dd, DuiScene *scene, QWidget *parent) :
    d_ptr(NULL)
{
    gDuiWindowStub->DuiWindowConstructor(dd, scene, parent);
}

DuiWindow::DuiWindow(DuiWindowPrivate &dd, DuiSceneManager *sceneManager, QWidget *parent) :
    d_ptr(NULL)
{
    gDuiWindowStub->DuiWindowConstructor(dd, sceneManager, parent);
}


#endif
