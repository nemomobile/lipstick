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

#ifndef APPLICATIONCONTEXT_H
#define APPLICATIONCONTEXT_H

#include <QObject>

class ContextItem;

/*!
 * Abstract interface that can be used to create context items that
 * communicate the context of the application (e.g. battery level,
 * network strength, device orientation, bluetooth status, ...)
 */
class ApplicationContext
{
public:
    /*!
     * Creates a new context item. Ownership of the context item is transferred
     * to the caller.
     * \param key Unique identifier of the context item. This key will identify the
     * context item monitored in the application context.
     * \return New ContextItem object that can be used to track the particular context
     * item in the system. Ownership of the created ContextItem object is transferred
     * to the caller.
     */
    virtual ContextItem *createContextItem(const QString &key) = 0;
};

/*!
 * Abstract interface that describes one item of the application context.
 * Concrete implementations of this interface can connect to various
 * backends to retrieve the state of the item such as context framework.
 */
class ContextItem: public QObject
{
    Q_OBJECT

public:
    /*!
     * Returns current value of the context item.
     * \return Current value of the context item as a QVariant.
     */
    virtual QVariant value() const = 0;

signals:
    /*!
     * Signal to indicate that the value of the context item has changed.
     */
    void contentsChanged();
};

#endif // APPLICATIONCONTEXT_H
