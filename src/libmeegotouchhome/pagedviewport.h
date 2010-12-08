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
#include "layoutvisualizationwrapper.h"

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

    /*!
     * Set the number of pages.
     *
     * \param pages The number of pages.
     */
    void updatePageCount(int pages);

    /*!
     * Returns the currently active page
     */
    int currentPage() const;

    /*!
     * Gets the target page where the panning is going to if there is panning
     * going on. If panning is not going on, then this returns the current page.
     */
    int targetPage() const;

    /*!
     * Returns the page width
     */
    float pageWidth() const;

    /*!
     * Enables or disables the page wrapping mode. When enabled, the pannable
     * viewport wraps back to the first or the last page when panning over the opposite side.
     *
     * \param enable enables or disables the mode
     */
    void setPageWrapMode(bool enable);

Q_SIGNALS:

    /*!
     * This signal is sent when ever the page changes inside the viewport. The page change
     * can occur programmatically or with an user interaction
     *
     * \param newPage the page that the viewport is currently showing
     */
    void pageChanged(int newPage);

    /*!
     * This signal is sent when the page count is changed.
     *
     * \param newPage the page new page count
     */
    void pageCountChanged(int newPageCount);

    /*!
     * Indicates that a page is panning
     * \param panning \c if true then page is panning, \c false otherwise
     */
    void pageIsPanning(bool panning);

public Q_SLOTS:

    /*!
     * Makes the viewport pan to a specific page. If the value exceeds the current amount
     * of pages then the last page is shown.
     *
     * \param page The page to pan to
     */
    void panToPage(uint page);

    /*!
     * Moves the viewport to the specified page immediately without animation
     *
     * \param page The page to set
     */
    void setPage(uint page);

    /*!
     * Moves the viewport to the first page
     */
    void focusFirstPage();

private Q_SLOTS:
    /*!
     * A slot for handling page changes in the paged viewport. Updates the visualization
     * wrapper if needed.
     */
    void newPageSet(int newPage);

    /*!
     * Updates the layout visualization wrapper to reflect the current status of panning.
     */
    void updateVisualizationWrapper();

private:
    //! Our custom physics implementation
    PagedPanning* pagedPanning;

    //! Keeps track of the previous page where the panning is coming from.
    //! This is only used when the page wrapping mode is enabled. It is used to
    //! determine if the page change wrapped or not.
    int previousPage;

    //! A layout visualization wrapper to be used when page wrap mode is on
    QSharedPointer<LayoutVisualizationWrapper> layoutVisualizationWrapper;

#ifdef UNIT_TEST
    friend class Ut_PagedViewport;
#endif
};

#endif
