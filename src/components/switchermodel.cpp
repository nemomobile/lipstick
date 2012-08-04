
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

#include <QtDeclarative/qdeclarative.h>
#include <QDir>
#include <QFileInfoList>
#include <QFileSystemWatcher>
#include <QRegExp>
#include <QX11Info>

#include "switchermodel.h"
#include "mainwindow.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

SwitcherModel::SwitcherModel(QObject *parent)
    : QObjectListModel<WindowInfo>(parent)
{
}

SwitcherModel::~SwitcherModel()
{
}

/*
static const char *appAtomNames[ATOM_COUNT] = {
    "_MEEGO_ORIENTATION",
    "_MEEGO_INHIBIT_SCREENSAVER",
    "_MEEGO_TABLET_NOTIFY",
    "_MEEGO_STACKING_LAYER",
    "_MEEGOTOUCH_SKIP_ANIMATIONS",
    "_NET_ACTIVE_WINDOW",
    "_NET_CLIENT_LIST",
    "_NET_CLOSE_WINDOW",
    "_NET_WM_WINDOW_TYPE",
    "_NET_WM_WINDOW_TYPE_NORMAL",
    "_NET_WM_WINDOW_TYPE_DESKTOP",
    "_NET_WM_WINDOW_TYPE_NOTIFICATION",
    "_NET_WM_WINDOW_TYPE_DOCK",
    "_NET_WM_STATE_SKIP_TASKBAR",
    "_NET_WM_STATE",
    "_NET_WM_PID",
    "_NET_WM_ICON_GEOMETRY",
    "_NET_WM_ICON_NAME",
    "Backlight",
    "BACKLIGHT",
    "UTF8_STRING",
    "WM_CHANGE_STATE",
    "_MEEGO_SYSTEM_DIALOG"
*/

bool SwitcherModel::handleXEvent(const XEvent &event)
{
    if (event.type == PropertyNotify &&
            event.xproperty.window == DefaultRootWindow(QX11Info::display()) &&
            event.xproperty.atom == WindowInfo::ClientListAtom)
    {
        updateWindowList();
        return true;
    }
    else if (event.type == ClientMessage &&
             event.xclient.message_type == WindowInfo::CloseWindowAtom)
    {
        qDebug() << Q_FUNC_INFO << "Got close WindowInfo event for " << event.xclient.window;
        if (!windowsBeingClosed.contains(event.xclient.window))
        {
            windowsBeingClosed.append(event.xclient.window);
        }
        updateWindowList();
        return true;
    }
    else if (event.type == PropertyNotify &&
             (event.xproperty.atom == WindowInfo::TypeAtom ||
              event.xproperty.atom == WindowInfo::StateAtom))
    {
        updateWindowList();
        return true;
    }
    else if (event.type == PropertyNotify &&
             event.xproperty.atom == WindowInfo::ActiveWindowAtom)
    {
        updateWindowList();
        return true;
    }

    return false;
}

static QVector<Atom> getNetWmState(Display *display, Window window)
{
    QVector<Atom> atomList;

    Atom actualType;
    int actualFormat;
    ulong propertyLength;
    ulong bytesLeft;
    uchar *propertyData = 0;

    // Step 1: Get the size of the list
    bool result = XGetWindowProperty(display, window, WindowInfo::StateAtom, 0, 0,
                                     false, XA_ATOM, &actualType,
                                     &actualFormat, &propertyLength,
                                     &bytesLeft, &propertyData);
    if (result == Success && actualType == XA_ATOM && actualFormat == 32)
    {
        atomList.resize(bytesLeft / sizeof(Atom));
        XFree(propertyData);

        // Step 2: Get the actual list
        if (XGetWindowProperty(display, window, WindowInfo::StateAtom, 0,
                               atomList.size(), false, XA_ATOM,
                               &actualType, &actualFormat,
                               &propertyLength, &bytesLeft,
                               &propertyData) == Success)
        {
            if (propertyLength != (ulong)atomList.size())
            {
                atomList.resize(propertyLength);
            }

            if (!atomList.isEmpty())
            {
                memcpy(atomList.data(), propertyData,
                       atomList.size() * sizeof(Atom));
            }
            XFree(propertyData);
        }
        else
        {
            qWarning("Unable to retrieve window properties: %i", (int) window);
            atomList.clear();
        }
    }

    return atomList;
}

void SwitcherModel::updateWindowList()
{
    qDebug() << Q_FUNC_INFO << "Updating window list";

    Display *dpy = QX11Info::display();
    XWindowAttributes wAttributes;
    Atom actualType;
    int actualFormat;
    unsigned long numWindowItems, bytesLeft;
    unsigned char *windowData = NULL;

    int result = XGetWindowProperty(dpy,
                                    DefaultRootWindow(dpy),
                                    WindowInfo::ClientListAtom,
                                    0, 0x7fffffff,
                                    false, XA_WINDOW,
                                    &actualType,
                                    &actualFormat,
                                    &numWindowItems,
                                    &bytesLeft,
                                    &windowData);

    if (result != Success || windowData == None)
        return;

    qDebug() << "Read list of " << numWindowItems << " windows";
    QList<WindowInfo *> windowList;
    Window *wins = (Window *)windowData;

    for (unsigned int i = 0; i < numWindowItems; i++)
    {
        if (XGetWindowAttributes(dpy, wins[i], &wAttributes) != 0
                && wAttributes.width > 0 &&
                wAttributes.height > 0 && wAttributes.c_class == InputOutput &&
                wAttributes.map_state != IsUnmapped)
        {
            unsigned char *typeData = NULL;
            unsigned long numTypeItems;

            // _NET_WM_PID
            unsigned char *propPid = 0;
            result = XGetWindowProperty(dpy,
                                        wins[i],
                                        WindowInfo::WindowPidAtom,
                                        0L, (long)BUFSIZ, false,
                                        XA_CARDINAL,
                                        &actualType,
                                        &actualFormat,
                                        &numTypeItems,
                                        &bytesLeft,
                                        &propPid);
            if (result == Success && propPid != 0)
            {
                unsigned long pid = 0;
                pid = *((unsigned long *)propPid);
                XFree(propPid);

                // TODO: use this to tie to .desktop entries in launcher
                Q_UNUSED(pid);
            }

            result = XGetWindowProperty(dpy,
                                        wins[i],
                                        WindowInfo::TypeAtom,
                                        0L, 16L, false,
                                        XA_ATOM,
                                        &actualType,
                                        &actualFormat,
                                        &numTypeItems,
                                        &bytesLeft,
                                        &typeData);
            if (result == Success)
            {
                Atom *type = (Atom *)typeData;

                bool includeInWindowList = false;

                // plain Xlib windows do not have a type
                if (numTypeItems == 0)
                    includeInWindowList = true;
                for (unsigned int n = 0; n < numTypeItems; n++)
                {
                    if (type[n] == WindowInfo::WindowTypeDesktopAtom ||
                            type[n] == WindowInfo::WindowTypeNotificationAtom ||
                            type[n] == WindowInfo::WindowTypeDockAtom ||
                            type[n] == WindowInfo::WindowTypeMenuAtom)
                    {
                        includeInWindowList = false;
                        break;
                    }
                    if (type[n] == WindowInfo::WindowTypeNormalAtom)
                    {
                        includeInWindowList = true;
                    }
                }

                if (includeInWindowList)
                {
                    if (getNetWmState(dpy, wins[i]).contains(WindowInfo::SkipTaskbarAtom))
                    {
                        includeInWindowList = false;
                    }
                }

                if (includeInWindowList)
                {
                    XTextProperty textProperty;
                    QString title;

                    if (XGetWMName(dpy, wins[i], &textProperty) != 0)
                    {
                        title = QString((const char *)(textProperty.value));
                        XFree(textProperty.value);
                    }

                    // Get window icon
                    XWMHints *wmhints = XGetWMHints(dpy, wins[i]);
                    if (wmhints != NULL)
                    {
                        Pixmap pixmap;
                        pixmap = wmhints->icon_pixmap;
                        XFree(wmhints);

                        // TODO: set pixmap
                        Q_UNUSED(pixmap);
                    }

                    if (!windowsBeingClosed.contains(wins[i]))
                    {
                        WindowInfo *wi = WindowInfo::windowFor(wins[i]);
                        windowList.append(wi);
                    }
                    else
                    {
                        windowsStillBeingClosed.append(wins[i]);
                    }
                }
                XFree(typeData);
            }
        }
    }

    XFree(wins);

    for (int i = windowsBeingClosed.count() - 1; i >= 0; i--)
    {
        Window window = windowsBeingClosed.at(i);
        if (!windowsStillBeingClosed.contains(window))
        {
            windowsBeingClosed.removeAt(i);
        }
    }

    qDebug() << Q_FUNC_INFO << "Deleting WindowInfos for " << windowsStillBeingClosed;
    foreach (Window wid, windowsStillBeingClosed) {
        WindowInfo *wi = WindowInfo::windowFor(wid);
        delete wi;
    }

    windowsStillBeingClosed.clear();

    setList(&windowList);
}

void SwitcherModel::windowToFront(qulonglong window)
{
    XEvent ev;
    memset(&ev, 0, sizeof(ev));
    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = WindowInfo::ActiveWindowAtom;
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = 1;
    ev.xclient.data.l[1]    = CurrentTime;
    XSendEvent(QX11Info::display(), QX11Info::appRootWindow(QX11Info::appScreen()), False, StructureNotifyMask, &ev);
    qDebug() << Q_FUNC_INFO << "Foregrounded " << window;
}

void SwitcherModel::closeWindow(qulonglong window)
{
    Window rootWin = QX11Info::appRootWindow(QX11Info::appScreen());

    XEvent ev;
    memset(&ev, 0, sizeof(ev));
    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = WindowInfo::CloseWindowAtom;
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = CurrentTime;
    ev.xclient.data.l[1]    = rootWin;
    XSendEvent(QX11Info::display(), rootWin, False, SubstructureRedirectMask, &ev);
    qDebug() << Q_FUNC_INFO << "Closed " << window;

    if (!windowsStillBeingClosed.contains(window))
        windowsStillBeingClosed.append(window);

    // Close also the window this one is transient for, if any
    WindowInfo *windowInfo = WindowInfo::windowFor(window);
    if (windowInfo->transientFor() != 0 && windowInfo->transientFor() != window) {
        qDebug() << Q_FUNC_INFO << "Closing transient " << windowInfo->transientFor();
        closeWindow(windowInfo->transientFor());
    } else {
        qDebug() << Q_FUNC_INFO << "Updating WindowInfo list, deleting " << windowsBeingClosed;
        updateWindowList();
    }
}
