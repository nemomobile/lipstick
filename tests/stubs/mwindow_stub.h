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
#ifndef MWINDOW_STUB
#define MWINDOW_STUB

#include <MWindow>
#include "stubbase.h"


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MWindowStub : public StubBase
{
public:
    virtual void MWindowConstructor(QWidget *parent);
    virtual void MWindowConstructor(MSceneManager *sceneManager, QWidget *parent);
    virtual void MWindowDestructor();
    virtual void setTranslucentBackground(bool enable);
    virtual MScene *scene();
    virtual void setSceneManager(MSceneManager *sceneManager);
    virtual MSceneManager *sceneManager() const;
    virtual MSceneManager *sceneManager();
    virtual M::Orientation orientation() const;
    virtual M::OrientationAngle orientationAngle() const;
    virtual QSize visibleSceneSize(M::Orientation) const;
    virtual QSize visibleSceneSize() const;
    virtual bool isOrientationLocked() const;
    virtual void setOrientationLocked(bool enabled);
    virtual bool isOnDisplay() const;
    virtual void setOrientationAngle(M::OrientationAngle angle);
    virtual bool event(QEvent *event);
    virtual void onDisplayChangeEvent(MOnDisplayChangeEvent *event);
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    virtual void MWindowConstructor(MWindowPrivate &dd, QWidget *parent);
    virtual void MWindowConstructor(MWindowPrivate &dd, MScene *scene, QWidget *parent);
    virtual void MWindowConstructor(MWindowPrivate &dd, MSceneManager *sceneManager, QWidget *parent);
};

// 2. IMPLEMENT STUB
void MWindowStub::MWindowConstructor(QWidget *parent)
{
    Q_UNUSED(parent);

}
void MWindowStub::MWindowConstructor(MSceneManager *sceneManager, QWidget *parent)
{
    Q_UNUSED(sceneManager);
    Q_UNUSED(parent);

}
void MWindowStub::MWindowDestructor()
{

}
void MWindowStub::setTranslucentBackground(bool enable)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(enable));
    stubMethodEntered("setTranslucentBackground", params);
}

MScene *MWindowStub::scene()
{
    stubMethodEntered("scene");
    return stubReturnValue<MScene *>("scene");
}

void MWindowStub::setSceneManager(MSceneManager *sceneManager)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MSceneManager * >(sceneManager));
    stubMethodEntered("setSceneManager", params);
}

MSceneManager *MWindowStub::sceneManager() const
{
    stubMethodEntered("sceneManager");
    return stubReturnValue<MSceneManager *>("sceneManager");
}

MSceneManager *MWindowStub::sceneManager()
{
    stubMethodEntered("sceneManager");
    return stubReturnValue<MSceneManager *>("sceneManager");
}

M::Orientation MWindowStub::orientation() const
{
    stubMethodEntered("orientation");
    return stubReturnValue<M::Orientation>("orientation");
}

M::OrientationAngle MWindowStub::orientationAngle() const
{
    stubMethodEntered("orientationAngle");
    return stubReturnValue<M::OrientationAngle>("orientationAngle");
}

QSize MWindowStub::visibleSceneSize(M::Orientation orientation) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<M::Orientation >((orientation)));
    stubMethodEntered("visibleSceneSize", params);
    return stubReturnValue<QSize>("visibleSceneSize");
}

QSize MWindowStub::visibleSceneSize() const
{
    stubMethodEntered("visibleSceneSize");
    return stubReturnValue<QSize>("visibleSceneSize");
}

bool MWindowStub::isOrientationLocked() const
{
    stubMethodEntered("isOrientationLocked");
    return stubReturnValue<bool>("isOrientationLocked");
}

void MWindowStub::setOrientationLocked(bool enabled)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(enabled));
    stubMethodEntered("setOrientationLocked", params);
}

bool MWindowStub::isOnDisplay() const
{
    stubMethodEntered("isOnDisplay");
    return stubReturnValue<bool>("isOnDisplay");
}

void MWindowStub::setOrientationAngle(M::OrientationAngle angle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<M::OrientationAngle >(angle));
    stubMethodEntered("setOrientationAngle", params);
}

bool MWindowStub::event(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("event", params);
    return stubReturnValue<bool>("event");
}

void MWindowStub::onDisplayChangeEvent(MOnDisplayChangeEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MOnDisplayChangeEvent * >(event));
    stubMethodEntered("onDisplayChangeEvent", params);
}

void MWindowStub::enterDisplayEvent()
{
    stubMethodEntered("enterDisplayEvent");
}

void MWindowStub::exitDisplayEvent()
{
    stubMethodEntered("exitDisplayEvent");
}

void MWindowStub::MWindowConstructor(MWindowPrivate &dd, QWidget *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(parent);

}
void MWindowStub::MWindowConstructor(MWindowPrivate &dd, MScene *scene, QWidget *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(scene);
    Q_UNUSED(parent);

}
void MWindowStub::MWindowConstructor(MWindowPrivate &dd, MSceneManager *sceneManager, QWidget *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(sceneManager);
    Q_UNUSED(parent);

}


// 3. CREATE A STUB INSTANCE
MWindowStub gDefaultMWindowStub;
MWindowStub *gMWindowStub = &gDefaultMWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MWindow::MWindow(QWidget *parent) :
    d_ptr(NULL)
{
    gMWindowStub->MWindowConstructor(parent);
}

MWindow::MWindow(MSceneManager *sceneManager, QWidget *parent) :
    d_ptr(NULL)
{
    gMWindowStub->MWindowConstructor(sceneManager, parent);
}

MWindow::~MWindow()
{
    gMWindowStub->MWindowDestructor();
}

void MWindow::setTranslucentBackground(bool enable)
{
    gMWindowStub->setTranslucentBackground(enable);
}

MScene *MWindow::scene()
{
    return gMWindowStub->scene();
}

void MWindow::setSceneManager(MSceneManager *sceneManager)
{
    gMWindowStub->setSceneManager(sceneManager);
}

MSceneManager *MWindow::sceneManager() const
{
    return gMWindowStub->sceneManager();
}

MSceneManager *MWindow::sceneManager()
{
    return gMWindowStub->sceneManager();
}

M::Orientation MWindow::orientation() const
{
    return gMWindowStub->orientation();
}

M::OrientationAngle MWindow::orientationAngle() const
{
    return gMWindowStub->orientationAngle();
}

QSize MWindow::visibleSceneSize(M::Orientation) const
{
    return gMWindowStub->visibleSceneSize();
}

QSize MWindow::visibleSceneSize() const
{
    return gMWindowStub->visibleSceneSize();
}

bool MWindow::isOrientationLocked() const
{
    return gMWindowStub->isOrientationLocked();
}

void MWindow::setOrientationLocked(bool enabled)
{
    gMWindowStub->setOrientationLocked(enabled);
}

bool MWindow::isOnDisplay() const
{
    return gMWindowStub->isOnDisplay();
}

void MWindow::setOrientationAngle(M::OrientationAngle angle)
{
    gMWindowStub->setOrientationAngle(angle);
}

bool MWindow::event(QEvent *event)
{
    return gMWindowStub->event(event);
}

void MWindow::onDisplayChangeEvent(MOnDisplayChangeEvent *event)
{
    gMWindowStub->onDisplayChangeEvent(event);
}

void MWindow::enterDisplayEvent()
{
    gMWindowStub->enterDisplayEvent();
}

void MWindow::exitDisplayEvent()
{
    gMWindowStub->exitDisplayEvent();
}

MWindow::MWindow(MWindowPrivate &dd, QWidget *parent) :
    d_ptr(NULL)
{
    gMWindowStub->MWindowConstructor(dd, parent);
}

MWindow::MWindow(MWindowPrivate &dd, MScene *scene, QWidget *parent) :
    d_ptr(NULL)
{
    gMWindowStub->MWindowConstructor(dd, scene, parent);
}

MWindow::MWindow(MWindowPrivate &dd, MSceneManager *sceneManager, QWidget *parent) :
    d_ptr(NULL)
{
    gMWindowStub->MWindowConstructor(dd, sceneManager, parent);
}


#endif
