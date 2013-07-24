
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
#include <QDBusConnection>

#include <sstream>
#include <fstream>

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
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/WindowModel", this, QDBusConnection::ExportAllSlots);
    dbus.registerService("org.nemomobile.lipstick");
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

static int handleXError(Display *dpy, XErrorEvent *e)
{
    char errorText[1024];
    XGetErrorText(dpy, e->error_code, errorText, sizeof(errorText));
    qWarning() << Q_FUNC_INFO << "Error from X while launching: " << errorText;
    return 0;
}

//! Activate a window.
static int clientMsg(Display *disp, Window win, const char *msg,
                     unsigned long data0, unsigned long data1,
                     unsigned long data2, unsigned long data3,
                     unsigned long data4)
{
    XEvent event;
    long mask = SubstructureRedirectMask | SubstructureNotifyMask;

    event.xclient.type = ClientMessage;
    event.xclient.serial = 0;
    event.xclient.send_event = True;
    event.xclient.message_type = XInternAtom(disp, msg, True);
    event.xclient.window = win;
    event.xclient.format = 32;
    event.xclient.data.l[0] = data0;
    event.xclient.data.l[1] = data1;
    event.xclient.data.l[2] = data2;
    event.xclient.data.l[3] = data3;
    event.xclient.data.l[4] = data4;

    // XInternAtom will return None if atom does not exists
    if (event.xclient.message_type && XSendEvent(disp, DefaultRootWindow(disp), False, mask, &event))
    {
        return EXIT_SUCCESS;
    }
    else
    {
        qWarning() << Q_FUNC_INFO << "Cannot send event: " << msg;
        return EXIT_FAILURE;
    }
}


/*!
 * \brief Return binary name assigned to a process id.
 *
 * Source for the binary name is /proc/[pid]/cmdline.
 *
 * \param pid The process id.
 * \return Full path to the command on success, empty string on failure.
 */
static std::string binaryNameForPid(int pid)
{
    std::string cmdLine;

    std::stringstream ss;
    ss << "/proc/" << pid << "/cmdline";

    std::ifstream procFile;
    procFile.open(ss.str().c_str());
    if (procFile.is_open())
    {
        procFile >> cmdLine;

        size_t nul = cmdLine.find_first_of('\0');
        if (nul != std::string::npos)
        {
            cmdLine = cmdLine.substr(0, nul);
        }
    }

    return cmdLine;
}

/*!
 * \brief Return pid assigned to a window id.
 *
 * \param dpy The X11 display.
 * \param window The window id.
 * \return Pid on success, -1 on failure.
 */
static int windowPid(Display * dpy, Window window)
{
    if (dpy)
    {
        static Atom    pidAtom = XInternAtom(dpy, "_NET_WM_PID", False);
        Atom           type;
        int            format;
        unsigned long  nItems;
        unsigned long  bytesAfter;
        unsigned char *propPID = 0;

        // Get the PID of the window
        if(XGetWindowProperty(dpy, window, pidAtom, 0, 1, False, XA_CARDINAL,
                              &type, &format, &nItems, &bytesAfter, &propPID) == Success)
        {
            if(propPID != 0)
            {
                // If the PID matches, add this window to the result set.
                int pid = *(reinterpret_cast<int *>(propPID));
                XFree(propPID);
                return pid;
            }
        }
    }

    return -1;
}

//! Raise given window of the given display
void raiseWindow(Display *dpy, Window window)
{
    clientMsg(dpy, window, "_NET_ACTIVE_WINDOW", 0, 0, 0, 0, 0);
    XSync(dpy, False);
}

/*!
 * \brief Return window id for the given binary.
 *
 * This method first fetches _NET_CLIENT_LIST for window candidates,
 * and then finds the matching binary using /proc/[pid]/cmdline.
 * Proc fs is primarily used, because we cannot trust WM_COMMAND window property
 * in all cases. Anyhow we check also WM_COMMAND because proc fs does not work
 * with scripts (python etc.).
 *
 * \param dpy The X11 display.
 * \param binaryName Full path to the binary.
 * \return Window id on success, 0 on failure.
 */
Window windowIdForBinary(Display *dpy, const char *binaryName)
{
    Window retValue = 0;
    if (dpy)
    {
        Atom           netClientListAtom = XInternAtom(dpy, "_NET_CLIENT_LIST", False);
        Atom           type;
        int            format;
        unsigned long  nItems = 0;
        unsigned long  bytesAfter;
        unsigned char *prop = 0;

        // Get the client list of the root window
        int status = XGetWindowProperty(dpy, XDefaultRootWindow(dpy), netClientListAtom,
                                        0, 0x7fffffff, False, XA_WINDOW,
                                        &type, &format, &nItems, &bytesAfter, &prop);

        if ((status == Success) && (prop != NULL))
        {
            Window * clients = reinterpret_cast<Window *>(prop);
            for (unsigned long i = 0; i < nItems; i++)
            {
                char **wmCommand = NULL;
                int  wmCommandCount = 0;

                if (binaryNameForPid(windowPid(dpy, clients[i])) == binaryName ||
                    (XGetCommand (dpy, clients[i], &wmCommand, &wmCommandCount) != 0 &&
                     wmCommandCount > 0 && strcmp(wmCommand[0], binaryName) == 0))
                {
                    retValue = clients[i];
                }

                if (wmCommand) {
                    XFreeStringList(wmCommand);
                }

                if (retValue)
                    break;
            }

            XFree(prop);
        }
    }

    return retValue;
}

void SwitcherModel::launchProcess(const QString &binaryName)
{
    XErrorHandler oldHandler = XSetErrorHandler(handleXError);
    Window winId = windowIdForBinary(QX11Info::display(), binaryName.toUtf8().constData());
    if (winId) {
        raiseWindow(QX11Info::display(), winId);
    } else {
        qWarning() << Q_FUNC_INFO << "Lock reserved but no window id for binary name found";
    }
    XSetErrorHandler(oldHandler);
}

