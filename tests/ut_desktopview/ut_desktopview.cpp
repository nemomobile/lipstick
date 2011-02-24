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
#include <MApplication>
#include <MApplicationWindow>
#include <QPaintEngine>
#include <QPaintDevice>
#include <MButton>
#include <MSceneManager>
#include <MScene>
#include <MDeviceProfile>
#include <MPannableViewport>
#include <MSceneWindow>
#include <MApplicationExtensionArea>

#include "ut_desktopview.h"
#include "desktopview.h"
#include "desktop.h"
#include "switcher_stub.h"
#include "quicklaunchbar_stub.h"
#include "launcher_stub.h"
#include "launcherdatastore_stub.h"
#include "mainwindow_stub.h"
#include "mwindow_stub.h"
#include "mscenemanager_stub.h"
#include "applicationpackagemonitor_stub.h"
#include "applicationpackagemonitorlistener_stub.h"
#include "homewindowmonitor_stub.h"
#include "desktop_stub.h"
#include <QDBusConnection>
#include "x11wrapper.h"

#define ATOM_TYPE_NORMAL 1
#define ATOM_TYPE_NOTIFICATION 2
#define ATOM_TYPE_MENU 3
#define ATOM_TYPE_DIALOG 4

// X11Wrapper Stubs
int X11Wrapper::XSelectInput(Display *, Window , long)
{
    return 0;
}

Status X11Wrapper::XGetWindowAttributes(Display *, Window, XWindowAttributes *)
{
    return 0;
}

Atom X11Wrapper::XInternAtom(Display *, const char *atom_name, Bool)
{
#define ATOM_TYPE_NORMAL 1
#define ATOM_TYPE_NOTIFICATION 2
#define ATOM_TYPE_MENU 3
#define ATOM_TYPE_DIALOG 4
#define ATOM_TYPE_DESKTOP 5
#define ATOM_TYPE_DEFAULT 6

    if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_NORMAL") == 0) {
        return ATOM_TYPE_NORMAL;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_NOTIFICATION") == 0) {
        return ATOM_TYPE_NOTIFICATION;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_MENU") == 0) {
        return ATOM_TYPE_MENU;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_DIALOG") == 0) {
        return ATOM_TYPE_DIALOG;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_DESKTOP") == 0) {
        return ATOM_TYPE_DESKTOP;
    } else {
        return ATOM_TYPE_DEFAULT;
    }
}

int X11Wrapper::XGetWindowProperty(Display *dpy, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return)
{
    Q_UNUSED(dpy);
    Q_UNUSED(property);
    Q_UNUSED(long_offset);
    Q_UNUSED(long_length);
    Q_UNUSED(del);
    Q_UNUSED(req_type);
    Q_UNUSED(actual_type_return);
    Q_UNUSED(actual_format_return);
    Q_UNUSED(bytes_after_return);

    if (w == ATOM_TYPE_NORMAL) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_NORMAL;
        return Success;
    } else if (w == ATOM_TYPE_NOTIFICATION) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_NOTIFICATION;
        return Success;
    } else if (w == ATOM_TYPE_MENU) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_MENU;
        return Success;
    } else if (w == ATOM_TYPE_DIALOG) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_DIALOG;
        return Success;
    } else if (w == ATOM_TYPE_DEFAULT) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_DEFAULT;
        return Success;
    } else {
        return BadAtom;
    }
}

int X11Wrapper::XFree(void *)
{
    return 0;
}

Status X11Wrapper::XGetWMName(Display *, Window, XTextProperty *)
{
    return -1;
}

Status X11Wrapper::XGetTextProperty(Display *, Window , XTextProperty *text_prop_return, Atom)
{
    QString textValue("Title");
    std::string::size_type strSize = textValue.toStdString().length();
    text_prop_return->value = new unsigned char[strSize + 1];
    strncpy((char *)text_prop_return->value, textValue.toStdString().c_str(), strSize + 1);
    return 1;
}

XWMHints *X11Wrapper::XGetWMHints(Display *, Window)
{
    return 0;
}

int X11Wrapper::XFreePixmap(Display *, Pixmap)
{
    return 0;
}

Pixmap X11Wrapper::XCompositeNameWindowPixmap(Display *, Window)
{
    return 0;
}

Damage X11Wrapper::XDamageCreate(Display *, Drawable, int)
{
    return 0;
}

void X11Wrapper::XDamageDestroy(Display *, Damage)
{

}

int X11Wrapper::XSync(Display *, Bool)
{
    return 0;
}

XErrorHandler X11Wrapper::XSetErrorHandler(XErrorHandler)
{
    return 0;
}

int X11Wrapper::XChangeProperty(Display *, Window, Atom, Atom, int, int, unsigned char *, int)
{
    return 0;
}

Status X11Wrapper::XSendEvent(Display *, Window, Bool, long, XEvent *)
{
    return 0;
}

Status X11Wrapper::XGetTransientForHint(Display *, Window, Window *)
{
    return 0;
}

WId QWidget::winId() const
{
    return 0xfeedbeef;
}

// QWidget stubs
bool windowActivated = false;
bool windowRaised = false;

void QWidget::activateWindow()
{
    windowActivated = true;
}

void QWidget::raise()
{
     windowRaised = true;
}

// QGraphicsItem stubs
void QGraphicsItem::setZValue(qreal)
{
}

static bool gQGraphicsItemIsVisible = true;
bool QGraphicsItem::isVisible() const
{
    if (dynamic_cast<const Launcher*>(this) || dynamic_cast<const MSceneWindow*>(this)) {
        return gQGraphicsItemIsVisible;
    }
    return false;
}

void MWidget::setVisible(bool visible)
{
    if (dynamic_cast<const Launcher*>(this)) {
        gQGraphicsItemIsVisible = visible;
    }
}

// QDBusConnection stubs
bool QDBusConnection::registerObject(QString const &, QObject *, QFlags<QDBusConnection::RegisterOption>)
{
    return true;
}

// QDir stubs
QStringList qDirExistsDirs;
bool QDir::exists(const QString &name) const
{
    return qDirExistsDirs.contains(name);
}

QStringList qDirMkPathDirs;
bool QDir::mkpath(const QString &dirPath) const
{
    qDirMkPathDirs.append(dirPath);
    return true;
}

// MTheme stubs
void MTheme::releasePixmap(const QPixmap *)
{
}

TestDesktopBackgroundExtension::TestDesktopBackgroundExtension()
{
}

TestDesktopBackgroundExtension::~TestDesktopBackgroundExtension()
{
}

bool TestDesktopBackgroundExtension::initialize(const QString &)
{
    return true;
}

void TestDesktopBackgroundExtension::setDesktopInterface(MDesktopInterface &)
{
}

bool extensionDefocused;
void TestDesktopBackgroundExtension::setDefocused(bool defocused)
{
    extensionDefocused = defocused;
}

void TestDesktopBackgroundExtension::drawBackground(QPainter *, const QRectF &boundingRect) const
{
    this->boundingRect = boundingRect;
}

// Test desktop view implementation
TestDesktopView::TestDesktopView(Desktop *desktop) : DesktopView(desktop)
{
}

// Tests
MainWindow *Ut_DesktopView::mainWindow;

void Ut_DesktopView::initTestCase()
{
    // MApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_desktopview";
    app = new MApplication(argc, &app_name);
    mainWindow = MainWindow::instance(true);

    backgroundImage = new QPixmap(50, 50);
    backgroundTopImage = new QPixmap(25, 25);
    backgroundBottomImage = new QPixmap(35, 35);
}

void Ut_DesktopView::cleanupTestCase()
{
    delete mainWindow;
    // Destroy the MApplication
    delete app;
}

void Ut_DesktopView::init()
{
    homeWindowMonitor = new HomeWindowMonitor();
    gHomeWindowMonitorStub->stubSetReturnValue("instance", homeWindowMonitor);
    desktop = new Desktop;
    desktopView = new TestDesktopView(desktop);
    desktop->setView(desktopView);
    desktopView->modifiableStyle()->setDesktopBackgroundImage(backgroundImage);
    connect(this, SIGNAL(windowListUpdated(const QList<WindowInfo> &)),
            desktopView, SLOT(setSwitcherHasContent(const QList<WindowInfo> &)));
    // For QWidget activateWindow() and raise() stubs
    windowRaised = false;
    windowActivated = false;
    gQGraphicsItemIsVisible = false;
    extensionDefocused = false;

    gMSceneManagerStub->stubReset();
    gLauncherStub->stubReset();
}

void Ut_DesktopView::cleanup()
{
    delete desktop;
    qDirExistsDirs.clear();
    qDirMkPathDirs.clear();
    gLauncherDataStoreStub->stubReset();
    gHomeWindowMonitorStub->stubReset();
    delete homeWindowMonitor;
    homeWindowMonitor = NULL;
}

void Ut_DesktopView::testToggleLauncher()
{
    gQGraphicsItemIsVisible = false;
    desktopView->toggleLauncher();
    QCOMPARE(gLauncherStub->stubLastCallTo("setPage").parameter<uint>(0), (uint)0);
    verifyAppearDisappear(desktopView->launcherWindow, desktopView->switcherWindow);

    gQGraphicsItemIsVisible = true;
    desktopView->toggleLauncher();
    verifyAppearDisappear(desktopView->switcherWindow, desktopView->launcherWindow);
}

void Ut_DesktopView::testBoundingRectAndDrawBackground()
{
    // Add two extensions to the registered list of extensions
    TestDesktopBackgroundExtension extension1;
    TestDesktopBackgroundExtension extension2;
    desktopView->addExtension(&extension1);
    desktopView->addExtension(&extension2);

    // Check that the drawBackground function of all extensions is called
    QPainter painter;
    QRectF br = desktopView->boundingRect();
    desktopView->drawBackground(&painter, NULL);
    QCOMPARE(extension1.boundingRect, br);
    QCOMPARE(extension2.boundingRect, br);
}

void Ut_DesktopView::verifyAppearDisappear(MSceneWindow *appear, MSceneWindow *disappear)
{
    QCOMPARE(gMSceneManagerStub->stubCallCount("appearSceneWindow"), 1);
    QCOMPARE(gMSceneManagerStub->stubCallCount("disappearSceneWindow"), 1);
    QCOMPARE(gMSceneManagerStub->stubLastCallTo("appearSceneWindow").parameter<MSceneWindow *>(0), appear);
    QCOMPARE(gMSceneManagerStub->stubLastCallTo("disappearSceneWindow").parameter<MSceneWindow *>(0), disappear);
    gMSceneManagerStub->stubReset();
}

void Ut_DesktopView::testWhenFullscreenWindowAppearsLauncherGetsHidden()
{
    desktopView->showLauncher();
    gMSceneManagerStub->stubReset();
    gQGraphicsItemIsVisible = true;

    desktopView->hideLauncher();

    verifyAppearDisappear(desktopView->switcherWindow, desktopView->launcherWindow);
}

void Ut_DesktopView::testShowLauncherAndFocusToButtonWithCorrectDesktopFile()
{
    gLauncherStub->stubSetReturnValue("focusToButton", 1);

    desktopView->showLauncherAndFocusToButton("correctFileName");
    gQGraphicsItemIsVisible = true;

    QCOMPARE(windowActivated, true);
    QCOMPARE(windowRaised, true);

    verifyAppearDisappear(desktopView->launcherWindow, desktopView->switcherWindow);
}

void Ut_DesktopView::testShowLauncherAndFocusToButtonWithBadDesktopFile()
{
    gLauncherStub->stubSetReturnValue("focusToButton", -1);

    desktopView->launcher->setVisible(false);

    QCOMPARE(desktopView->launcherWindow->isVisible(), false);

    desktopView->showLauncherAndFocusToButton("badFileName");

    QCOMPARE(windowActivated, false);
    QCOMPARE(windowRaised, false);
    QCOMPARE(desktopView->launcherWindow->isVisible(), false);
}

void Ut_DesktopView::testShowLauncherAndFocusToButtonWithEmptyDesktopFile()
{
    gLauncherStub->stubSetReturnValue("focusToButton", -1);

    QCOMPARE(desktopView->launcherWindow->isVisible(), false);

    desktopView->showLauncherAndFocusToButton("");
    gQGraphicsItemIsVisible = true;

    QCOMPARE(windowActivated, true);
    QCOMPARE(windowRaised, true);

    QCOMPARE(1, gMSceneManagerStub->stubCallCount("appearSceneWindow"));
}

void Ut_DesktopView::testDefocusing()
{
    TestDesktopBackgroundExtension extension;
    desktopView->addExtension(&extension);

    gQGraphicsItemIsVisible = false;
    desktopView->showLauncher();
    QVERIFY(extensionDefocused);
    gQGraphicsItemIsVisible = true;

    desktopView->hideLauncher();
    QVERIFY(!extensionDefocused);
    gQGraphicsItemIsVisible = false;

    QList<WindowInfo> windowList;
    windowList.append(WindowInfo(0));
    emit windowListUpdated(windowList);
    QVERIFY(extensionDefocused);

    desktopView->showLauncher();
    QVERIFY(extensionDefocused);
    gQGraphicsItemIsVisible = true;

    windowList.clear();
    emit windowListUpdated(windowList);
    QVERIFY(extensionDefocused);

    desktopView->hideLauncher();
    QVERIFY(!extensionDefocused);
}

void Ut_DesktopView::testDataStoreInitialization_data()
{
    QTest::addColumn<QStringList>("existingDirectories");
    QTest::addColumn<QStringList>("createdDirectories");
    QTest::newRow("No directory exists") << QStringList() << (QStringList() << (QDir::homePath() + "/.config/meegotouchhome") << (QDir::homePath() + "/.local/share/applications"));
    QTest::newRow("Config exists") << (QStringList() << (QDir::homePath() + "/.config/meegotouchhome")) << (QStringList() << (QDir::homePath() + "/.local/share/applications"));
    QTest::newRow("Apps exists") << (QStringList() << (QDir::homePath() + "/.local/share/applications")) << (QStringList() << (QDir::homePath() + "/.config/meegotouchhome"));
    QTest::newRow("Both exist") << (QStringList() << (QDir::homePath() + "/.config/meegotouchhome") << (QDir::homePath() + "/.local/share/applications")) << QStringList();
}

void Ut_DesktopView::testDataStoreInitialization()
{
    QFETCH(QStringList, existingDirectories);
    QFETCH(QStringList, createdDirectories);

    cleanup();

    qDirExistsDirs = existingDirectories;

    init();

    // Existing directories should not be created, missing ones should
    foreach (const QString &directory, existingDirectories) {
        QCOMPARE(qDirMkPathDirs.contains(directory), QBool(false));
    }
    foreach (const QString &directory, createdDirectories) {
        QCOMPARE(qDirMkPathDirs.contains(directory), QBool(true));
    }

    QCOMPARE(gLauncherDataStoreStub->stubLastCallTo("LauncherDataStore").parameter<QStringList>(1), (QStringList() << APPLICATIONS_DIRECTORY << (QDir::homePath() + "/.local/share/applications/")));
}

void Ut_DesktopView::testConnectionsInConstructor()
{
    QVERIFY(disconnect(HomeWindowMonitor::instance(), SIGNAL(fullscreenWindowOnTopOfOwnWindow()), desktopView, SLOT(hideLauncher())));
}

QTEST_APPLESS_MAIN(Ut_DesktopView)
