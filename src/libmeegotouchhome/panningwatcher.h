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
#ifndef PANNINGWATCHER_H
#define PANNINGWATCHER_H

#include <QTimer>
#include <QPointF>

class MPannableWidget;

/*!
  PanningWatcher watches the panning of a MPannableWidget and sends
  signals when panning starts or stops.
 */
class PanningWatcher : public QObject
{
    Q_OBJECT

public:
    /*! Constructor
        \param widget the pannable widget to watch
        \param parent object */
    PanningWatcher(MPannableWidget &widget, QObject *parent = 0);

    /*! Returns true if the pannable widget is currently panning */
    virtual bool isPanning();

private slots:
    //! Enables panning and emits a signal if the position of the viewport has changed
    void enablePanningIfPositionChanged();

    //! Disables panning and emits a signal if the position of the viewport has not changed
    void disablePanningIfPositionNotChanged();

signals:
    /*! Sent when the panning starts or stops
      \param panning if true, the pannable widget is currently panning */
    void panningStateChanged(bool panning);

private:
    /*! The pannable widget to watch */
    MPannableWidget &pannableWidget;

    /*! A timer for checking for the stopping of panning after the timeout */
    QTimer movementDetectorTimer;

    /*! The viewport position when panning status was last checked */
    QPointF oldViewportPos;

    /*! True if the pannable widget is currently panning */
    bool panning;

#ifdef UNIT_TEST
    friend class Ut_PanningWatcher;
#endif
};

#endif
