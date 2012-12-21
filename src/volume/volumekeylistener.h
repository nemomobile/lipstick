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
#include "xtools/xeventlistener.h"

/*!
 * \class VolumeKeyListener
 *
 * \brief Listens to KeyPress and KeyRelease events in all windows.
 *
 * Calls XSelectInput for all windows in the _NET_CLIENT_LIST except
 * Lipstick's own windows in order to listen to key presses and releases.
 */
class VolumeKeyListener : public QObject, XEventListener
{
    Q_OBJECT

public:
    /*!
     * Constructs a volume key listener.
     *
     * \param parent the parent QObject
     */
    VolumeKeyListener(QObject *parent = 0);

    //! \reimp
    virtual bool handleXEvent(const XEvent &event);
    //! \reimp_end

signals:
    //! Sent when a volume key is pressed or released
    void keyEvent(unsigned int key, int type);

private:
    //! Calls XSelectInput for all windows in the _NET_CLIENT_LIST except Lipstick's own windows
    void selectKeyPressInputForAllWindows();
};

#endif // VOLUMEKEYLISTENER_H

