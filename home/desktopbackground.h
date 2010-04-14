/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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

#ifndef DESKTOPBACKGROUND_H
#define DESKTOPBACKGROUND_H

#include <MGConfItem>
#include <QPixmap>

class DesktopStyleContainer;

class MGConfItem;

/*!
 * The desktop background handles loading the background image according
 * to GConf settings, and takes care of validating the configured images.
 * It also returns the correct background image for the current orientation
 * and will observe GConf setting modifications and tell the client to
 * reload the background if a setting for the current orientation changes.
 */
class DesktopBackground : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs a DesktopBackground.
     * \param parent Parent object that will handle lifetime.
     */
    explicit DesktopBackground(QObject* parent);

    /*!
     * Destroys the DesktopBackground.
     */
    virtual ~DesktopBackground();

    /*!
     * Gets the current backgroundImage, depending on the
     * current orientation. Returns the background image from
     * theme if none was set through customization, or if
     * the size of the customized image is not suitable for
     * the resolution.
     */
    QPixmap backgroundImage(const DesktopStyleContainer& style);

signals:
    /*!
     * Called if the current background image needs to be
     * redrawn, because it has changed. This signal will be
     * emitted if the gconf keys for the images are modified.
     */
    void backgroundImageChanged();

private slots:
    //! Called if the portrait image's gconf value changes
    void reloadPortraitImage();

    //! Called if the landscape image's gconf value changes
    void reloadLandscapeImage();
private:
    //! loads and verifies the portrait image
    //! \return true if portrait image was changed
    bool loadPortraitPixmap();
    //! loads and verifies the landscape image
    //! \return true if landscape image was changed
    bool loadLandscapePixmap();
private:
    //! gconf item for portrait image
    MGConfItem portraitBackgroundImageConfig_;
    /*!
     * portrait image, or null QPixmap if the image
     * given was invalid or empty
     */
    QPixmap portraitBackgroundImage_;
    /*! The last gconf value known to work for portrait background image.
     * Used when loading a new image fails
     */
    QString lastKnownGoodPortraitBackgroundImagePath_;
    //! gconf item for landscape image
    MGConfItem landscapeBackgroundImageConfig_;
    /*!
     * landscape image, or null QPixmap if the image
     * given was invalid or empty
     */
    QPixmap landscapeBackgroundImage_;
    /*! The last gconf value known to work for landscape background image.
     * Used when loading a new image fails
     */
    QString lastKnownGoodLandscapeBackgroundImagePath_;

#ifdef UNIT_TEST
    friend class Ut_DesktopBackground;
#endif
};

#endif // DESKTOPBACKGROUND_H
