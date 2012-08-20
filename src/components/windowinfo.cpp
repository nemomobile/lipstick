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

#include <QHash>
#include <QDebug>
#include <QX11Info>

#include "windowinfo.h"
#include "xtools/xatomcache.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

class WindowInfo::WindowData
{

public:
    //! Constructs a window data object
    WindowData(Window id) :
            window(id),
            transientFor(0),
            title(),
            types(),
            states(),
            pid(0),
            pixmapSerial(0)
    {
    }

    //! Destructor
    ~WindowData()
    {
    }

    //! The X window ID
    Window window;

    //! The ID of the window this window is transient for
    Window transientFor;

    //! The title of the window
    QString title;

    //! The window types associated with this window
    QList<Atom> types;

    //! The status atoms of this window
    QList<Atom> states;

    int pid;

    int pixmapSerial;
};

//! Storage for the WindowInfo data objects. A central storage enables constructing
//! new WindowInfo objects with shared data.
static QHash<Window, WindowInfo * > windowDatas;

WindowInfo *WindowInfo::windowFor(Qt::HANDLE wid)
{
    if (WindowInfo *wi = windowDatas.value(wid)) {
        return wi;
    } else {
        return new WindowInfo(wid);
    }
}

WindowInfo::WindowInfo(Qt::HANDLE window)
    : d(new WindowData(window))
{
    qDebug() << Q_FUNC_INFO << "Created WindowInfo for " << window;
    updateWindowTitle();
    updateWindowProperties();
    windowDatas[window] = this;
}

WindowInfo::~WindowInfo()
{
    qDebug() << Q_FUNC_INFO << "Destroyed windwo for " << d->window;
    windowDatas.remove(d->window);
}

const QString& WindowInfo::title() const
{
    return d->title;
}

Qt::HANDLE WindowInfo::window() const
{
    return d->window;
}

Qt::HANDLE WindowInfo::transientFor() const
{
    return d->transientFor;
}

QList<Qt::HANDLE> WindowInfo::types() const
{
    return d->types;
}

QList<Qt::HANDLE> WindowInfo::states() const
{
    return d->states;
}

bool operator==(const WindowInfo &wi1, const WindowInfo &wi2)
{
    return wi1.window() == wi2.window();
}

bool WindowInfo::updateWindowTitle()
{
    Display *dpy = QX11Info::display();
    XTextProperty textProperty;
    bool updated = false;
    int result = XGetTextProperty(dpy, d->window, &textProperty, AtomCache::NameAtom);
    if (result == 0) {
        result = XGetWMName(dpy, d->window, &textProperty);
    }

    if (result != 0) {
        d->title = QString::fromUtf8((const char *)textProperty.value);
        XFree(textProperty.value);
        updated = true;
    }
    return updated;
}

void WindowInfo::updateWindowProperties()
{
    d->types = getWindowProperties(d->window, AtomCache::TypeAtom);
    d->states = getWindowProperties(d->window, AtomCache::StateAtom);

    if (!XGetTransientForHint(QX11Info::display(), d->window, &d->transientFor) || d->transientFor == d->window) {
        d->transientFor = 0;
    }
}

int WindowInfo::pid() const
{
    return d->pid;
}

void WindowInfo::setPid(int pid)
{
    d->pid = pid;
}

QList<Qt::HANDLE> WindowInfo::getWindowProperties(Qt::HANDLE winId, Qt::HANDLE propertyAtom, long maxCount)
{
    QList<Atom> properties;
    Atom actualType;
    int actualFormat;
    unsigned long numTypeItems, bytesLeft;
    unsigned char *typeData = NULL;

    Status result = XGetWindowProperty(QX11Info::display(), winId, propertyAtom, 0L, maxCount, False, XA_ATOM, &actualType, &actualFormat, &numTypeItems, &bytesLeft, &typeData);
    if (result == Success) {
        Atom *type = (Atom *) typeData;
        for (unsigned int n = 0; n < numTypeItems; n++) {
            properties.append(type[n]);
        }
        XFree(typeData);
    }
    return properties;
}

Qt::HANDLE WindowInfo::pixmapSerial() const
{
    return d->pixmapSerial;
}

void WindowInfo::setPixmapSerial(Qt::HANDLE pixmapSerial)
{
    d->pixmapSerial = pixmapSerial;
    qDebug() << Q_FUNC_INFO << "Changed pixmap serial on " << d->window << " to " << d->pixmapSerial;
    emit pixmapSerialChanged();
}
