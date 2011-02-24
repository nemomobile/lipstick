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

#include <QtTest/QtTest>
#include <MApplication>
#include "contentaction.h"
#include "ut_launcherbutton.h"
#include "launcherbutton.h"
#include "launcher_stub.h"
#include "homewindowmonitor_stub.h"
#include "x11wrapper.h"
#include "launcher.h"
#include "mdesktopentry_stub.h"
#include <QDBusMessage>
#include <QDBusAbstractInterface>

#include "launcherbuttonmodel.h"
Q_DECLARE_METATYPE(LauncherButtonModel::State);

#define ATOM_TYPE_NORMAL 1
#define ATOM_TYPE_NOTIFICATION 2
#define ATOM_TYPE_MENU 3
#define ATOM_TYPE_DIALOG 4
#define ATOM_TYPE_DESKTOP 5
#define ATOM_TYPE_DEFAULT 6

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
    } else if (w == ATOM_TYPE_DESKTOP) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_DESKTOP;
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
    return 0;
}

Status X11Wrapper::XGetTextProperty(Display *, Window , XTextProperty *, Atom)
{
    return 0;
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

using namespace ContentAction;

QString language;
QMap< QString, QSharedPointer<ActionPrivate> > contentActionPrivate;

// LauncherAction stubs (used by LauncherButton)
LauncherAction::LauncherAction()
    : Action()
{
}

LauncherAction::LauncherAction(const QString& desktopEntry)
    : Action(Action::defaultActionForFile(desktopEntry, ""))
{
}

bool operator!=(const LauncherAction &a, const LauncherAction &b)
{
    Q_UNUSED(a);
    Q_UNUSED(b);
    return true;
}

// ContentAction stubs (used by LauncherAction)
struct ContentAction::ActionPrivate
{
    ActionPrivate(bool isValid, QString name,
                  QString english, QString nonEnglish,
                  QString icon) :
        isValid_(isValid), name_(name),
        english_(english), nonEnglish_(nonEnglish),
        icon_(icon) {}
    virtual ~ActionPrivate() {}

    virtual bool isValid() const { return isValid_; }
    virtual QString name() const { return  name_; }
    virtual QString localizedName() const {
       if (language == "english") {
           return english_;
       } else {
           return nonEnglish_;
       }
    }
    virtual QString icon() const { return icon_; }

    bool isValid_;
    QString name_;
    QString english_;
    QString nonEnglish_;
    QString icon_;
};

bool Action::isValid() const { return d->isValid(); }
QString Action::name() const { return d->name(); }
QString Action::localizedName() const { return d->localizedName(); }
QString Action::icon() const { return d->icon(); }

Action::Action()
    : d(new ActionPrivate(false, "", "", "", ""))
{
}

Action::Action(const Action& other)
    : d(other.d)
{
}

Action::~Action()
{
}

Action Action::defaultActionForFile(const QUrl& fileUri, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    QString fileName = fileUri.toString();
    QSharedPointer<ActionPrivate> priv =
       contentActionPrivate.value(fileName);

    Action action;
    action.d = priv;

    return action;
}

int contentActionTriggerCalls = 0;
void Action::trigger() const
{
    contentActionTriggerCalls++;
}

// QDBusInterface stubs
QString g_qDBusInterfaceCall;
QDBusMessage QDBusAbstractInterface::call(const QString &method,
                  const QVariant &,
                  const QVariant &,
                  const QVariant &,
                  const QVariant &,
                  const QVariant &,
                  const QVariant &,
                  const QVariant &,
                  const QVariant &)
{
    g_qDBusInterfaceCall = method;
    return QDBusMessage();
}

// A helper function to set up a contentActionPrivate entry
void addActionPrivate(QString fileName, bool isValid, QString name,
                      QString english, QString nonEnglish, QString icon)
{
    ActionPrivate* priv = new ActionPrivate(isValid,
                                            name,
                                            english,
                                            nonEnglish,
                                            icon);
    contentActionPrivate.insert(fileName,
                                QSharedPointer<ActionPrivate>(priv));
}

void Ut_LauncherButton::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_launcherbutton" };
    app = new MApplication(argc, app_name);
    WindowInfo::initializeAtoms();
}

void Ut_LauncherButton::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherButton::init()
{
    homeWindowMonitor = new HomeWindowMonitor();
    gHomeWindowMonitorStub->stubSetReturnValue("instance", homeWindowMonitor);
    language = "english";
    contentActionPrivate.clear();
    contentActionTriggerCalls = 0;
    g_qDBusInterfaceCall.clear();

    m_subject = new LauncherButton("");
    connect(this, SIGNAL(clicked()), m_subject, SLOT(launch()));
    connect(this, SIGNAL(obscured()), HomeWindowMonitor::instance(), SIGNAL(fullscreenWindowOnTopOfOwnWindow()));
}

void Ut_LauncherButton::cleanup()
{
    m_subject->stopLaunchProgress();
    delete m_subject;
    gMDesktopEntryStub->stubReset();
    gHomeWindowMonitorStub->stubReset();
    delete homeWindowMonitor;
    homeWindowMonitor = NULL;
}

void Ut_LauncherButton::testInitialization()
{
    cleanup();

    // Set up the stubs
    gMDesktopEntryStub->stubSetReturnValue("fileName", QString("/dev/null"));
    gMDesktopEntryStub->stubSetReturnValue("name", QString("english"));
    addActionPrivate("/dev/null", true, "name", "english", "nonenglish", "icon");

    // Check whether the subject is populated from the desktop file
    m_subject = new LauncherButton("/dev/null");
    QCOMPARE(gMDesktopEntryStub->stubCallCount("MDesktopEntry"), 1);
    QCOMPARE(gMDesktopEntryStub->stubLastCallTo("MDesktopEntry").parameter<QString>(0), QString("/dev/null"));
    QCOMPARE(m_subject->desktopEntry(), QString("/dev/null"));
    QCOMPARE(m_subject->text(), QString("english"));
    QCOMPARE(m_subject->action.name(), QString("name"));
}

void Ut_LauncherButton::testWhenLauncherButtonIsClickedContentActionIsTriggered()
{
    emit clicked();
    QCOMPARE(contentActionTriggerCalls, 1);
}

void Ut_LauncherButton::testWhenLauncherButtonIsClickedInDownloadingStateContentActionIsNotTriggered()
{
    m_subject->model()->setButtonState(LauncherButtonModel::Downloading);
    emit clicked();
    QCOMPARE(contentActionTriggerCalls, 0);
}

void Ut_LauncherButton::testWhenLauncherButtonIsClickedInInstallingStateContentActionIsNotTriggered()
{
    m_subject->model()->setButtonState(LauncherButtonModel::Installing);
    emit clicked();
    QCOMPARE(contentActionTriggerCalls, 0);
}

void Ut_LauncherButton::testWhenLauncherButtonIsClickedInBrokenStateAndHasPackageErrorThenDBusCallIsMade()
{
    m_subject->model()->setButtonState(LauncherButtonModel::Broken);
    emit clicked();
    QCOMPARE(contentActionTriggerCalls, 0);

    gMDesktopEntryStub->stubSetReturnValue("value",QString("true"));
    m_subject->launch();

   QCOMPARE(g_qDBusInterfaceCall, QString("show_installation_exception"));
}

void Ut_LauncherButton::testStopLaunchProgressIfObscured()
{
    emit clicked();
    QCOMPARE(m_subject->buttonState(), LauncherButtonModel::Launching);

    emit obscured();
    QCOMPARE(m_subject->buttonState(), LauncherButtonModel::Installed);

    // Change the button state artificially so something else and see that another
    // obscured signal does nothing
    m_subject->model()->setButtonState(LauncherButtonModel::Downloading);
    emit obscured();
    QCOMPARE(m_subject->buttonState(), LauncherButtonModel::Downloading);
}

void Ut_LauncherButton::testLanguageChange()
{
    delete m_subject;

    // Set up the stubs
    gMDesktopEntryStub->stubSetReturnValue("fileName", QString("/dev/null"));
    gMDesktopEntryStub->stubSetReturnValue("name", QString("english"));

    // Check whether the subject is populated from the desktop file
    m_subject = new LauncherButton("/dev/null");
    gMDesktopEntryStub->stubSetReturnValue("name", QString("nonenglish"));
    m_subject->retranslateUi();
    QCOMPARE(m_subject->text(), QString("nonenglish"));
}

void Ut_LauncherButton::testSettingButtonStateAndProgress()
{
    //Default value for state is "installed" and 0 for progress
    QCOMPARE(m_subject->model()->buttonState(), LauncherButtonModel::Installed);
    QCOMPARE(m_subject->model()->operationProgress(), 0);

    int progress = 50;
    m_subject->setState(LauncherButtonModel::Downloading, progress);
    QCOMPARE(m_subject->model()->buttonState(), LauncherButtonModel::Downloading);
    QCOMPARE(m_subject->model()->operationProgress(), progress);

    //Only progress should change
    progress = 99;
    m_subject->setState(LauncherButtonModel::Downloading, progress);
    QCOMPARE(m_subject->model()->buttonState(), LauncherButtonModel::Downloading);
    QCOMPARE(m_subject->model()->operationProgress(), progress);
}

void Ut_LauncherButton::testSettingButtonStateAndProgressWithInvalidValues()
{
    //With invalid values progress shouldn't change from default value 0
    int progress = -1;
    m_subject->setState(LauncherButtonModel::Installing, progress);
    QCOMPARE(m_subject->model()->buttonState(), LauncherButtonModel::Installing);
    QCOMPARE(m_subject->model()->operationProgress(), 0);

    progress = 101;
    m_subject->setState(LauncherButtonModel::Downloading, progress);
    QCOMPARE(m_subject->model()->buttonState(), LauncherButtonModel::Downloading);
    QCOMPARE(m_subject->model()->operationProgress(), 0);
}

void Ut_LauncherButton::testLaunchingMultipleTimes()
{
    emit clicked();
    QCOMPARE(contentActionTriggerCalls, 1);

    emit obscured();

    emit clicked();
    QCOMPARE(contentActionTriggerCalls, 2);
}

void Ut_LauncherButton::testTryingToLaunchSecondActionWhileLaunching()
{
    QSharedPointer<LauncherButton> secondTestButton = QSharedPointer<LauncherButton>(new LauncherButton(""));

    emit clicked();
    QCOMPARE(contentActionTriggerCalls, 1);

    secondTestButton->launch();
    QCOMPARE(contentActionTriggerCalls, 1);
}

QTEST_APPLESS_MAIN(Ut_LauncherButton)
