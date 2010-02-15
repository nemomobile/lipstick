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

#include "ut_duicompositornotificationsink.h"

#include <QtTest/QtTest>
#include <DuiRemoteAction>
#include <DuiInfoBanner>
#include <DuiApplication>
#include <DuiScene>
#include <DuiSceneManager>
#include "duicompositornotificationsink.h"
#include "notificationwidgetparameterfactory.h"
#include "x11wrapper.h"
#include "testnotificationparameters.h"
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include "notificationmanager.h"
#include "eventtypestore.h"
#include <QSettings>

#define ATOM_TYPE 0x00010000
#define ATOM_TYPE_NORMAL 0x00010001
#define ATOM_TYPE_DESKTOP 0x00010002
#define ATOM_CLIENT_LIST_STACKING 0x00020000
#define NUMBER_OF_WINDOWS 2

static QSettings *settings;

//EventTypeStore stubs
EventTypeStore::EventTypeStore(const QString &, uint)
{
}

const QSettings *EventTypeStore::settingsForEventType(QString const &) const
{
    return settings;
}

static EventTypeStore store("");
const EventTypeStore &NotificationManager::eventTypeStore() const
{
    return store;
}

// Mock notification manager (used by DuiCompositorNotificationSink)
MockNotificationManager::MockNotificationManager() :
    nextAvailableNotificationID(0)
{
}

uint MockNotificationManager::addNotification(uint, const NotificationParameters &parameters, uint groupId, bool, NotificationType type, int timeout)
{
    uint notificationId = nextAvailableNotificationID++;
    Notification notification = Notification(notificationId, groupId, 0, parameters, type, timeout);
    notifications.append(notification);
    emit notificationUpdated(notification);
    return notificationId;
}

uint MockNotificationManager::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId, bool persistent, NotificationType type)
{
    return addNotification(notificationUserId, parameters, groupId, persistent, type, 1000);
}

bool MockNotificationManager::updateNotification(uint, uint notificationId, const NotificationParameters &parameters)
{
    for (int i = 0; i < notifications.count(); ++i) {
        if (notifications.at(i).notificationId() == notificationId) {
            Notification oldNotification = notifications.at(i);
            Notification newNotification = Notification(notificationId, oldNotification.groupId(), oldNotification.userId(), parameters, oldNotification.type(), 1000);
            notifications[i] = newNotification;
            emit notificationUpdated(newNotification);
            return true;
        }
    }
    return false;
}

bool MockNotificationManager::removeNotification(uint, uint notificationId)
{
    bool removed = false;
    for (int i = 0; i < notifications.count(); ++i) {
        if (notifications.at(i).notificationId() == notificationId) {
            notifications.removeAt(i);
            removed = true;
            break;
        }
    }

    if (removed) {
        emit notificationRemoved(notificationId);
    }

    return removed;
}

uint MockNotificationManager::addGroup(uint, const NotificationParameters &, bool)
{
    return 0;
}

bool MockNotificationManager::updateGroup(uint, uint, const NotificationParameters &)
{
    return false;
}

bool MockNotificationManager::removeGroup(uint, uint)
{
    return false;
}

uint MockNotificationManager::notificationUserId()
{
    return 0;
}

QList< uint > MockNotificationManager::notificationIdList(uint /*notificationUserId*/)
{
    QList<uint> tmp;
    return tmp;
}

// QGraphicsWidget stubs (used by DuiCompositorNotificationSink)
void QGraphicsWidget::addAction(QAction *action)
{
    Ut_DuiCompositorNotificationSink::actions[this].append(action);

    DuiRemoteAction *dra = dynamic_cast<DuiRemoteAction *>(action);
    if (dra != NULL) {
        Ut_DuiCompositorNotificationSink::contents.append(dra->toString());
    }
}

void QGraphicsWidget::removeAction(QAction *action)
{
    Ut_DuiCompositorNotificationSink::actions[this].removeAll(action);

    DuiRemoteAction *dra = dynamic_cast<DuiRemoteAction *>(action);
    if (dra != NULL) {
        Ut_DuiCompositorNotificationSink::contents.removeAll(dra->toString());
    }
}

QList<QAction *> QGraphicsWidget::actions() const
{
    return Ut_DuiCompositorNotificationSink::actions[this];
}

// QWidget stubs (used by DuiCompositorNotificationSink)
void QWidget::setVisible(bool)
{
}

void QWidget::setFixedSize(int w, int h)
{
    Ut_DuiCompositorNotificationSink::lastFixedWidth = w;
    Ut_DuiCompositorNotificationSink::lastFixedHeight = h;
}

// QTimer stubs (used by DuiCompositorNotificationSink)
void QTimer::start(int msec)
{
    Ut_DuiCompositorNotificationSink::lastTimeout = msec;

    if (msec <= 1) {
        emit timeout();
    }
}

// QGraphicsScene stubs (used by DuiCompositorNotificationSink)
void QGraphicsScene::addItem(QGraphicsItem *item)
{
    DuiInfoBanner *n = dynamic_cast<DuiInfoBanner *>(item);
    if (n != NULL) {
        Ut_DuiCompositorNotificationSink::types.append(n->bannerType());
        Ut_DuiCompositorNotificationSink::icons.append(n->model()->imageID());
        Ut_DuiCompositorNotificationSink::bodies.append(n->model()->bodyText());
        Ut_DuiCompositorNotificationSink::buttonIcons.append(n->model()->iconID());
        Ut_DuiCompositorNotificationSink::notifications.append(n);
    }
}

void QGraphicsScene::removeItem(QGraphicsItem *item)
{
    DuiInfoBanner *n = dynamic_cast<DuiInfoBanner *>(item);
    if (n != NULL) {
        int index = Ut_DuiCompositorNotificationSink::notifications.indexOf(n);

        Ut_DuiCompositorNotificationSink::types.removeAt(index);
        Ut_DuiCompositorNotificationSink::icons.removeAt(index);
        Ut_DuiCompositorNotificationSink::bodies.removeAt(index);
        Ut_DuiCompositorNotificationSink::contents.removeAt(index);
        Ut_DuiCompositorNotificationSink::actions[n].clear();
        Ut_DuiCompositorNotificationSink::buttonIcons.removeAt(index);
        Ut_DuiCompositorNotificationSink::notifications.removeAt(index);
    }
}

// QGraphicsView stubs (used by DuiCompositorNotificationSink)
void QGraphicsView::setTransform(const QTransform &matrix, bool)
{
    Ut_DuiCompositorNotificationSink::lastTransform = matrix;
}

// DuiSceneWindow stubs (used by DuiCompositorNotificationSink)
void DuiSceneWindow::disappear()
{
    emit windowHidden();
}

// DuiInfoBanner stubs (used by DuiCompositorNotificationSink)
void DuiInfoBanner::setImageID(const QString &image)
{
    int index = Ut_DuiCompositorNotificationSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_DuiCompositorNotificationSink::icons.replace(index, image);
    }
    model()->setImageID(image);
}

void DuiInfoBanner::setBodyText(const QString &body)
{
    int index = Ut_DuiCompositorNotificationSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_DuiCompositorNotificationSink::bodies.replace(index, body);
    }
    model()->setBodyText(body);
}

void DuiInfoBanner::setIconID(const QString &iconId)
{
    int index = Ut_DuiCompositorNotificationSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_DuiCompositorNotificationSink::buttonIcons.replace(index, iconId);
    }
    model()->setIconID(iconId);
}

// X11Wrapper stubs (used by DuiCompositorNotificationSink)
Atom X11Wrapper::XInternAtom(Display *, const char *atom_name, Bool)
{
    if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE") == 0) {
        return ATOM_TYPE;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_DESKTOP") == 0) {
        return ATOM_TYPE_DESKTOP;
    } if (strcmp(atom_name, "_NET_CLIENT_LIST_STACKING") == 0) {
        return ATOM_CLIENT_LIST_STACKING;
    }

    return 0;
}

int X11Wrapper::XGetWindowProperty(Display *display, Window w, Atom property, long, long, Bool, Atom, Atom *, int *, unsigned long *nitems_return, unsigned long *, unsigned char **prop_return)
{
    if (property == ATOM_CLIENT_LIST_STACKING) {
        if (w != DefaultRootWindow(display)) {
            return BadWindow;
        } else {
            *nitems_return = NUMBER_OF_WINDOWS;
            *prop_return = new unsigned char[NUMBER_OF_WINDOWS * sizeof(Window)];

            Window *windows = (Window *) * prop_return;
            for (int i = 0; i < NUMBER_OF_WINDOWS; i++)
                windows[i] = i + 1;
            return Success;
        }
    } else if (property == ATOM_TYPE) {
        Atom *atom;

        switch (w) {
        case 1:
            *nitems_return = 1;
            *prop_return = new unsigned char[sizeof(Atom)];
            atom = (Atom *) * prop_return;
            atom[0] = Ut_DuiCompositorNotificationSink::desktopIsOnTop ? ATOM_TYPE_NORMAL : ATOM_TYPE_DESKTOP;
            break;
        case 2:
            *nitems_return = 1;
            *prop_return = new unsigned char[sizeof(Atom)];
            atom = (Atom *) * prop_return;
            atom[0] = Ut_DuiCompositorNotificationSink::desktopIsOnTop ? ATOM_TYPE_DESKTOP : ATOM_TYPE_NORMAL;
            break;
        default:
            *nitems_return = 0;
            *prop_return = 0;
            break;
        }
        return Success;
    } else {
        return BadAtom;
    }
}

int X11Wrapper::XFree(void *data)
{
    if (data != NULL) {
        delete [](unsigned char *)data;
    }
    return 0;
}

void XSetWMProperties(Display *, Window, XTextProperty *, XTextProperty *, char **, int, XSizeHints *, XWMHints *, XClassHint *)
{
}

QList<DuiInfoBanner::BannerType> Ut_DuiCompositorNotificationSink::types;
QList<QString> Ut_DuiCompositorNotificationSink::icons;
QList<QString> Ut_DuiCompositorNotificationSink::bodies;
QList<QString> Ut_DuiCompositorNotificationSink::buttonIcons;
QList<QString> Ut_DuiCompositorNotificationSink::contents;
QHash<const QGraphicsWidget *, QList<QAction *> > Ut_DuiCompositorNotificationSink::actions;
QList<DuiInfoBanner *> Ut_DuiCompositorNotificationSink::notifications;
int Ut_DuiCompositorNotificationSink::lastTimeout;
bool Ut_DuiCompositorNotificationSink::desktopIsOnTop;
int Ut_DuiCompositorNotificationSink::lastFixedWidth;
int Ut_DuiCompositorNotificationSink::lastFixedHeight;
QTransform Ut_DuiCompositorNotificationSink::lastTransform;

// Tests
void Ut_DuiCompositorNotificationSink::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_duicompositornotificationsink";
    app = new DuiApplication(argc, &app_name);
    mainWindow = MainWindow::instance(true);
    settings = new QSettings();
    gHomeApplicationStub->stubSetReturnValue("mainWindow", mainWindow);
}

void Ut_DuiCompositorNotificationSink::cleanupTestCase()
{
    delete settings;
    delete mainWindow;
    delete app;
}

void Ut_DuiCompositorNotificationSink::init()
{
    notificationManager = new MockNotificationManager();
    sink = new DuiCompositorNotificationSink();
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), sink, SLOT(removeNotification(uint)));
    connect(notificationManager, SIGNAL(notificationUpdated(Notification)), sink, SLOT(addNotification(Notification)));
    connect(sink, SIGNAL(notificationRemovalRequested(uint)), notificationManager, SLOT(removeNotification(uint)));
    connect(this, SIGNAL(orientationChangeFinished(Dui::Orientation)), sink, SLOT(rotateInfoBanners(Dui::Orientation)));
    lastTimeout = -1;
    Ut_DuiCompositorNotificationSink::desktopIsOnTop = false;
    types.clear();
    icons.clear();
    bodies.clear();
    buttonIcons.clear();
    contents.clear();
    actions.clear();
    notifications.clear();
    lastFixedWidth = -1;
    lastFixedHeight = -1;
    lastTransform = QTransform();
}

void Ut_DuiCompositorNotificationSink::cleanup()
{
    delete sink;
    delete notificationManager;
}

void Ut_DuiCompositorNotificationSink::testAddNotification()
{
    // Create three notifications - two with a content link and one without
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    TestNotificationParameters parameters2("icon2", "body2", "buttonicon2", "");
    notificationManager->addNotification(0, parameters0, 0, NotificationManagerInterface::SystemEvent);
    // Check that notifications are not shown when desktop is on top
    desktopIsOnTop = true;
    notificationManager->addNotification(0, parameters1);
    desktopIsOnTop = false;
    notificationManager->addNotification(0, parameters2);

    // Check that two DuiInfoBanners were created
    QCOMPARE(notifications.count(), 2);

    // Make sure the view size and rotation was set correctly
    DuiInfoBanner infoBanner1(types[1]);
    QTransform transform1;
    transform1.rotate(mainWindow->orientationAngle());
    QCOMPARE(lastFixedWidth, (int)infoBanner1.preferredWidth());
    QCOMPARE(lastFixedHeight, (int)infoBanner1.preferredHeight());
    QCOMPARE(lastTransform, transform1);

    // Rotate the screen
    mainWindow->setOrientationAngle(Dui::Angle90);

    // Create a notification without a content link
    TestNotificationParameters parameters3("icon3", "body3", "buttonicon3", "");
    notificationManager->addNotification(0, parameters3);

    // Check that a DuiInfoBanner was created with the given parameters
    QCOMPARE(types.length(), 3);
    QCOMPARE(types[2], DuiInfoBanner::Event);
    QCOMPARE(icons.length(), 3);
    QCOMPARE(icons[2], QString("icon3"));
    QCOMPARE(bodies.length(), 3);
    QCOMPARE(bodies[2], QString("body3"));
    QCOMPARE(buttonIcons.length(), 3);
    QCOMPARE(buttonIcons[2], QString("buttonicon3"));
    QCOMPARE(contents.length(), 1);

    // Make sure the view size and rotation was set correctly
    DuiInfoBanner infoBanner2(types[2]);
    QTransform transform2;
    transform2.rotate(mainWindow->orientationAngle());
    QCOMPARE(lastFixedWidth, (int)infoBanner2.preferredHeight());
    QCOMPARE(lastFixedHeight, (int)infoBanner2.preferredWidth());
    QCOMPARE(lastTransform, transform2);
}

void Ut_DuiCompositorNotificationSink::testUpdateNotification()
{
    // Create two notifications
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    notificationManager->addNotification(0, parameters0);
    uint id = notificationManager->addNotification(0, parameters1);

    // Update the second notification
    TestNotificationParameters parametersX("iconX", "bodyX", "buttoniconX", "contentX X X X");
    notificationManager->updateNotification(0, id, parametersX);

    // Check that two DuiInfoBanners were created with the given parameters
    QCOMPARE(types.length(), 2);
    QCOMPARE(types[0], DuiInfoBanner::Event);
    QCOMPARE(types[1], DuiInfoBanner::Event);
    QCOMPARE(icons.length(), 2);
    QCOMPARE(icons[0], QString("icon0"));
    QCOMPARE(icons[1], QString("iconX"));
    QCOMPARE(bodies.length(), 2);
    QCOMPARE(bodies[0], QString("body0"));
    QCOMPARE(bodies[1], QString("bodyX"));
    QCOMPARE(buttonIcons.length(), 2);
    QCOMPARE(buttonIcons[0], QString("buttonicon0"));
    QCOMPARE(buttonIcons[1], QString("buttoniconX"));
    QCOMPARE(contents.length(), 2);
    QCOMPARE(contents[0], QString("content0 0 0 0"));
    QCOMPARE(contents[1], QString("contentX X X X"));
}

void Ut_DuiCompositorNotificationSink::testRemoveNotification()
{
    // Create three notifications
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    TestNotificationParameters parameters2("icon2", "body2", "buttonicon2", "");
    notificationManager->addNotification(0, parameters0, 0, NotificationManagerInterface::SystemEvent);
    uint id = notificationManager->addNotification(0, parameters1);
    notificationManager->addNotification(0, parameters2);

    // Remove the second one
    notificationManager->removeNotification(0, id);

    // Check that two DuiInfoBanners exist with the given icon parameters (to identify them)
    QCOMPARE(icons.length(), 2);
    QCOMPARE(icons[0], QString("icon0"));
    QCOMPARE(icons[1], QString("icon2"));

    // Recreate the second notification and create an additional one
    notificationManager->addNotification(0, parameters1);
    TestNotificationParameters parameters4("icon3", "body3", "buttonicon3", "");
    notificationManager->addNotification(0, parameters4, 0, NotificationManagerInterface::SystemEvent);
    QCOMPARE(icons.length(), 4);
    QCOMPARE(icons[0], QString("icon0"));
    QCOMPARE(icons[1], QString("icon2"));
    QCOMPARE(icons[2], QString("icon1"));
    QCOMPARE(icons[3], QString("icon3"));
}

void Ut_DuiCompositorNotificationSink::testTimeout()
{
    // Create a notification with a timeout of 1000 milliseconds
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0);

    // Check that the timeout was set
    QCOMPARE(lastTimeout, 1000);

    // The notification should be created
    QCOMPARE(notifications.length(), 1);

    // Create a notification with a timeout of 0 milliseconds
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1 1 1 1");
    notificationManager->addNotification(0, parameters1, 0, false, NotificationManagerInterface::ApplicationEvent, 0);

    // Check that the timeout was set
    QCOMPARE(lastTimeout, 0);

    // The notification should be immediately destroyed
    QCOMPARE(notifications.length(), 1);
}

void Ut_DuiCompositorNotificationSink::testOrientationChanged()
{
    // Make sure landscape is activated
    mainWindow->setOrientationAngle(Dui::Angle0);
    emit orientationChangeFinished(Dui::Landscape);

    // Create a notification
    TestNotificationParameters parameters("icon0", "body0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters);

    // The notification should be created
    QCOMPARE(types.length(), 1);
    QCOMPARE(icons.length(), 1);
    QCOMPARE(bodies.length(), 1);
    QCOMPARE(buttonIcons.length(), 1);

    // Make sure the view size and rotation was set correctly
    DuiInfoBanner infoBanner(types[0]);
    QTransform transform;
    transform.rotate(mainWindow->orientationAngle());
    QCOMPARE(lastFixedWidth, (int)infoBanner.preferredWidth());
    QCOMPARE(lastFixedHeight, (int)infoBanner.preferredHeight());
    QCOMPARE(lastTransform, transform);

    // Rotate the screen
    mainWindow->setOrientationAngle(Dui::Angle90);
    emit orientationChangeFinished(Dui::Portrait);

    // Make sure the view size and rotation was set correctly
    QTransform transform2;
    transform2.rotate(mainWindow->orientationAngle());
    QCOMPARE((int)infoBanner.preferredHeight(), lastFixedWidth);
    QCOMPARE((int)infoBanner.preferredWidth(), lastFixedHeight);

    QCOMPARE(lastTransform, transform2);
}

void Ut_DuiCompositorNotificationSink::testNotificationWhileApplicationEventsDisabled()
{
    // Create notification
    TestNotificationParameters parameters("icon0", "body0", "buttonicon0", "content0 0 0 0");
    sink->setApplicationEventsEnabled(true);
    notificationManager->addNotification(0, parameters);
    // Check that notification was added when application events enabled
    QCOMPARE(notifications.count(), 1);

    sink->setApplicationEventsEnabled(false);
    notificationManager->addNotification(0, parameters);
    // Check that notification was NOT added when application events are NOT enabled
    QCOMPARE(notifications.count(), 1);
}

QTEST_APPLESS_MAIN(Ut_DuiCompositorNotificationSink)
