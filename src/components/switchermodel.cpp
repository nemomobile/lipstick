/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QtDeclarative/qdeclarative.h>
#include <QDir>
#include <QFileInfoList>
#include <QFileSystemWatcher>
#include <QRegExp>
#include <QX11Info>

#include <mdesktopentry.h>
#include "switchermodel.h"
#include "desktop.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

// TODO: generic atom cache would be nice
static Atom clientListAtom;
static Atom closeWindowAtom;
static Atom windowTypeAtom;
static Atom windowStateAtom;
static Atom activeWindowAtom;
static Atom skipTaskbarAtom;
static Atom windowTypeDesktopAtom;
static Atom windowTypeNotificationAtom;
static Atom windowTypeDockAtom;
static Atom windowTypeNormalAtom;
static Atom utf8StringAtom;
static Atom windowPidAtom;
static Atom iconGeometryAtom;

SwitcherModel::SwitcherModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    clientListAtom = XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST", False);
    closeWindowAtom = XInternAtom(QX11Info::display(), "_NET_CLOSE_WINDOW", False);
    windowTypeAtom = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False);
    windowStateAtom = XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    activeWindowAtom = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", False);
    skipTaskbarAtom = XInternAtom(QX11Info::display(), "_NET_WM_STATE_SKIP_TASKBAR", False);
    windowTypeDesktopAtom = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    windowTypeNotificationAtom = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_NOTIFICATION", False);
    windowTypeDockAtom = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DOCK", False);
    windowTypeNormalAtom = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_NORMAL", False);
    utf8StringAtom = XInternAtom(QX11Info::display(), "UTF8_STRING", False);
    windowPidAtom = XInternAtom(QX11Info::display(), "_NET_WM_PID", False);
    iconGeometryAtom = XInternAtom(QX11Info::display(), "_NET_WM_ICON_GEOMETRY", False);

    QHash<int, QByteArray> roles;
    roles[id]="pid";
    roles[name]="name";
    roles[exec]="exec";
    roles[comment]="comment";
    roles[icon]="icon";
    roles[filename]="filename";
    roles[nodisplay]="nodisplay";
    roles[object]="object";
    roles[windowId] = "windowId";

    setRoleNames(roles);
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
        event.xproperty.atom == clientListAtom)
    {
        updateWindowList();
        return true;
    }
    else if (event.type == ClientMessage &&
             event.xclient.message_type == closeWindowAtom)
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
             (event.xproperty.atom == windowTypeAtom ||
              event.xproperty.atom == windowStateAtom))
    {
        updateWindowList();
        return true;
    }
    else if (event.type == PropertyNotify &&
             event.xproperty.atom == activeWindowAtom)
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
    bool result = XGetWindowProperty(display, window, windowStateAtom, 0, 0,
                                     false, XA_ATOM, &actualType,
                                     &actualFormat, &propertyLength,
                                     &bytesLeft, &propertyData);
    if (result == Success && actualType == XA_ATOM && actualFormat == 32)
    {
        atomList.resize(bytesLeft / sizeof(Atom));
        XFree(propertyData);

        // Step 2: Get the actual list
        if (XGetWindowProperty(display, window, windowStateAtom, 0,
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
                                    clientListAtom,
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
                                        windowPidAtom,
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
                                        windowTypeAtom,
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
                    if (type[n] == windowTypeDesktopAtom ||
                        type[n] == windowTypeNotificationAtom ||
                        type[n] == windowTypeDockAtom)
                    {
                        includeInWindowList = false;
                        break;
                    }
                    if (type[n] == windowTypeNormalAtom)
                    {
                        includeInWindowList = true;
                    }
                }

                if (includeInWindowList)
                {
                    if (getNetWmState(dpy, wins[i]).contains(skipTaskbarAtom))
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
                        unsigned int geom[4];
                        geom[0] = 0; // x
                        geom[1] = 0; // y
                        geom[2] = 100; // width
                        geom[3] = 100; // height
                        XChangeProperty(QX11Info::display(),
                                        wins[i],
                                        iconGeometryAtom,
                                        XA_CARDINAL,
                                        sizeof(unsigned int) * 8,
                                        PropModeReplace,
                                        (unsigned char *)&geom, 4);

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

    // TODO: don't reset whole model
    beginResetModel();
    m_windows = windowList;
    endResetModel();

    emit itemCountChanged();
}

QModelIndex SwitcherModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex SwitcherModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int SwitcherModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_windows.count();
}

int SwitcherModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant SwitcherModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    WindowInfo *i = m_windows.at(index.row());

    switch (role) {
        case name:
            return i->title();
        case windowId: {
            qulonglong wid = i->window();
            return wid;
        }
        case object:
            return QVariant::fromValue<QObject *>(i);
        default:
            break;
    }

    return QVariant();
}

bool SwitcherModel::hasChildren ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent);
    return false;
}

void SwitcherModel::windowToFront(qulonglong window)
{
    XEvent ev;
    memset(&ev, 0, sizeof(ev));
    ev.xclient.type         = ClientMessage;
    ev.xclient.window       = window;
    ev.xclient.message_type = activeWindowAtom;
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
    ev.xclient.message_type = closeWindowAtom;
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

int SwitcherModel::itemCount()
{
    return rowCount();
}
