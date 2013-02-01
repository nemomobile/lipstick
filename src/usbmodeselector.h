/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef USBMODESELECTOR_H
#define USBMODESELECTOR_H

#include <QMap>
#include <qmusbmode.h>
#include "lipstickglobal.h"

class QDeclarativeView;

namespace MeeGo {
class QmLocks;
}

class LIPSTICK_EXPORT USBModeSelector : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool windowVisible READ windowVisible WRITE setWindowVisible NOTIFY windowVisibleChanged)

public:
    explicit USBModeSelector(QObject *parent = 0);
    
    /*!
     * Returns whether the window is visible or not.
     *
     * \return \c true if the window is visible, \c false otherwise
     */
    bool windowVisible() const;

    /*!
     * Sets the visibility of the window.
     *
     * \param visible \c true if the window should be visible, \c false otherwise
     */
    void setWindowVisible(bool visible);

    /*!
     * Sets the USB mode to the given mode.
     *
     * \param mode the mode to set
     */
    Q_INVOKABLE void setUSBMode(int mode);

signals:
    //! Signaled when the USB mode dialog is shown.
    void dialogShown();

    //! Sent when the visibility of the window has changed.
    void windowVisibleChanged();

private slots:
    /*!
     * Shows the USB dialog/banners based on the given USB mode.
     *
     * \param mode the USB mode to show UI elements for
     */
    void applyUSBMode(MeeGo::QmUSBMode::Mode mode);

    /*!
     * Shows an error string matching the given error code, if any.
     *
     * \param errorCode the error code of the error to be shown
     */
    void showError(const QString &errorCode);

    /*!
     * Shows the USB dialog/banners based on the current USB mode.
     */
    void applyCurrentUSBMode();

private:
    /*!
     * Shows a notification.
     *
     * \param mode the USB mode for the notification
     */
    void showNotification(MeeGo::QmUSBMode::Mode mode);

    //! MeeGo::QmUSBMode::error() error code to translation ID mapping
    static QMap<QString, QString> errorCodeToTranslationID;

    //! The volume control window
    QDeclarativeView *window;

    //! For getting and setting the USB mode
    MeeGo::QmUSBMode *usbMode;

    //! For getting information about the device lock state
    MeeGo::QmLocks *locks;

#ifdef UNIT_TEST
    friend class Ut_USBModeSelector;
#endif
};

#endif // USBMODESELECTOR_H
