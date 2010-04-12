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
#ifndef MAPPLICATION_STUB
#define MAPPLICATION_STUB

#include <MApplication>
#include "stubbase.h"


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MApplicationStub : public StubBase
{
public:
    virtual void MApplicationConstructor(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service);
    virtual void MApplicationConstructor(int &argc, char **argv, MApplicationService *service);
    virtual void MApplicationDestructor();
    virtual MApplication *instance();
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
    virtual MApplicationWindow *activeApplicationWindow();
    virtual MWindow *activeWindow();
    virtual QList<MWindow *> windows();
    virtual QString appName();
    virtual QString binaryName();
    virtual MFeedbackPlayer *feedbackPlayer();
    virtual bool isLoadMInputContextEnabled();
    virtual void setLoadMInputContext(bool enable);
    virtual void fastFloatMath(bool val);
    virtual void setPrestartMode(M::PrestartMode mode);
    virtual M::PrestartMode prestartMode();
    virtual bool isPrestarted();
    virtual void restorePrestart();
    virtual void releasePrestart();
    virtual void damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height);
    virtual void MApplicationConstructor(MApplicationPrivate &dd, int &argc, char **argv, const QString &appIdentifier);
    virtual bool x11EventFilter(XEvent *event);
};

// 2. IMPLEMENT STUB
void MApplicationStub::MApplicationConstructor(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    Q_UNUSED(appIdentifier);
    Q_UNUSED(service);

}
void MApplicationStub::MApplicationConstructor(int &argc, char **argv, MApplicationService *service)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    Q_UNUSED(service);

}
void MApplicationStub::MApplicationDestructor()
{

}
MApplication *MApplicationStub::instance()
{
    stubMethodEntered("instance");
    return stubReturnValue<MApplication *>("instance");
}

QString MApplicationStub::deviceName()
{
    stubMethodEntered("deviceName");
    return stubReturnValue<QString>("deviceName");
}

bool MApplicationStub::softwareRendering()
{
    stubMethodEntered("softwareRendering");
    return stubReturnValue<bool>("softwareRendering");
}

bool MApplicationStub::fullScreen()
{
    stubMethodEntered("fullScreen");
    return stubReturnValue<bool>("fullScreen");
}

bool MApplicationStub::showBoundingRect()
{
    stubMethodEntered("showBoundingRect");
    return stubReturnValue<bool>("showBoundingRect");
}

bool MApplicationStub::showFps()
{
    stubMethodEntered("showFps");
    return stubReturnValue<bool>("showFps");
}

bool MApplicationStub::showSize()
{
    stubMethodEntered("showSize");
    return stubReturnValue<bool>("showSize");
}

bool MApplicationStub::showPosition()
{
    stubMethodEntered("showPosition");
    return stubReturnValue<bool>("showPosition");
}

bool MApplicationStub::showMargins()
{
    stubMethodEntered("showMargins");
    return stubReturnValue<bool>("showMargins");
}

bool MApplicationStub::showObjectNames()
{
    stubMethodEntered("showObjectNames");
    return stubReturnValue<bool>("showObjectNames");
}

bool MApplicationStub::showCursor()
{
    stubMethodEntered("showCursor");
    return stubReturnValue<bool>("showCursor");
}

void MApplicationStub::setShowPosition(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowPosition", params);
}

void MApplicationStub::setShowMargins(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowMargins", params);
}

void MApplicationStub::setShowObjectNames(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowObjectNames", params);
}

void MApplicationStub::setShowSize(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowSize", params);
}

void MApplicationStub::setShowBoundingRect(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowBoundingRect", params);
}

void MApplicationStub::setShowFps(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowFps", params);
}

void MApplicationStub::setShowCursor(bool show)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(show));
    stubMethodEntered("setShowCursor", params);
}

MApplicationWindow *MApplicationStub::activeApplicationWindow()
{
    stubMethodEntered("activeApplicationWindow");
    return stubReturnValue<MApplicationWindow *>("activeApplicationWindow");
}

MWindow *MApplicationStub::activeWindow()
{
    stubMethodEntered("activeWindow");
    return stubReturnValue<MWindow *>("activeWindow");
}

QList<MWindow *> MApplicationStub::windows()
{
    stubMethodEntered("windows");
    return stubReturnValue<QList<MWindow *> >("windows");
}

QString MApplicationStub::appName()
{
    stubMethodEntered("appName");
    return stubReturnValue<QString>("appName");
}

QString MApplicationStub::binaryName()
{
    stubMethodEntered("binaryName");
    return stubReturnValue<QString>("binaryName");
}

MFeedbackPlayer *MApplicationStub::feedbackPlayer()
{
    stubMethodEntered("feedbackPlayer");
    return stubReturnValue<MFeedbackPlayer *>("feedbackPlayer");
}

bool MApplicationStub::isLoadMInputContextEnabled()
{
    stubMethodEntered("isLoadMInputContextEnabled");
    return stubReturnValue<bool>("isLoadMInputContextEnabled");
}

void MApplicationStub::setLoadMInputContext(bool enable)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(enable));
    stubMethodEntered("setLoadMInputContext", params);
}

void MApplicationStub::fastFloatMath(bool val)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(val));
    stubMethodEntered("fastFloatMath", params);
}

void MApplicationStub::setPrestartMode(M::PrestartMode mode)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<M::PrestartMode >(mode));
    stubMethodEntered("setPrestartMode", params);
}

M::PrestartMode MApplicationStub::prestartMode()
{
    stubMethodEntered("prestartMode");
    return stubReturnValue<M::PrestartMode>("prestartMode");
}

bool MApplicationStub::isPrestarted()
{
    stubMethodEntered("isPrestarted");
    return stubReturnValue<bool>("isPrestarted");
}

void MApplicationStub::restorePrestart()
{
    stubMethodEntered("restorePrestart");
}

void MApplicationStub::releasePrestart()
{
    stubMethodEntered("releasePrestart");
}

void MApplicationStub::damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::HANDLE & >(damage));
    params.append(new Parameter<short & >(x));
    params.append(new Parameter<short & >(y));
    params.append(new Parameter<unsigned short & >(width));
    params.append(new Parameter<unsigned short & >(height));
    stubMethodEntered("damageEvent", params);
}

void MApplicationStub::MApplicationConstructor(MApplicationPrivate &dd, int &argc, char **argv, const QString &appIdentifier)
{
    Q_UNUSED(dd);
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    Q_UNUSED(appIdentifier);

}
bool MApplicationStub::x11EventFilter(XEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<XEvent * >(event));
    stubMethodEntered("x11EventFilter", params);
    return stubReturnValue<bool>("x11EventFilter");
}



// 3. CREATE A STUB INSTANCE
MApplicationStub gDefaultMApplicationStub;
MApplicationStub *gMApplicationStub = &gDefaultMApplicationStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MApplication::MApplication(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service) :
    QApplication(argc, argv),
    d_ptr(NULL)
{
    gMApplicationStub->MApplicationConstructor(argc, argv, appIdentifier, service);
}

MApplication::MApplication(int &argc, char **argv, MApplicationService *service) :
    QApplication(argc, argv),
    d_ptr(NULL)
{
    gMApplicationStub->MApplicationConstructor(argc, argv, service);
}

MApplication::~MApplication()
{
    gMApplicationStub->MApplicationDestructor();
}

MApplication *MApplication::instance()
{
    return gMApplicationStub->instance();
}

QString MApplication::deviceName()
{
    return gMApplicationStub->deviceName();
}

bool MApplication::softwareRendering()
{
    return gMApplicationStub->softwareRendering();
}

bool MApplication::fullScreen()
{
    return gMApplicationStub->fullScreen();
}

bool MApplication::showBoundingRect()
{
    return gMApplicationStub->showBoundingRect();
}

bool MApplication::showFps()
{
    return gMApplicationStub->showFps();
}

bool MApplication::showSize()
{
    return gMApplicationStub->showSize();
}

bool MApplication::showPosition()
{
    return gMApplicationStub->showPosition();
}

bool MApplication::showMargins()
{
    return gMApplicationStub->showMargins();
}

bool MApplication::showObjectNames()
{
    return gMApplicationStub->showObjectNames();
}

bool MApplication::showCursor()
{
    return gMApplicationStub->showCursor();
}

void MApplication::setShowPosition(bool show)
{
    gMApplicationStub->setShowPosition(show);
}

void MApplication::setShowMargins(bool show)
{
    gMApplicationStub->setShowMargins(show);
}

void MApplication::setShowObjectNames(bool show)
{
    gMApplicationStub->setShowObjectNames(show);
}

void MApplication::setShowSize(bool show)
{
    gMApplicationStub->setShowSize(show);
}

void MApplication::setShowBoundingRect(bool show)
{
    gMApplicationStub->setShowBoundingRect(show);
}

void MApplication::setShowFps(bool show)
{
    gMApplicationStub->setShowFps(show);
}

void MApplication::setShowCursor(bool show)
{
    gMApplicationStub->setShowCursor(show);
}

MApplicationWindow *MApplication::activeApplicationWindow()
{
    return gMApplicationStub->activeApplicationWindow();
}

MWindow *MApplication::activeWindow()
{
    return gMApplicationStub->activeWindow();
}

QList<MWindow *> MApplication::windows()
{
    return gMApplicationStub->windows();
}

QString MApplication::appName()
{
    return gMApplicationStub->appName();
}

QString MApplication::binaryName()
{
    return gMApplicationStub->binaryName();
}

MFeedbackPlayer *MApplication::feedbackPlayer()
{
    return gMApplicationStub->feedbackPlayer();
}

bool MApplication::isLoadMInputContextEnabled()
{
    return gMApplicationStub->isLoadMInputContextEnabled();
}

void MApplication::setLoadMInputContext(bool enable)
{
    gMApplicationStub->setLoadMInputContext(enable);
}

#ifdef __arm__
void MApplication::fastFloatMath(bool val)
{
    gMApplicationStub->fastFloatMath(val);
}
#endif

void MApplication::setPrestartMode(M::PrestartMode mode)
{
    gMApplicationStub->setPrestartMode(mode);
}

M::PrestartMode MApplication::prestartMode()
{
    return gMApplicationStub->prestartMode();
}

bool MApplication::isPrestarted()
{
    return gMApplicationStub->isPrestarted();
}

void MApplication::restorePrestart()
{
    gMApplicationStub->restorePrestart();
}

void MApplication::releasePrestart()
{
    gMApplicationStub->releasePrestart();
}

void MApplication::damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height)
{
    gMApplicationStub->damageEvent(damage, x, y, width, height);
}

MApplication::MApplication(MApplicationPrivate &dd, int &argc, char **argv, const QString &appIdentifier) :
    QApplication(argc, argv),
    d_ptr(NULL)
{
    gMApplicationStub->MApplicationConstructor(dd, argc, argv, appIdentifier);
}

bool MApplication::x11EventFilter(XEvent *event)
{
    return gMApplicationStub->x11EventFilter(event);
}


#endif
