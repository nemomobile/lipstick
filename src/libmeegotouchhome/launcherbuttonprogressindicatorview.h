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

#ifndef LAUNCHERBUTTONPROGRESSINDICATORVIEW_H
#define LAUNCHERBUTTONPROGRESSINDICATORVIEW_H

#include <MWidgetView>
#include "launcherbuttonprogressindicator.h"
#include "launcherbuttonprogressindicatorstyle.h"
#include "launcherbuttonstyle.h"

class QPropertyAnimation;

class LauncherButtonProgressIndicatorView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(LauncherButtonProgressIndicatorModel, LauncherButtonProgressIndicatorStyle)

    //! \internal
    Q_PROPERTY(int currentFrame READ currentFrame WRITE setCurrentFrame)
    //! \internal_end

public:
    LauncherButtonProgressIndicatorView(LauncherButtonProgressIndicator *controller);

    virtual ~LauncherButtonProgressIndicatorView();

    //! Returns current frame
    int currentFrame() const;
    //! Sets current frame
    void setCurrentFrame(int newCurrentFrame);

protected:
    //! reimp
    void setupModel();
    void updateData(const QList<const char *>& modifications);
    void applyStyle();
    void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    //! \reimp_end

public slots:
    //! Resumes progress animation
    void resumeAnimation();
    //! Pauses progress animation
    void pauseAnimation();
    //! Pauses or resumes progress animation according to visibility and 'unknown duration' state
    void pauseOrResumeAnimation();

private:
    //! Updates indicator style mode according to state.
    void updateStyleMode();

    //! Reloads frame pixmaps from theme
    void reloadFrames();

    //! Releases used frame pixmaps
    void releaseUsedPixmaps();

    //! Calculates the frame index from value
    int frameFromValue(const int &value) const;

    //! Animation for the progress. Animates the current frame.
    QScopedPointer<QPropertyAnimation> positionAnimation;

    //! Current frame index (-1 if value or set frame is invalid)
    int currentFrameIndex;
    //! Loaded progress pixmaps
    QList<const QPixmap *> animationPixmaps;
    //! Controller
    LauncherButtonProgressIndicator *controller;


#ifdef UNIT_TEST
    friend class Ut_LauncherButtonProgressIndicatorView;
#endif
};

#endif
