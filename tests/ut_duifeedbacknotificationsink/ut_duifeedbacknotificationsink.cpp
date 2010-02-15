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


#include "ut_duifeedbacknotificationsink.h"
#include <DuiApplication>
#include <DuiFeedbackPlayer>
#include "duifeedbacknotificationsink.h"
#include "duifeedbackplayer_p.h"
#include "feedbackparameterfactory.h"
#include "eventtypestore_stub.h"
#include "notificationmanager_stub.h"
#include "homeapplication_stub.h"
#include "systemnotificationsource.h"

// xlib.h defines Status, undef it so we can use QSettings::Status
#ifdef Status
#undef Status
#endif

static NotificationManager *manager;

// DuiFeedbackPlayer stubs (used by DuiApplication)
void DuiFeedbackPlayer::play(const QString &feedbackName)
{
    Ut_DuiFeedbackNotificationSink::played.append(feedbackName);
}

bool DuiFeedbackPlayerPrivate::init(const QString &)
{
    return true;
}

// maemosec stubs
int maemosec::storage::get_file(const char *pathname, unsigned char **to_buf, ssize_t *bytes)
{
    Q_UNUSED(pathname);
    Q_UNUSED(to_buf);
    Q_UNUSED(bytes);
    return 0;
}

int maemosec::storage::put_file(const char *pathname, unsigned char *data, ssize_t bytes)
{
    Q_UNUSED(pathname);
    Q_UNUSED(data);
    Q_UNUSED(bytes);
    return 0;
}

void maemosec::storage::release_buffer(unsigned char *data)
{
    Q_UNUSED(data);
}

bool maemosec::storage::contains_file(const char *pathname)
{
    Q_UNUSED(pathname);
    return true;
}

void maemosec::storage::remove_file(const char *pathname)
{
    Q_UNUSED(pathname);
}

void maemosec::storage::commit()
{
}

maemosec::storage::storage(const char *name, maemosec::storage::visibility_t visibility, maemosec::storage::protection_t protection)
{
    Q_UNUSED(name);
    Q_UNUSED(visibility);
    Q_UNUSED(protection);
}

maemosec::storage::~storage()
{
}

QList<QString> Ut_DuiFeedbackNotificationSink::played;

void Ut_DuiFeedbackNotificationSink::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_duifeedbacknotificationsink";
    app = new DuiApplication(argc, &app_name);
    eventTypeSettings = new QSettings;
    gEventTypeStoreStub->stubSetReturnValue("settingsForEventType", const_cast<const QSettings *>(eventTypeSettings));
}

void Ut_DuiFeedbackNotificationSink::cleanupTestCase()
{
    delete app;
}

void Ut_DuiFeedbackNotificationSink::init()
{
    played.clear();
    sink = new DuiFeedbackNotificationSink();
    manager = new NotificationManager();
    connect(this, SIGNAL(addNotification(Notification)), sink, SLOT(addNotification(Notification)));
    connect(this, SIGNAL(removeNotification(uint)), sink, SLOT(removeNotification(uint)));
    eventTypeSettings->clear();
}

void Ut_DuiFeedbackNotificationSink::cleanup()
{
    delete sink;
    delete manager;
}

void Ut_DuiFeedbackNotificationSink::testAddNotification()
{
    // Create a notification
    NotificationParameters parameters;
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    emit addNotification(Notification(0, 0, 0, parameters, NotificationManagerInterface::ApplicationEvent, 1000));

    // Check that DuiFeedbackPlayer::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("feedback"));
}

void Ut_DuiFeedbackNotificationSink::testNotificationWhileApplicationEventsDisabled()
{
    // Create a notification
    NotificationParameters parameters;
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    sink->setApplicationEventsEnabled(true);
    emit addNotification(Notification(0, 0, 0, parameters, NotificationManagerInterface::ApplicationEvent, 1000));
    // Check that DuiFeedbackPlayer::play() was called for the feedback when application events enabled
    QCOMPARE(played.count(), 1);

    sink->setApplicationEventsEnabled(false);
    emit addNotification(Notification(0, 0, 0, parameters, NotificationManagerInterface::ApplicationEvent, 1000));
    // Check that DuiFeedbackPlayer::play() was NOT called for the feedback when application events are NOT enabled
    QCOMPARE(played.count(), 1);
}

void Ut_DuiFeedbackNotificationSink::testRemoveNotification()
{
    emit removeNotification(1);
}

void Ut_DuiFeedbackNotificationSink::testWithEventTypeAndFeedbackId()
{
    NotificationParameters parameters;
    parameters.add("eventType", "message-received");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    emit addNotification(Notification(0, 0, 0, parameters, NotificationManagerInterface::ApplicationEvent, 1000));

    // Check that DuiFeedbackPlayer::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("feedback"));
}

void Ut_DuiFeedbackNotificationSink::testWithEventTypeWithoutFeedbackId()
{
    eventTypeSettings->setValue(FeedbackParameterFactory::feedbackIdKey(), "eventTypeStoreFeedback");

    NotificationParameters parameters;
    parameters.add("eventType", "message-received");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter(""));
    emit addNotification(Notification(0, 0, 0, parameters, NotificationManagerInterface::ApplicationEvent, 1000));

    // Check that DuiFeedbackPlayer::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("eventTypeStoreFeedback"));
}

void Ut_DuiFeedbackNotificationSink::testWithoutEventTypeOrFeedbackId()
{
    NotificationParameters parameters;
    parameters.add("eventType", "");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter(""));
    emit addNotification(Notification(0, 0, 0, parameters, NotificationManagerInterface::ApplicationEvent, 1000));

    // Check that DuiFeedbackPlayer::play() was not called for the feedback
    QCOMPARE(played.count(), 0);
}

void Ut_DuiFeedbackNotificationSink::testWithoutEventTypeWithFeedbackId()
{
    NotificationParameters parameters;
    parameters.add("eventType", "");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    emit addNotification(Notification(0, 0, 0, parameters, NotificationManagerInterface::ApplicationEvent, 1000));

    // Check that DuiFeedbackPlayer::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("feedback"));
}

void Ut_DuiFeedbackNotificationSink::testDetermineFeedBackId()
{
    eventTypeSettings->setValue(FeedbackParameterFactory::feedbackIdKey(), "eventTypeStoreFeedback");

    NotificationParameters parameters;
    parameters.add("eventType", "message-received");
    emit addNotification(Notification(0, 0, 0,  parameters, NotificationManagerInterface::ApplicationEvent, 1000));

    // Check that DuiFeedbackPlayer::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("eventTypeStoreFeedback"));
}

QTEST_APPLESS_MAIN(Ut_DuiFeedbackNotificationSink)
