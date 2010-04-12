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
#ifndef PAGEDVIEWPORT_H
#define PAGEDVIEWPORT_H

#include <MPannableViewport>

class PagedPanning;

/*!
 * \class PagedViewport
 * \brief PagedViewport creates a view that allows any widget to be panned a set 
 * of pages.
 *
 * The PagedViewport extends the MPannableViewport functionality so that it
 * pans the widget inside the viewport through pages. The page within this class
 * refres to the visual appearance only. 
 *
 * The widget inside the MPannableViewport
 * is not split into multiple component or widgets. The PagedViewport only modifies the
 * movement of the MPannableViewport so that the widget being panned appears to
 * consist of multiple pages.
 * 
 * Currently only horizontal paging is supported.
 */
class PagedViewport : public MPannableViewport
{
    Q_OBJECT

public:
    /*!
     * Constructs a PagedViewport
     *
     * \param parent The parent of this viewport
     */
    PagedViewport(QGraphicsItem *parent = 0);

    /*!
     * Destroys this PagedViewport
     */
    virtual ~PagedViewport();

    /*! \reimp
     * Sets pan direction to horizontal regardles of user's wishes.
     * Setting direction to vertical has undetermined effects since
     * it's currently not supported 
     */
    void setPanDirection(const Qt::Orientations &panDirection);
    //! \reimp_end

Q_SIGNALS:

    /*!
     * This signal is sent when ever the page changes inside the viewport. The page change
     * can occur programmatically or with an user interaction
     *
     * \param newPage the page that the viewport is currently showing
     */
    void pageChanged(int newPage);

public Q_SLOTS:

    /*!
     * Makes the viewport pan to a specific page. If the value exceeds the current amount
     * of pages then the last page is shown.
     *
     * \param page The page to pan to
     */
    void panToPage(uint page);

    /*!
     * Informs the viewport the width of the page. The viewport needs to know the width
     * of a page so that it can pan them correctly.
     *
     * \param width The width of the page in pixels.
     */
    void updatePageWidth(int width);

private:

    //! Our custom phyics implementation
    PagedPanning* pagedPanning;
};

#endif
