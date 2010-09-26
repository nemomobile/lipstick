/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouchhome.
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


#ifndef MDESKTOPBACKGROUNDEXTENSIONINTERFACE_H_
#define MDESKTOPBACKGROUNDEXTENSIONINTERFACE_H_

#include <QObject>
#include <QRectF>
#include <MNamespace>
#include <mapplicationextensioninterface.h>

class QPainter;

/*!
 * An interface for getting information about the state of the desktop.
 */
class MDesktopInterface
{
public:
    /*!
     * Asks the desktop to be redrawn
     */
    virtual void update() = 0;

    /*!
     * Returns the current orientation angle of the desktop
     */
    virtual M::OrientationAngle orientationAngle() = 0;
};

/*!
 * Desktop background extension interface. Extensions implementing this
 * interface can draw the desktop background.
 */
class MDesktopBackgroundExtensionInterface : public MApplicationExtensionInterface
{
    Q_INTERFACES(MApplicationExtensionInterface)

public:
    /*!
     * Destroys the desktop background extension.
     */
    virtual ~MDesktopBackgroundExtensionInterface() {}

    /*!
     * Tells the extension about an implementation of the desktop background interface.
     *
     * \param desktopInterface an interface for getting information about the state of the desktop
     */
    virtual void setDesktopInterface(MDesktopInterface &desktopInterface) = 0;

    /*!
     * Tells the extension whether the desktop should be defocused or not.
     *
     * \param defocused \c true if the desktop should be defocused, \c false otherwise
     */
    virtual void setDefocused(bool defocused) = 0;

    /*!
     * Paints the desktop background using the given painter.
     *
     * \param painter the painter to draw the desktop background with.
     * \param boundingRect the bounding rectangle of the desktop area.
     */
    virtual void drawBackground(QPainter *painter, const QRectF &boundingRect) const = 0;
};

Q_DECLARE_INTERFACE(MDesktopBackgroundExtensionInterface, "com.meego.core.MDesktopBackgroundExtensionInterface/1.0")

#endif /* MDESKTOPBACKGROUNDEXTENSIONINTERFACE_H_ */
