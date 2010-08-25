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

#ifndef SWITCHERBUTTONWITHTITLEBARVIEW_H
#define SWITCHERBUTTONWITHTITLEBARVIEW_H

#include "switcherbuttonview.h"
#include "switcherbuttonwithtitlebarstyle.h"

class SwitcherButton;
class MButton;
class MLabel;
class QGraphicsLinearLayout;

/*!
 * \class SwitcherButtonWithTitleBarView
 *
 * \brief SwitcherButtonWithTitleBarView implements a switcher view with a title bar
 */
class SwitcherButtonWithTitleBarView : public SwitcherButtonView
{
    Q_OBJECT
    M_VIEW(SwitcherButtonModel, SwitcherButtonWithTitleBarStyle)

public:
    /*!
     * \brief Constructs a new SwitcherButtonWithTitleBarView.
     *
     * \param controller The button's controller
     */
    SwitcherButtonWithTitleBarView(SwitcherButton *controller);

    /*!
     * \brief Destroys the SwitcherButtonWithTitleBarView.
     */
    virtual ~SwitcherButtonWithTitleBarView();

    //! \reimp
    virtual void applyStyle();
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

protected:
    //! \reimp
    virtual void setupModel();
    virtual QPoint thumbnailPosition() const;
    //! \reimp_end

private:
    //! Button for closing the window
    MButton *closeButton;

    //! Title label
    MLabel *titleLabel;

    //! Title bar layout
    QGraphicsLinearLayout *titleBarLayout;

#ifdef UNIT_TEST
    friend class Ut_SwitcherButtonWithTitleBarView;
#endif

    Q_DISABLE_COPY(SwitcherButtonWithTitleBarView);
};

#endif
