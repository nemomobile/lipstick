/*
 * This file is part of mhome
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * Contact: Tomas Junnonen <tomas.junnonen@nokia.com>
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved. Copying,
 * including reproducing, storing, adapting or translating, any or all of
 * this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */
#ifndef PAGEDPANNING_H
#define PAGEDPANNING_H

#include <QObject>
#include <mphysics2dpanning.h>

/*!
 * \class PagedPanning
 *
 * \brief This class extends the \c MPhysics2DPanning to provide
 * a paged effect to the viewport
 *
 * The paging is only supported in the horizontal direction.
 *
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
     * \return the currently active page
     */
    int activePage() const;

    /*!
     * Returns the page width
     */
    float pageWidth() const;

protected:

    //! \reimp
    virtual void integrateAxis( Qt::Orientation orientation,
                                qreal &position,
                                qreal &velocity,
                                qreal &acceleration,
                                qreal &pointerDifference,
                                bool pointerPressed
                                );

    virtual void pointerPress(const QPointF &pos);

    virtual void pointerMove(const QPointF &pos);

    virtual void pointerRelease();

    //! \reimp_end

signals:

    /*!
     * Indecates that the page has chaged
     * \param page The new page
     */
    void pageChanged(int page);

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
    int targetPage;

    //! Page on which the pointer first came down
    int initialPage;

    //! Current page width
    qreal pageWidth_;

    //! Page width during the previous integration step
    qreal previousPageWidth;

    /*! Calculate the sliding distance with the given
     *  initial velocity and friction factor.
     */
    qreal slideDistance(qreal initialVelocity, qreal friction);

#ifdef UNIT_TEST
    //! Test unit is defined as a friend of production code to access private members
    friend class Ut_PagedPanning;
#endif

};

#endif
