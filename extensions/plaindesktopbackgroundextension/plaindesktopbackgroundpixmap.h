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

#ifndef PLAINDESKTOPBACKGROUNDPIXMAP_H_
#define PLAINDESKTOPBACKGROUNDPIXMAP_H_

class QPixmap;

/*!
 * A helper class representing a background pixmap.
 */
class PlainDesktopBackgroundPixmap
{
public:
    /*!
     * Loads a background pixmap by a given name. If the name begins with a /
     * the pixmap is loaded from a local file. Otherwise it is considered to
     * be a pixmap ID and is loaded from the theme.
     *
     * \param name the name of the pixmap to load
     * \param defaultName the name of the default pixmap to load if loading the user selected one fails
     * \param blurRadius the blur radius (in pixels)
     */
    PlainDesktopBackgroundPixmap(const QString &name, const QString &defaultName, int blurRadius);

    /*!
     * Frees the background pixmap.
     */
    ~PlainDesktopBackgroundPixmap();

    /*!
     * Returns the normal pixmap.
     *
     * \return the normal pixmap
     */
    const QPixmap *pixmap() const;

    /*!
     * Returns the blurred pixmap.
     *
     * \return the blurred pixmap
     */
    const QPixmap *blurredPixmap() const;

    /*!
     * Returns the name of the pixmap.
     *
     * \return the name of the pixmap
     */
    QString pixmapName() const;

private:
    // The pixmap if read from the theme
    const QPixmap *pixmapFromTheme_;

    //! The pixmap if read from a file
    QSharedPointer<QPixmap> pixmapFromFile_;

    //! A blurred version of the pixmap
    QSharedPointer<QPixmap> blurredPixmap_;

    //! The name of the pixmap
    QString pixmapName_;

    /*!
     * Creates a blurred pixmap from a pixmap.
     *
     * \param pixmap the pixmap to blur
     * \param blurRadius the blur radius (in pixels)
     * \return a blurred version of the pixmap
     */
    static QPixmap *createBlurredPixmap(const QPixmap &pixmap, int blurRadius);

#ifdef UNIT_TEST
    friend class Ut_PlainDesktopBackgroundPixmap;
#endif
};

#endif /* PLAINDESKTOPBACKGROUNDPIXMAP_H_ */
