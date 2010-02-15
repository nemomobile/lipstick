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
#ifndef DUIAPPLICATION_STUB
#define DUIAPPLICATION_STUB

#include <DuiApplication>
#include "stubbase.h"


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiApplicationStub : public StubBase
{
public:
    virtual void DuiApplicationConstructor(int &argc, char **argv, const QString &appIdentifier, DuiApplicationService *service);
    virtual void DuiApplicationConstructor(int &argc, char **argv, DuiApplicationService *service);
    virtual void DuiApplicationDestructor();
    virtual DuiApplication *instance();
    virtual QString deviceName();
    virtual bool softwareRendering();
    virtual bool fullScreen();
    virtual bool showBoundingRect();
    virtual bool showFps();
    virtual bool showSize();
    virtual bool showPosition();
    virtual bool showMargins();
    virtual bool showObjectNames();
    virtual bool showCursor();
    virtual void setShowPosition(bool show);
    virtual void setShowMargins(bool show);
    virtual void setShowObjectNames(bool show);
    virtual void setShowSize(bool show);
    virtual void setShowBoundingRect(bool show);
    virtual void setShowFps(bool show);
    virtual void setShowCursor(bool show);
    virtual DuiApplicationWindow *activeApplicationWindow();
    virtual DuiWindow *activeWindow();
    virtual QList<DuiWindow *> windows();
    virtual QString appName();
    virtual QString binaryName();
    virtual DuiFeedbackPlayer *feedbackPlayer();
    virtual bool isLoadDuiInputContextEnabled();
    virtual void setLoadDuiInputContext(bool enable);
    virtual void fastFloatMath(bool val);
    virtual void setPrestartMode(Dui::PrestartMode mode);
    virtual Dui::PrestartMode prestartMode();
    virtual bool isPrestarted();
    virtual void restorePrestart();
    virtual void releasePrestart();
    virtual void damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height);
    virtual void DuiApplicationConstructor(DuiApplicationPrivate &dd, int &argc, char **argv, const QString &appIdentifier);
    virtual bool x11EventFilter(XEvent *event);
};

// 2. IMPLEMENT STUB
void DuiApplicationStub::DuiApplicationConstructor(int &argc, char **argv, const QString &appIdentifier, DuiApplicationService *service)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    Q_UNUSED(appIdentifier);
    Q_UNUSED(service);

}
void DuiApplicationStub::DuiApplicationConstructor(int &argc, char **argv, DuiApplicationService *service)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    Q_UNUSED(service);

}
void DuiApplicationStub::DuiApplicationDestructor()
{

}
DuiApplication *DuiApplicationStub::instance()
{
    stubMethodEntered("instance");
    return stubReturnValue<DuiApplication *>("instance");
}

QString DuiApplicationStub::deviceName()
{
    stubMethodEntered("deviceName");
    return stubReturnValue<QString>("deviceName");
}

bool DuiApplicationStub::softwareRendering()
{
    stubMethodEntered("softwareRendering");
    return stubReturnValue<bool>("softwareRendering");
}

bool DuiApplicationStub::fullScreen()
{
    stubMethodEntered("fullScreen");
    return stubReturnValue<bool>("fullScreen");
}

bool DuiApplicationStub::showBoundingRect()
{
    stubMethodEntered("showBoundingRect");
    return stubReturnValue<bool>("showBoundingRect");
}

bool DuiApplicationStub::showFps()
{
    stubMethodEntered("showFps");
    return stubReturnValue<bool>("showFps");
}

bool DuiApplicationStub::showSize()
{
    stubMethodEntered("showSize");
    return stubReturnValue<bool>("showSize");
}

bool DuiApplicationStub::showPosition()
{
    stubMethodEntered("showPosition");
    return stubReturnValue<bool>("showPosition");
}

bool DuiApplicationStub::showMargins()
{
    stubMethodEntered("showMargins");
    return stubReturnValue<bool>("showMargins");
}

bool DuiApplicationStub::showObjectNames()
{
    stubMethodEntered("showObjectNames");
    return stubReturnValue<bool>("showObjectNames");
}

bool DuiApplicationStub::showCursor()
{
    stubMethodEntered("showCursor");
    return stubReturnValue<bool>("showCursor");
}

void DuiApplicationStub::setShowPosition(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowPosition", params);
}

void DuiApplicationStub::setShowMargins(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowMargins", params);
}

void DuiApplicationStub::setShowObjectNames(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowObjectNames", params);
}

void DuiApplicationStub::setShowSize(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowSize", params);
}

void DuiApplicationStub::setShowBoundingRect(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowBoundingRect", params);
}

void DuiApplicationStub::setShowFps(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowFps", params);
}

void DuiApplicationStub::setShowCursor(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowCursor", params);
}

DuiApplicationWindow *DuiApplicationStub::activeApplicationWindow()
{
    stubMethodEntered("activeApplicationWindow");
    return stubReturnValue<DuiApplicationWindow *>("activeApplicationWindow");
}

DuiWindow *DuiApplicationStub::activeWindow()
{
    stubMethodEntered("activeWindow");
    return stubReturnValue<DuiWindow *>("activeWindow");
}

QList<DuiWindow *> DuiApplicationStub::windows()
{
    stubMethodEntered("windows");
    return stubReturnValue<QList<DuiWindow *> >("windows");
}

QString DuiApplicationStub::appName()
{
    stubMethodEntered("appName");
    return stubReturnValue<QString>("appName");
}

QString DuiApplicationStub::binaryName()
{
    stubMethodEntered("binaryName");
    return stubReturnValue<QString>("binaryName");
}

DuiFeedbackPlayer *DuiApplicationStub::feedbackPlayer()
{
    stubMethodEntered("feedbackPlayer");
    return stubReturnValue<DuiFeedbackPlayer *>("feedbackPlayer");
}

bool DuiApplicationStub::isLoadDuiInputContextEnabled()
{
    stubMethodEntered("isLoadDuiInputContextEnabled");
    return stubReturnValue<bool>("isLoadDuiInputContextEnabled");
}

void DuiApplicationStub::setLoadDuiInputContext(bool enable)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(enable));
    stubMethodEntered("setLoadDuiInputContext", params);
}

void DuiApplicationStub::fastFloatMath(bool val)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(val));
    stubMethodEntered("fastFloatMath", params);
}

void DuiApplicationStub::setPrestartMode(Dui::PrestartMode mode)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Dui::PrestartMode >(mode));
    stubMethodEntered("setPrestartMode", params);
}

Dui::PrestartMode DuiApplicationStub::prestartMode()
{
    stubMethodEntered("prestartMode");
    return stubReturnValue<Dui::PrestartMode>("prestartMode");
}

bool DuiApplicationStub::isPrestarted()
{
    stubMethodEntered("isPrestarted");
    return stubReturnValue<bool>("isPrestarted");
}

void DuiApplicationStub::restorePrestart()
{
    stubMethodEntered("restorePrestart");
}

void DuiApplicationStub::releasePrestart()
{
    stubMethodEntered("releasePrestart");
}

void DuiApplicationStub::damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::HANDLE & >(damage));
    params.append(new Parameter<short & >(x));
    params.append(new Parameter<short & >(y));
    params.append(new Parameter<unsigned short & >(width));
    params.append(new Parameter<unsigned short & >(height));
    stubMethodEntered("damageEvent", params);
}

void DuiApplicationStub::DuiApplicationConstructor(DuiApplicationPrivate &dd, int &argc, char **argv, const QString &appIdentifier)
{
    Q_UNUSED(dd);
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    Q_UNUSED(appIdentifier);

}
bool DuiApplicationStub::x11EventFilter(XEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<XEvent * >(event));
    stubMethodEntered("x11EventFilter", params);
    return stubReturnValue<bool>("x11EventFilter");
}



// 3. CREATE A STUB INSTANCE
DuiApplicationStub gDefaultDuiApplicationStub;
DuiApplicationStub *gDuiApplicationStub = &gDefaultDuiApplicationStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiApplication::DuiApplication(int &argc, char **argv, const QString &appIdentifier, DuiApplicationService *service) :
    QApplication(argc, argv),
    d_ptr(NULL)
{
    gDuiApplicationStub->DuiApplicationConstructor(argc, argv, appIdentifier, service);
}

DuiApplication::DuiApplication(int &argc, char **argv, DuiApplicationService *service) :
    QApplication(argc, argv),
    d_ptr(NULL)
{
    gDuiApplicationStub->DuiApplicationConstructor(argc, argv, service);
}

DuiApplication::~DuiApplication()
{
    gDuiApplicationStub->DuiApplicationDestructor();
}

DuiApplication *DuiApplication::instance()
{
    return gDuiApplicationStub->instance();
}

QString DuiApplication::deviceName()
{
    return gDuiApplicationStub->deviceName();
}

bool DuiApplication::softwareRendering()
{
    return gDuiApplicationStub->softwareRendering();
}

bool DuiApplication::fullScreen()
{
    return gDuiApplicationStub->fullScreen();
}

bool DuiApplication::showBoundingRect()
{
    return gDuiApplicationStub->showBoundingRect();
}

bool DuiApplication::showFps()
{
    return gDuiApplicationStub->showFps();
}

bool DuiApplication::showSize()
{
    return gDuiApplicationStub->showSize();
}

bool DuiApplication::showPosition()
{
    return gDuiApplicationStub->showPosition();
}

bool DuiApplication::showMargins()
{
    return gDuiApplicationStub->showMargins();
}

bool DuiApplication::showObjectNames()
{
    return gDuiApplicationStub->showObjectNames();
}

bool DuiApplication::showCursor()
{
    return gDuiApplicationStub->showCursor();
}

void DuiApplication::setShowPosition(bool show)
{
    gDuiApplicationStub->setShowPosition(show);
}

void DuiApplication::setShowMargins(bool show)
{
    gDuiApplicationStub->setShowMargins(show);
}

void DuiApplication::setShowObjectNames(bool show)
{
    gDuiApplicationStub->setShowObjectNames(show);
}

void DuiApplication::setShowSize(bool show)
{
    gDuiApplicationStub->setShowSize(show);
}

void DuiApplication::setShowBoundingRect(bool show)
{
    gDuiApplicationStub->setShowBoundingRect(show);
}

void DuiApplication::setShowFps(bool show)
{
    gDuiApplicationStub->setShowFps(show);
}

void DuiApplication::setShowCursor(bool show)
{
    gDuiApplicationStub->setShowCursor(show);
}

DuiApplicationWindow *DuiApplication::activeApplicationWindow()
{
    return gDuiApplicationStub->activeApplicationWindow();
}

DuiWindow *DuiApplication::activeWindow()
{
    return gDuiApplicationStub->activeWindow();
}

QList<DuiWindow *> DuiApplication::windows()
{
    return gDuiApplicationStub->windows();
}

QString DuiApplication::appName()
{
    return gDuiApplicationStub->appName();
}

QString DuiApplication::binaryName()
{
    return gDuiApplicationStub->binaryName();
}

DuiFeedbackPlayer *DuiApplication::feedbackPlayer()
{
    return gDuiApplicationStub->feedbackPlayer();
}

bool DuiApplication::isLoadDuiInputContextEnabled()
{
    return gDuiApplicationStub->isLoadDuiInputContextEnabled();
}

void DuiApplication::setLoadDuiInputContext(bool enable)
{
    gDuiApplicationStub->setLoadDuiInputContext(enable);
}

#ifdef __arm__
void DuiApplication::fastFloatMath(bool val)
{
    gDuiApplicationStub->fastFloatMath(val);
}
#endif

void DuiApplication::setPrestartMode(Dui::PrestartMode mode)
{
    gDuiApplicationStub->setPrestartMode(mode);
}

Dui::PrestartMode DuiApplication::prestartMode()
{
    return gDuiApplicationStub->prestartMode();
}

bool DuiApplication::isPrestarted()
{
    return gDuiApplicationStub->isPrestarted();
}

void DuiApplication::restorePrestart()
{
    gDuiApplicationStub->restorePrestart();
}

void DuiApplication::releasePrestart()
{
    gDuiApplicationStub->releasePrestart();
}

void DuiApplication::damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height)
{
    gDuiApplicationStub->damageEvent(damage, x, y, width, height);
}

DuiApplication::DuiApplication(DuiApplicationPrivate &dd, int &argc, char **argv, const QString &appIdentifier) :
    QApplication(argc, argv),
    d_ptr(NULL)
{
    gDuiApplicationStub->DuiApplicationConstructor(dd, argc, argv, appIdentifier);
}

bool DuiApplication::x11EventFilter(XEvent *event)
{
    return gDuiApplicationStub->x11EventFilter(event);
}


#endif
