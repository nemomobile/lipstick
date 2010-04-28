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

protected:

    //! \reimp
    virtual void integrateAxis( Qt::Orientation orientation,
                                qreal &position,
                                qreal &velocity,
                                qreal &acceleration,
                                qreal &pointerDifference,
                                bool pointerPressed
                                );
    //! \reimp_end

signals:

    /*!
     * Indecates that the page has chaged
     * \param page The new page
     */
    void pageChanged(int page);

private:

    //! The number of pages
    int pageCount_;

    //! The current index
    int currentPage;

    //! Indicates if the panning is automatically targeting a specific page
    bool autoIntegrateMode;

    //! The target page of the automatic integration
    int autoIntegrateTargetPage;

    //! The range during the previous integration step
    QRectF previousRange;

#ifdef UNIT_TEST
    //! Test unit is defined as a friend of production code to access private members
    friend class Ut_PagedPanning;
#endif

};

#endif
