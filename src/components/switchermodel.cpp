
/*
 * Copyright (c) 2011-2012, Robin Burchell
 * Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>
 * Copyright (c) 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QDir>
#include <QFileInfoList>
#include <QFileSystemWatcher>
#include <QRegExp>
#include <QX11Info>

#include "switchermodel.h"
#include "xtools/xatomcache.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

// Define this if you'd like to see debug messages from the switcher
#ifdef DEBUG_SWITCHER
#define SWITCHER_DEBUG(things) qDebug() << Q_FUNC_INFO << things
#else
#define SWITCHER_DEBUG(things)
#endif

SwitcherModel::SwitcherModel(QObject *parent)
    : QObjectListModel(parent)
{
}

SwitcherModel::~SwitcherModel()
{
}

bool SwitcherModel::handleXEvent(const XEvent &event)
{
    if (event.type == PropertyNotify &&
        event.xproperty.window == DefaultRootWindow(QX11Info::display()) &&
        event.xproperty.atom == AtomCache::atom("_NET_CLIENT_LIST"))
    {
        updateWindowList();
        return true;
    }
    else if (event.type == ClientMessage &&
             event.xclient.message_type == AtomCache::atom("_NET_CLOSE_WINDOW"))
    {
        SWITCHER_DEBUG("Got close WindowInfo event for " << event.xclient.window);
        WindowInfo *wi = WindowInfo::windowFor(event.xclient.window);
        delete wi;
        return true;
    }

    return false;
}

void SwitcherModel::updateWindowList()
{
    SWITCHER_DEBUG("Updating window list");

    Display *dpy = QX11Info::display();
    Atom actualType;
    int actualFormat;
    unsigned long numWindowItems, bytesLeft;
    unsigned char *windowData = NULL;

    int result = XGetWindowProperty(dpy,
                                    DefaultRootWindow(dpy),
                                    AtomCache::atom("_NET_CLIENT_LIST"),
                                    0, 0x7fffffff,
                                    false, XA_WINDOW,
                                    &actualType,
                                    &actualFormat,
                                    &numWindowItems,
                                    &bytesLeft,
                                    &windowData);

    if (result != Success || windowData == None)
        return;

    SWITCHER_DEBUG("Read list of " << numWindowItems << " windows");
    QSet<WindowInfo *> oldWindowSet;
    QSet<WindowInfo *> newWindowSet;

    QList<WindowInfo *> *temporaryList = getList<WindowInfo>();
    oldWindowSet.reserve(temporaryList->size());
    for (int i = 0; i < temporaryList->count(); ++i)
        oldWindowSet.insert(temporaryList->at(i));

    newWindowSet.reserve(numWindowItems);

    Window *wins = (Window *)windowData;

    for (unsigned int i = 0; i < numWindowItems; i++) {
        WindowInfo *wi = WindowInfo::windowFor(wins[i]);
        if (wi->visibleInSwitcher())
            newWindowSet.insert(wi);
    }

    XFree(windowData);

    QSet<WindowInfo *> closedWindows = oldWindowSet - newWindowSet;
    QSet<WindowInfo *> openedWindows = newWindowSet - oldWindowSet;

    foreach (WindowInfo *wi, closedWindows)
        delete wi;

    foreach (WindowInfo *wi, openedWindows)
        addItem(wi);

    emit itemCountChanged();
}
