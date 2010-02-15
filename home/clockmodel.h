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

#ifndef CLOCKMODEL_H_
#define CLOCKMODEL_H_

#include <DuiWidgetModel>
#include <QTime>

/*!
 * A model for the Clock widget.
 */
class ClockModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL(ClockModel)

    //! The current time
    DUI_MODEL_PROPERTY(QDateTime, time, Time, true, QDateTime::currentDateTime())
};

#endif /* CLOCKMODEL_H_ */
