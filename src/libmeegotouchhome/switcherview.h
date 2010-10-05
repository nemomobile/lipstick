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

#ifndef SWITCHERVIEW_H
#define SWITCHERVIEW_H

#include <MWidgetView>
#include "switcherviewbase.h"
#include "switchermodel.h"
#include "switcherstyle.h"

class Switcher;
class MLinearLayoutPolicy;
class MGridLayoutPolicy;
class PagedViewport;

/*!
 * The switcher view draws a background for the switcher.
 */
class SwitcherView : public SwitcherViewBase
{
    Q_OBJECT
    M_VIEW(SwitcherModel, SwitcherStyle)

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

protected:
    //! \reimp
    virtual void applySwitcherMode();
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    virtual void applyPinchGestureTargetMode();
    //! \reimp_end

private slots:
    /*! Listens for page change in the paged viewport */
    void updateFocusedButton(int currentPage);

    /*! Listens when the pannig has stopped in the viewport */
    void panningStopped();

    /*! Update all buttons in the layout policies */
    void updateButtons();

private:
    /*! Repositions the switcher so that the visible page is reflects the topmost application */
    void repositionSwitcher();

    /*! Adds a button in the correct position in the overview layout policy */
    void addButtonInOverviewPolicy(QSharedPointer<SwitcherButton> button);

    /*! Updates the content margin and spacings of the layout policies */
    void updateContentsMarginsAndSpacings();

    /*! Updates the content margins and spacings of the detail view layout policy */
    void updateDetailViewContentsMarginsAndSpacings();

    /*! Updates the content margins and spacings of the overview layout policy */
    void updateOverviewContentsMarginsAndSpacings();

    /*! Updates the modes of the buttons and the page count according to the switcher mode */
    void updateButtonModesAndPageCount();

    //! Returns the maximum number of buttons in a page
    int buttonsPerPage() const;

    /*! Layout policies for the different switcher modes */
    MGridLayoutPolicy   *overviewPolicy;
    MLinearLayoutPolicy *detailPolicy;

    /*! The current focused switcher button */
    int focusedSwitcherButton;

    /*! The viewport that shows the switcher buttons */
    PagedViewport *pagedViewport;

    /*! Style of the switcher in overview mode */
    const SwitcherStyle *overviewStyle;

#ifdef UNIT_TEST
    // to test snapIndexChanged effects
    friend class Ut_SwitcherView;
#endif
};

#endif // SWITCHERVIEW_H
