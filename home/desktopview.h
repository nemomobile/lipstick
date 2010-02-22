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

#ifndef DESKTOPVIEW_H
#define DESKTOPVIEW_H

#include <DuiWidgetView>
#include "desktopmodel.h"
#include "desktopstyle.h"
#include "contextframeworkcontext.h"

class Desktop;
class Switcher;
class QuickLaunchBar;
class Launcher;
class NotificationArea;
class StatusIndicator;
class QGraphicsLinearLayout;
class QTimeLine;
class AppletSpace;
class DuiModalSceneWindow;
class DuiPannableViewport;
class DuiOverlay;

/*!
 * The desktop view draws a background for the desktop and manages layouts
 * related to the desktop. The applets are inside a flow layout, which in
 * turn is inside a main layout. In addition to this the main layout
 * contains a button for displaying the applet inventory.
 */
class DesktopView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DesktopModel, DesktopStyle)

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
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual QRectF boundingRect() const;
    virtual void setGeometry(const QRectF &rect);
    //! \reimp_end

private slots:
    //! Shows the launcher if it is not visible, hides it otherwise
    void toggleLauncher();

    //! Shows the applet space if it is not visible, hides it otherwise
    void toggleAppletSpace();

private:
    /*!
     * \brief Shows the application launcher
     */
    void showLauncher();

    /*!
     * \brief Hides the application launcher
     */
    void hideLauncher();

    //! The controller
    Desktop *desktop;

    //! The switcher widget
    Switcher *switcher;

    //! The quick launch bar
    QuickLaunchBar *quickLaunchBar;

    //! Scene window for the quick launch bar
    DuiOverlay *quickLaunchBarWindow;

    //! Application launcher
    Launcher *launcher;

    //! scene window for the launcher
    DuiModalSceneWindow *launcherWindow;

    //! Pannable viewport in which the launcher is displayed
    DuiPannableViewport *launcherViewport;

    //! Phone network indicator
    StatusIndicator *phoneNetworkIndicator;

    //! Context framework application context for the indicators
    ContextFrameworkContext contextFrameworkContext;

    //! The applet space
    AppletSpace *appletSpace;

    //! Scene window for the applet space
    DuiModalSceneWindow *appletSpaceWindow;

    //! Pannable viewport in which the applet space is displayed
    DuiPannableViewport *appletSpaceViewport;

#ifdef UNIT_TEST
    friend class Ut_DesktopView;
#endif
};

#endif // DESKTOPVIEW_H
