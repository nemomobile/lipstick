/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <getopt.h>
#include "notificationmanager.h"
#include "notificationmanagerproxy.h"

// The operations for this tool
enum ToolOperation {
    Undefined,
    Add,
    Update,
    Remove
};

// The operation to perform
ToolOperation toolOperation = Undefined;

// The notification ID to use
uint id = 0;

// The icon of the notification
QString icon;

// The category of the notification
QString category;

// The count of items represented by the notification
int count = -1;

// Expire timeout for the notifcation
int expireTimeout = -1;

// Timestamp for notification
QString timestamp;

// Actions for the notification
QHash<QString, QStringList> actions;

// Prints usage information
int usage(const char *program)
{
    std::cerr << std::setw(7) << "Usage: " << program << " [OPTION]... [SUMMARY BODY PREVIEWSUMMARY PREVIEWBODY]" << std::endl;
    std::cerr << std::setw(7) << "Manage notifications." << std::endl;
    std::cerr << std::setw(7) << std::endl;
    std::cerr << std::setw(7) << "Mandatory arguments to long options are mandatory for short options too." << std::endl;
    std::cerr << std::setw(7) << "  -o, --operation=OPERATION  The operation (add/update/remove) to perform." << std::endl;
    std::cerr << std::setw(7) << "                             add - Adds a new notification." << std::endl;
    std::cerr << std::setw(7) << "                             update - Updates an existing notification." << std::endl;
    std::cerr << std::setw(7) << "                             remove - Removes an existing notification." << std::endl;
    std::cerr << std::setw(7) << "  -i, --id=ID                The notification ID to use when updating or removing a notification." << std::endl;
    std::cerr << std::setw(7) << "  -I, --icon=ICON            Icon for the notification."<< std::endl;
    std::cerr << std::setw(7) << "  -c, --category=CATEGORY    The category of the notification." << std::endl;
    std::cerr << std::setw(7) << "  -C, --count=NUMBER         The number of items represented by the notification." << std::endl;
    std::cerr << std::setw(7) << "  -t, --timestamp=TIMESTAMP  Timestamp to use on a notification. Use ISO 8601 extended date format."<< std::endl;
    std::cerr << std::setw(7) << "  -T, --timeout=MILLISECONDS Expire timeout for the notification in milliseconds or -1 to use server defaults."<< std::endl;
    std::cerr << std::setw(7) << "  -a, --action               An action for the notification in \"ACTIONNAME DBUSSERVICE DBUSPATH DBUSINTERFACE METHOD ARGUMENTS\" format."<< std::endl;
    std::cerr << std::setw(7) << "      --help                 display this help and exit" << std::endl;
    std::cerr << std::setw(7) << std::endl;
    std::cerr << std::setw(7) << "A notification ID is mandatory when the operation is 'update' or 'remove'." << std::endl;
    std::cerr << std::setw(7) << "The number of items and the timestamp are only used when the operation is 'add' or 'update'." << std::endl;
    return -1;
}

// Parses command line arguments (the flags)
int parseArguments(int argc, char *argv[])
{
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            { "operation", required_argument, NULL, 'o' },
            { "id", required_argument, NULL, 'i' },
            { "icon", required_argument, NULL, 'I' },
            { "category", required_argument, NULL, 'c' },
            { "count", required_argument, NULL, 'C' },
            { "timestamp", required_argument, NULL, 't'},
            { "timeout", required_argument, NULL, 'T' },
            { "action", required_argument, NULL, 'a'},
            { "help", no_argument, NULL, 'h' },
            { 0, 0, 0, 0 }
        };

        int c = getopt_long(argc, argv, "o:i:I:c:C:t:T:a:h", long_options, &option_index);
        if (c == -1) {
            break;
        }

        switch (c) {
        case 'o':
            if (strcmp(optarg, "add") == 0) {
                toolOperation = Add;
            } else if (strcmp(optarg, "update") == 0) {
                toolOperation = Update;
            } else if (strcmp(optarg, "remove") == 0) {
                toolOperation = Remove;
            }
            break;
        case 'i':
            id = atoi(optarg);
            break;
        case 'I':
            icon = QString(optarg);
            break;
        case 'c':
            category = QString(optarg);
            break;
        case 'C':
            count = atoi(optarg);
            break;
        case 'h':
            return usage(argv[0]);
            break;
        case 't':
            timestamp = QString(optarg);
            break;
        case 'T':
            expireTimeout = atoi(optarg);
            break;
        case 'a': {
            QStringList action = QString(optarg).split(' ');
            QString name = action.takeFirst();
            actions.insert(name, action);
            break;
            }
        default:
            break;
        }
    }

    if (toolOperation == Undefined ||
            (toolOperation == Add && argc < optind) ||
            (toolOperation == Add && id != 0) ||
            (toolOperation == Update && argc < optind) ||
            (toolOperation == Update && id == 0)) {
        return usage(argv[0]);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // Parse arguments
    int result = parseArguments(argc, argv);
    if (result != 0) {
        return result;
    }

    QCoreApplication application(argc, argv);
    qDBusRegisterMetaType<QVariantHash>();
    NotificationManagerProxy proxy("org.freedesktop.Notifications", "/org/freedesktop/Notifications", QDBusConnection::sessionBus());

    // Execute the desired operation
    switch (toolOperation) {
    case Add:
    case Update: {
        // Get the parameters for adding and updating notifications
        QString summary, body, previewSummary, previewBody;
        if (argc >= optind) {
            summary = QString(argv[optind]);
        }
        if (argc >= optind + 1) {
            body = QString(argv[optind + 1]);
        }
        if (argc >= optind + 2) {
            previewSummary = QString(argv[optind + 2]);
        }
        if (argc >= optind + 3) {
            previewBody = QString(argv[optind + 3]);
        }

        // Add/update a notification
        QVariantHash hints;
        if (!category.isEmpty()) {
            hints.insert(NotificationManager::HINT_CATEGORY, category);
        }
        if (count >= 0) {
            hints.insert(NotificationManager::HINT_ITEM_COUNT, count);
        }
        if (!timestamp.isEmpty()) {
            hints.insert(NotificationManager::HINT_TIMESTAMP, timestamp);
        }
        if (!actions.isEmpty()) {
            foreach (const QString &name, actions.keys()) {
                hints.insert(QString(NotificationManager::HINT_REMOTE_ACTION_PREFIX) + name, actions.value(name));
            }
        }
        if (!previewSummary.isEmpty()) {
            hints.insert(NotificationManager::HINT_PREVIEW_SUMMARY, previewSummary);
        }
        if (!previewBody.isEmpty()) {
            hints.insert(NotificationManager::HINT_PREVIEW_BODY, previewBody);
        }
        result = proxy.Notify(argv[0], id, icon, summary, body, QStringList(), hints, expireTimeout);
        break;
    }
    case Remove:
        if (id > 0) {
            proxy.CloseNotification(id);
        }
        break;
    default:
        break;
    }

    return result;
}
