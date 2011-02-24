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

#ifndef DESKTOPVIEW_H
#define DESKTOPVIEW_H

#include <MWidgetView>
#include "desktopmodel.h"
#include "desktopstyle.h"
#include "mdesktopbackgroundextensioninterface.h"
#include "windowinfo.h"

class Desktop;
class Switcher;
class QuickLaunchBar;
class LauncherDataStore;
class Launcher;
class NotificationArea;
class QGraphicsLinearLayout;
class MSceneWindow;
class MPannableViewport;
class MOverlay;
class MApplicationExtensionArea;
class MApplicationExtensionInterface;
class MDesktopBackgroundExtensionInterface;
class HomeScreenService;
class ApplicationPackageMonitorListener;

/*!
 * The desktop view draws a background for the desktop and manages layouts
 * related to the desktop.
 */
class DesktopView : public MWidgetView, public MDesktopInterface
{
    Q_OBJECT
    M_VIEW(DesktopModel, DesktopStyle)

public:
    /*!
     * Constructs a DesktopView.
     *
     * \param desktop the Desktop controller to be used
     */
    DesktopView(Desktop *desktop);

    /*!
     * Destroys the DesktopView.
     */
    virtual ~DesktopView();

    //! \reimp
#ifdef BENCHMARKS_ON
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);
#endif
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void update();
    virtual M::OrientationAngle orientationAngle();
    //! \reimp_end

private slots:
    //! Sets whether the switcher has content or not based on the window list
    void setSwitcherHasContent(const QList<WindowInfo> &windowList);

    //! Toggles the launcher visibility
    void toggleLauncher();

    /*!
     * \brief Shows the application launcher
     */
    void showLauncher();

    /*!
     * \brief Hides the application launcher
     */
    void hideLauncher();

    /*!
     * Registers a desktop background extension.
     *
     * \param an extension to be registered.
     */
    void addExtension(MApplicationExtensionInterface *extension);

    /*!
     * Unregisters a desktop background extension.
     *
     * \param an extension to be unregistered.
     */
    void removeExtension(MApplicationExtensionInterface *extension);

    /*!
     * Shows Launcher and focuses Launcher to an application's Launcher Button specified
     * by a desktop file
     *
     * \param desktop file name or file path
     */
    void showLauncherAndFocusToButton(const QString &desktopFileEntry = QString());

#ifdef BENCHMARKS_ON
private slots:
    void startBenchmarking();
    void stopBenchmarking();
    void writeFps();
#endif

private:
    /*!
     * \brief Rearranges the Z order of the scene window
     */
    void setSceneWindowOrder();

    /*!
     * \brief Lets the background extensions know whether the desktop should be defocused or not
     */
    void setDefocused();

    /*!
     * Creates a launcher data store.
     *
     * \return an initialized launcher data store
     */
    LauncherDataStore *createLauncherDataStore();

    //! The switcher widget
    Switcher *switcher;

    //! Scene window for the switcher
    MSceneWindow *switcherWindow;

    //! Whether the switcher has content in it
    bool switcherHasContent;

    //! Data store to store the launcher and quick launch bar item locations to
    LauncherDataStore *launcherDataStore;

    //! Application package monitor
    ApplicationPackageMonitorListener *packageMonitorListener;

    //! Application launcher
    Launcher *launcher;

    //! Scene window for the launcher
    MSceneWindow *launcherWindow;

    //! Whether the launcher is visible or not
    bool launcherVisible;

    //! The quick launch bar
    QuickLaunchBar *quickLaunchBar;

    //! Scene window for the quick launch bar
    MOverlay *quickLaunchBarWindow;

    //! Application extension support for desktop background drawing
    MApplicationExtensionArea *backgroundExtensionArea;

    //! A list of desktop background extensions
    QList<MDesktopBackgroundExtensionInterface*> backgroundExtensions;

#ifdef UNIT_TEST
    friend class Ut_DesktopView;
#endif
};

#endif // DESKTOPVIEW_H
