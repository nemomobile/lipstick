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
#ifndef PAGEDPANNING_H
#define PAGEDPANNING_H

#include <mphysics2dpanning.h>

/*!
 * \class PagedPanning
 *
 * \brief This class extends the \c MPhysics2DPanning to provide
 * a paged effect to the viewport
 *
 * The paging is only supported in the horizontal direction.
 */
class PagedPanning : public MPhysics2DPanning
{
    Q_OBJECT

public:
    /*!
     * Constructs a PagedPanning
     * \param parent The parent object
     */
    PagedPanning(QObject* parent);

    /*!
     * Destroys a PagedPanning
     */
    virtual ~PagedPanning();

    /*!
     * Sets the number of pages.
     *
     * \param pageCount The page count
     */
    void setPageCount(int pageCount);

    /*!
     * \return the current page count
     */
    int pageCount() const;

    /*!
     * Pans the view automatically to the given item index
     */
    void panToPage(int page);

    /*!
     * Sets the specified page immediately
     *
     * \param page The page to set
     */
    void setPage(uint page);

    /*!
     * Sets the snap activation velocity threshold
     */
    void setVelocityThreshold(qreal value);

    /*!
     * Sets the snap activation drag threshold
     */
    void setDragThreshold(qreal value);

    /*!
     * Sets the maximum number of pages to change per gesture
     */
    void setSlideLimit(int value);

    /*!
     * Sets the page snapping spring stiffness factor
     */
    void setPageSnapSpringK(qreal value);

    /*!
     * Sets the page snapping spring damping friction factor
     */
    void setPageSnapFriction(qreal value);

    /*!
     * Sets the pan threshold value
     */
    void setPanThreshold(qreal value);

    /*!
     * \return the currently active page
     */
    int activePage() const;

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
     * Enables or disables the page wrapping mode.
     *
     * \param enable enables or disables the mode
     * \sa pageWrapped
     */
    void setPageWrapMode(bool enable);

    /*!
     * Queries the current page wrap mode.
     */
    bool pageWrapMode() const;

    //! \reimp
    virtual void pointerPress(const QPointF &pos);
    virtual void pointerMove(const QPointF &pos);
    virtual void pointerRelease();
    //! \reimp_end

protected:

    //! \reimp
    virtual void integrateAxis(Qt::Orientation orientation, qreal &position, qreal &velocity, qreal &acceleration, qreal &pointerDifference, bool pointerPressed);
    //! \reimp_end

signals:
    /*!
     * Indicates that the page has changed
     * \param page The new page
     */
    void pageChanged(int page);

    /*!
     * Indicates that the page is panning
     * \param panning \c true is page is panning. \c false otherwise
     */
    void pageIsPanning(bool panning);

    /*!
     * A signal that gets emitted when a page wrap happens.
     * \sa setPageWrapMode
     */
    void pageWrapped();

private slots:

    /*!
     * Pan to the current page if not already on it
     */
    void panToCurrentPage();

private:
    //! The number of pages
    int pageCount_;

    //! The current page index
    int currentPage;

    //! True when snapping to pages
    bool snapMode;

    //! The range during the previous integration step
    QRectF previousRange;

    //! Snap activation velocity threshold
    qreal velocityThreshold_;

    //! Panning treshold
    qreal panThreshold;

    //! Snap activation drag threshold
    qreal dragThreshold_;

    //! Maximum number of pages to change per gesture
    int slideLimit_;

    //! Page snapping spring stiffness factor
    qreal pageSnapSpringK_;

    //! Page snapping spring damping friction factor
    qreal pageSnapFriction_;

    //! The view position during the previous integration step
    qreal previousPosition;

    //! Target page index
    int targetPage_;

    //! Page on which the pointer first came down
    int initialPage;

    //! Page width during the previous integration step
    qreal previousPageWidth;

    /*! Calculate the sliding distance with the given
     *  initial velocity and friction factor.
     */
    qreal slideDistance(qreal initialVelocity, qreal friction);

    //! The point in which the pointer was pressed
    QPointF pressPosition;

    //! Distance from the press position
    qreal distanceToPressPosition;

    //! Velocity threshold to be exceeded in order to pan to the initial target page
    qreal initialTargetPageVelocityThreshold;

    //! If true, panning wraps the page around the edges
    bool wrapMode;

    //! If true, the page is currently being wrapped
    bool pageWrapping;

#ifdef UNIT_TEST
    //! Test unit is defined as a friend of production code to access private members
    friend class Ut_PagedPanning;
#endif
};

#endif
