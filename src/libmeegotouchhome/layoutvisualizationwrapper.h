/*
 * This file is part of mhome
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef LAYOUTVISUALIZATIONWRAPPER_H
#define LAYOUTVISUALIZATIONWRAPPER_H

#include <QtGlobal>

class QGraphicsLinearLayout;

/*!
 * A helper class for wrapping the visual appearance of a widget that has
 * a linear layout. The visualization wrapper shows the right edge of the widget
 * next to the left edge and vice versa.
 *
 * Only the visual appearance gets wrapped, nothing is done to the actual layout
 * or its contents.
 *
 * \note This class only works with horizontal linear layouts. With vertical layouts
 *       this class has no effect.
 *
 * \note Changing the layout's contents after giving the layout to this helper and
 *       setting a wrapping mode might lead to unwanted results.
 */
class LayoutVisualizationWrapper
{
public:
    /*!
     * Constructs a new layout visualization wrapper using \a layout.
     * \param layout the layout that gets wrapped.
     */
    LayoutVisualizationWrapper(QGraphicsLinearLayout &layout);

    /*!
     * Destructor.
     */
    virtual ~LayoutVisualizationWrapper();

    /*!
     * The wrapping mode.
     */
    enum WrappingMode {
        NoWrap,               //!< No wrapping used
        WrapRightEdgeToLeft,  //!< Wrap the right edge next to the left edge
        WrapLeftEdgeToRight   //!< Wrap the left edge next to the right edge
    };

    /*!
     * Sets the wrapping mode.
     * \param mode the new wrapping mode.
     */
    void setWrappingMode(WrappingMode mode);

private:
    //! The layout that is used for the visualization wrapping
    const QGraphicsLinearLayout &layout;

    //! The current wrapping mode
    WrappingMode wrappingMode;

    //! Width of current parent layout
    qreal previousParentLayoutWidth;

    //! A direction
    enum Direction {
        Left,   //!< Left
        Right   //!< Right
    };

    /*!
     * Resets the transformation of the item in the layout indicated by \a layoutIndex.
     * \param layoutIndex the index to the layout.
     */
    void resetTransformation(int layoutIndex) const;

    /*!
     * Sets a transformation to a layout item in the layout indicated by \a layoutIndex.
     * Argument \a direction determines to what direction the transformation is made.
     * \param layoutIndex the index to the layout.
     * \param direction the direction where the transformation will be made.
     */
    void setTransformation(int layoutIndex, Direction direction);

    /*!
     * Fetches widht of parent layout from parent widget
     */
    qreal parentLayoutWidth() const;
};

#endif // LAYOUTVISUALIZATIONWRAPPER_H
