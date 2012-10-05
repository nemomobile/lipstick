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

// The count of items represented by the notification
uint count = 1;

// Timestamp for notification
QString timestamp;

// Prints usage information
int usage(const char *program)
{
    std::cerr << std::setw(7) << "Usage: " << program << " [OPTION]... CATEGORY [SUMMARY BODY IMAGE ACTION...]" << std::endl;
    std::cerr << std::setw(7) << "Manage notifications." << std::endl;
    std::cerr << std::setw(7) << std::endl;
    std::cerr << std::setw(7) << "Mandatory arguments to long options are mandatory for short options too." << std::endl;
    std::cerr << std::setw(7) << "  -o, --operation=OPERATION  The operation (add/update/remove) to perform." << std::endl;
    std::cerr << std::setw(7) << "                             add - Adds a new notification." << std::endl;
    std::cerr << std::setw(7) << "                             update - Updates an existing notification." << std::endl;
    std::cerr << std::setw(7) << "                             remove - Removes an existing notification." << std::endl;
    std::cerr << std::setw(7) << "  -i, --id=ID                The notification ID to use." << std::endl;
    std::cerr << std::setw(7) << "  -c, --count=NUMBER         The number of items represented by thenotification." << std::endl;
    std::cerr << std::setw(7) << "  -t, --timestamp            Timestamp to use on a notification. Use ISO 8601 extended date format."<< std::endl;
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
            { "count", required_argument, NULL, 'c' },
            { "help", no_argument, NULL, 'h' },
            { "timestamp", required_argument, NULL, 't'},
            { 0, 0, 0, 0 }
        };

        int c = getopt_long(argc, argv, "o:i:c:t:", long_options, &option_index);
        if (c == -1)
            break;

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
        case 'c':
            count = atoi(optarg);
            break;
        case 'h':
            return usage(argv[0]);
            break;
        case 't':
            timestamp = QString(optarg);
            break;
        default:
            break;
        }
    }

    if (toolOperation == Undefined ||
            (toolOperation == Add && argc < optind + 1) ||
            (toolOperation == Add && id != 0) ||
            (toolOperation == Update && argc < optind + 1) ||
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
        QString category = QString(argv[optind]);
        QString summary, body, image;
        QStringList actions;
        if (argc >= optind + 1) {
            summary = QString(argv[optind + 1]);
        }
        if (argc >= optind + 2) {
            body = QString(argv[optind + 2]);
        }
        if (argc >= optind + 3) {
            image = QString(argv[optind + 3]);
        }
        if (argc >= optind + 4) {
            for (int action = optind + 4; action < argc; action++) {
                actions.append(QString(argv[action]));
            }
        }

        // Add/update a notification
        QVariantHash hints;
        hints.insert(NotificationManager::HINT_CATEGORY, category);
        hints.insert(NotificationManager::HINT_ITEM_COUNT, count);
        hints.insert(NotificationManager::HINT_TIMESTAMP, timestamp);
        result = proxy.Notify(argv[0], id, image, summary, body, actions, hints, -1);
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
