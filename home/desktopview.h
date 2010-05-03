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
#include "contextframeworkcontext.h"
#include "mdesktopbackgroundextensioninterface.h"

class Desktop;
class Switcher;
class QuickLaunchBar;
class LauncherDataStore;
class Launcher;
class NotificationArea;
class StatusIndicator;
class QGraphicsLinearLayout;
class AppletSpace;
class MModalSceneWindow;
class MPannableViewport;
class MOverlay;
class MApplicationExtensionArea;
class MApplicationExtensionInterface;
class MDesktopBackgroundExtensionInterface;

/*!
 * The desktop view draws a background for the desktop and manages layouts
 * related to the desktop. The applets are inside a flow layout, which in
 * turn is inside a main layout. In addition to this the main layout
 * contains a button for displaying the applet inventory.
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
    virtual void setGeometry(const QRectF &rect);
    virtual M::OrientationAngle orientationAngle();
    //! \reimp_end

private slots:
    //! \reimp
    virtual void update();
    //! \reimp_end

    //! Shows the launcher if it is not visible, hides it otherwise
    void toggleLauncher();

    //! Shows the applet space if it is not visible, hides it otherwise
    void toggleAppletSpace();

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

#ifdef BENCHMARKS_ON
private slots:
    void startBenchmarking();
    void stopBenchmarking();
    void writeFps();
#endif

private:
    /*!
     * \brief Shows the application launcher
     */
    void showLauncher();

    /*!
     * \brief Hides the application launcher
     */
    void hideLauncher();

    /*!
     * Creates a launcher data store.
     *
     * \return an initialized launcher data store
     */
    LauncherDataStore *createLauncherDataStore();

    //! The controller
    Desktop *desktop;

    //! The switcher widget
    Switcher *switcher;

    //! Data store to store the launcher and quick launch bar item locations to
    LauncherDataStore *launcherDataStore;

    //! Application launcher
    Launcher *launcher;

    //! scene window for the launcher
    MModalSceneWindow *launcherWindow;

    //! The quick launch bar
    QuickLaunchBar *quickLaunchBar;

    //! Scene window for the quick launch bar
    MOverlay *quickLaunchBarWindow;

    //! Phone network indicator
    StatusIndicator *phoneNetworkIndicator;

    //! Context framework application context for the indicators
    ContextFrameworkContext contextFrameworkContext;

    //! The applet space
    AppletSpace *appletSpace;

    //! Scene window for the applet space
    MModalSceneWindow *appletSpaceWindow;

    //! Pannable viewport in which the applet space is displayed
    MPannableViewport *appletSpaceViewport;

    //! Application extension support for desktop background drawing
    MApplicationExtensionArea *backgroundExtensionArea;

    //! A list of desktop background extensions
    QList<MDesktopBackgroundExtensionInterface*> backgroundExtensions;

#ifdef UNIT_TEST
    friend class Ut_DesktopView;
#endif
};

#endif // DESKTOPVIEW_H
