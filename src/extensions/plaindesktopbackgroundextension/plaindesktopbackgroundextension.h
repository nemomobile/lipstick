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

#ifndef PLAINDESKTOPBACKGROUNDEXTENSION_H_
#define PLAINDESKTOPBACKGROUNDEXTENSION_H_

#include <QObject>
#include <QTimeLine>
#include <MGConfItem>
#include "mdesktopbackgroundextensioninterface.h"

class QPixmap;
class WindowInfo;
class PlainDesktopBackgroundPixmap;

/*!
 * An extension that draws the desktop background without any special effects.
 */
class PlainDesktopBackgroundExtension : public QObject, public MDesktopBackgroundExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(MDesktopBackgroundExtensionInterface MApplicationExtensionInterface)

public:
    /*!
     * Constructs a PlainDesktopBackgroundExtension.
     */
    PlainDesktopBackgroundExtension();

    /*!
     * Destroys the PlainDesktopBackgroundExtension.
     */
    virtual ~PlainDesktopBackgroundExtension();

    //! \reimp
    virtual void setDesktopInterface(MDesktopInterface &desktopInterface);
    virtual void setDefocused(bool defocused);
    virtual void drawBackground(QPainter *painter, const QRectF &boundingRect) const;
    virtual bool initialize(const QString &interface);
    virtual QGraphicsWidget *widget();
    //! \reimp_end

private slots:
    /*!
     * Sets the defocus factor.
     *
     * \param defocusFactor the new defocus factor
     */
    void setDefocusFactor(qreal defocusFactor);

    /*!
     * Updates the landscape pixmap.
     */
    void updateLandscapePixmap();

    /*!
     * Updates the portrait pixmap.
     */
    void updatePortraitPixmap();

    /*!
     * Updates the desktop.
     */
    void updateDesktop();

private:
    /*!
     * Updates a pixmap.
     *
     * \param pixmap the pixmap to be updated
     * \param gConfItem the MGConfItem to get the pixmap name from
     * \param defaultName the name of the default pixmap to be used if loading the user selected one fails
     */
    void updatePixmap(QSharedPointer<PlainDesktopBackgroundPixmap> *pixmap, MGConfItem &gConfItem, const QString &defaultName);

    //! An interface for getting information about the state of the desktop
    MDesktopInterface *desktop;

    //! GConf item for the landscape background image
    MGConfItem landscapeGConfItem;

    //! GConf item for the portrait background image
    MGConfItem portraitGConfItem;

    //! The landscape desktop background pixmap
    QSharedPointer<PlainDesktopBackgroundPixmap> landscapePixmap;

    //! The portrait desktop background pixmap
    QSharedPointer<PlainDesktopBackgroundPixmap> portraitPixmap;

    //! The name of the landscape default background image
    QString landscapeDefaultBackgroundImage;

    //! The name of the portrait default background image
    QString portraitDefaultBackgroundImage;

    //! Defocusing factor
    qreal defocusFactor;

    //! Defocusing timeline
    QTimeLine defocusTimeLine;

    //! Blur radius (in pixels)
    int blurRadius;

    //! Brightness (from 0 to 1)
    qreal brightness;

    //! Whether a pixmap is being updated or not
    bool pixmapBeingUpdated;
};

#endif /* PLAINDESKTOPBACKGROUNDEXTENSION_H_ */
