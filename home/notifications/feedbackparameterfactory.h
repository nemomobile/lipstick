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


#ifndef FEEDBACKPARAMETERFACTORY_H_
#define FEEDBACKPARAMETERFACTORY_H_

#include "notificationparameter.h"

/*!
 * Creates NotificationParameters related to feedbacks.
 */
class FeedbackParameterFactory
{
public:
    /*!
     * Returns the keyname of the feedback id parameter
     */
    static QString feedbackIdKey() {
        return QString("feedbackId");
    }

    /*!
     * Creates a NotificationParameter with the given feedback ID.
     *
     * \param feedbackId the feedback ID for the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createFeedbackIdParameter(const QString &feedbackId) {
        return NotificationParameter(feedbackIdKey(), QVariant(feedbackId));
    }
};

#endif /* FEEDBACKPARAMETERFACTORY_H_ */
