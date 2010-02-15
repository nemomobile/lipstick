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

#ifndef NOTIFICATIONPARAMETER_H_
#define NOTIFICATIONPARAMETER_H_

#include <QString>
#include <QVariant>

/*!
 * NotificationParameter is a class for providing a notification parameter.
 */
class NotificationParameter
{
public:
    /*!
     * Creates a notification parameter with a given name and value.
     *
     * \param name the name of the parameter
     * \param value the value of the parameter
     */
    NotificationParameter(const QString &name, const QVariant &value);

    /*!
     * Destroys the NotificationParameter.
     */
    virtual ~NotificationParameter();

    /*!
     * Returns the name of the parameter.
     *
     * \return the name of the parameter
     */
    QString name() const;

    /*!
     * Returns the value of the parameter.
     *
     * \return the value of the parameter
     */
    QVariant value() const;

private:
    //! The name of the parameter
    QString _name;
    //! The value of the parameter
    QVariant _value;
};

#endif /* NOTIFICATIONPARAMETER_H_ */
