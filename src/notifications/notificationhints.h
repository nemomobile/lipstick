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

#ifndef NOTIFICATIONHINTS_H
#define NOTIFICATIONHINTS_H

#include "lipstickglobal.h"
#include <QHash>
#include <QVariant>

class QDataStream;
class QDBusArgument;

/*!
 * Notification hints is a class for providing hints for a notification.
 * Hints are a way to provide extra data to a notification server that the server
 * may be able to make use of.
 */
class LIPSTICK_EXPORT NotificationHints
{
public:
    //! Standard hint: The urgency level.
    static const char *HINT_URGENCY;

    //! Standard hint: The type of notification this is.
    static const char *HINT_CATEGORY;

    //! Standard hint: This specifies the name of the desktop filename representing the calling program. This should be the same as the prefix used for the application's .desktop file. An example would be "rhythmbox" from "rhythmbox.desktop". This can be used by the daemon to retrieve the correct icon for the application, for logging purposes, etc.
    static const char *HINT_DESKTOP_ENTRY;

    //! Standard hint: This is a raw data image format which describes the width, height, rowstride, has alpha, bits per sample, channels and image data respectively. We use this value if the icon field is left blank.
    static const char *HINT_IMAGE_DATA;

    //! Standard hint: The path to a sound file to play when the notification pops up.
    static const char *HINT_SOUND_FILE;

    //! Standard hint: Causes the server to suppress playing any sounds, if it has that ability. This is usually set when the client itself is going to play its own sound.
    static const char *HINT_SUPPRESS_SOUND;

    //! Standard hint: Specifies the X location on the screen that the notification should point to. The "y" hint must also be specified.
    static const char *HINT_X;

    //! Standard hint: Specifies the Y location on the screen that the notification should point to. The "x" hint must also be specified.
    static const char *HINT_Y;

    //! Nemo hint: Class of the notification (application/system).
    static const char *HINT_CLASS;

    //! Nemo hint: Icon of the notification.
    static const char *HINT_ICON;

    //! Nemo hint: Item count represented by the notification.
    static const char *HINT_ITEM_COUNT;

    //! Nemo hint: Timestamp of the notification.
    static const char *HINT_TIMESTAMP;

    //! Nemo hint: Icon of the preview of the notification.
    static const char *HINT_PREVIEW_ICON;

    //! Nemo hint: Body text of the preview of the notification.
    static const char *HINT_PREVIEW_BODY;

    //! Nemo hint: Summary text of the preview of the notification.
    static const char *HINT_PREVIEW_SUMMARY;

    //! Nemo hint: User removability of the notification.
    static const char *HINT_USER_REMOVABLE;

    //! Nemo hint: Translation ID for the generic text of the notification.
    static const char *HINT_GENERIC_TEXT_TRANSLATION_ID;

    //! Nemo hint: Translation catalogue name for the generic text of the notification.
    static const char *HINT_GENERIC_TEXT_TRANSLATION_CATALOGUE;

    /*!
     * Creates a notification hints object.
     */
    NotificationHints();

    /*!
     * Sets the value of a hint.
     *
     * \param hint the name of the hint to set the value of
     * \param value a value of the hint
     */
    void setHint(const QString &hint, const QVariant &hintValue);

    /*!
     * Gets the value of a hint.
     *
     * \param hint the name of the hint to get the value of
     * \return the value of the hint
     */
    QVariant hintValue(const QString &hint) const;

    /*!
     * Returns a list of hints contained by the notification hints object.
     *
     * \return a list of hints
     */
    QStringList hints() const;

    friend LIPSTICK_EXPORT QDBusArgument &operator<<(QDBusArgument &, const NotificationHints &);
    friend const LIPSTICK_EXPORT QDBusArgument &operator>>(const QDBusArgument &, NotificationHints &);

private:
    QHash<QString, QVariant> hintValues;
};

QDebug operator<<(QDebug debug, const NotificationHints &hints);

Q_DECLARE_METATYPE(NotificationHints)

#endif // NOTIFICATIONHINTS_H
