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

#ifndef NOTIFIER_H_
#define NOTIFIER_H_

#include <DuiButton>

class NotifierNotificationSink;

/*!
 * The Notifier is a widget that displays the number of
 * notifications that have not been dismissed (if any).
 */
class Notifier : public DuiButton
{
    Q_OBJECT

public:
    /*!
     * Constructs a new Notifier.
     *
     * \param parent the parent widget
     */
    Notifier(DuiWidget *parent = NULL);

    /*!
     * Destroys the Notifier.
     */
    virtual ~Notifier();

private slots:
    /*!
     * Receives information about notification count changes.
     *
     * \param count the number of notifications
     */
    void notificationCountChanged(uint count);

private:
    //! The notification sink for the notifier
    NotifierNotificationSink *notifierSink;
};

#endif /* NOTIFIER_H_ */
