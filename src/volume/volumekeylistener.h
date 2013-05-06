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

#ifndef VOLUMEKEYLISTENER_H
#define VOLUMEKEYLISTENER_H

#include <QObject>

/*!
 * \class VolumeKeyListener
 *
 * \brief Listens to KeyPress and KeyRelease events in all windows.
 */
class VolumeKeyListener : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs a volume key listener.
     *
     * \param parent the parent QObject
     */
    VolumeKeyListener(QObject *parent = 0);

signals:
    //! Sent when a volume key is pressed or released
    void keyEvent(unsigned int key, int type);
};

#endif // VOLUMEKEYLISTENER_H

