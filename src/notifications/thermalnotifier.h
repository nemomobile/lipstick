/***************************************************************************
**
** Copyright (C) 2012-2014 Jolla Ltd.
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
#ifndef THERMALNOTIFIER_H
#define THERMALNOTIFIER_H

#include <qmthermal.h>

class ThermalNotifier : public QObject
{
    Q_OBJECT
public:
    explicit ThermalNotifier(QObject *parent = 0);

private slots:
    /*!
     * Reacts to thermal state changes by showing the a
     * related notification.
     *
     * \param state the new thermal state
     */
    void applyThermalState(MeeGo::QmThermal::ThermalState state);

private:
    /*!
     * Shows a system notification.
     *
     * \param category the category of the notification
     * \param body the body text of the notification
     */
    void createAndPublishNotification(const QString &category, const QString &body);

    //! For getting the thermal state
    MeeGo::QmThermal *thermalState;

#ifdef UNIT_TEST
    friend class Ut_ThermalNotifier;
#endif
};

#endif // THERMALNOTIFIER_H
