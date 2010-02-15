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

class Switcher;
class QPixmap;
class QTimeLine;
class QGraphicsLinearLayout;
class DuiLayout;
class DuiFlowLayoutPolicy;
class DuiSeparator;

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

protected slots:
    //! \cond
    virtual void updateData(const QList<const char *>& modifications);
    //! \endcond

private:
    /*! The switcher controller */
    Switcher *controller;

    /*! Layouts */
    QGraphicsLinearLayout *mainLayout;
    DuiLayout *layout;
    DuiFlowLayoutPolicy *policy;

    /*! Top and bottom separators */
    DuiSeparator *topSeparator;
    DuiSeparator *bottomSeparator;
};

#endif // SWITCHERVIEW_H
