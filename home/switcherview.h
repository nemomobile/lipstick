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

#ifndef SWITCHERVIEW_H
#define SWITCHERVIEW_H

#include <DuiWidgetView>
#include "switchermodel.h"
#include "switcherstyle.h"
#include "switcherphysicsintegrationstrategy.h"

class Switcher;
class QPixmap;
class QTimeLine;
class QGraphicsLinearLayout;
class DuiLayout;
class DuiFlowLayoutPolicy;
class DuiSeparator;
class DuiLinearLayoutPolicy;

/*!
 * The switcher view draws a background for the switcher.
 */
class SwitcherView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(SwitcherModel, SwitcherStyle)

public:
    /*!
     * Constructs a SwitcherView.
     *
     * \param container the Switcher controller to be used
     */
    SwitcherView(Switcher *controller);

    /*!
     * Destroys the SwitcherView.
     */
    virtual ~SwitcherView();

    /*! \reimp
     *
     * Reimplemented here to react to the switcher orientation change
     */ 
    virtual void setGeometry(const QRectF &rect);
    //! reimp_end

protected slots:
    //! \cond
    virtual void updateData(const QList<const char *>& modifications);
    //! \endcond

private slots:
    /*! Monitors the movement of the pannable viewport */
    void sizePosChanged(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos);

    /*! Listens for changes in the physics integrator */
    void snapIndexChanged(int newPosition);

    /*! Listens when the pannig has stopped in the viewport */
    void panningStopped();

private:
    /*! Convenience function to update the panning parameters */
    void updatePanMarginsAndSnapInterval();

    /*! The switcher controller */
    Switcher *controller;

    DuiWidget* layoutWidget;

    /*! Layouts */
    QGraphicsLinearLayout *mainLayout;

    /*! The widget that will contain all of the switcher buttons */
    DuiWidget* pannedWidget;

    /*! Layout for the panned widget */
    QGraphicsLinearLayout* pannedLayout;    

    /*! The integrator for the pannable viewport, controls how the switcher buttons move */
    SwitcherPhysicsIntegrationStrategy* integrator;

    /*! The current focused switcher button */
    int focusedSwitcherButton;

    /*! Keep track of the first button's priority */
    WindowInfo::WindowPriority firstButtonPriority;
};

#endif // SWITCHERVIEW_H
