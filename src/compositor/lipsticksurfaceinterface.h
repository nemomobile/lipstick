/***************************************************************************
**
** Copyright (C) 2014 Jolla Ltd.
** Contact: Giulio Camuffo <giulio.camuffo@jollamobile.com>
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

#ifndef LIPSTICKSURFACEINTERFACE
#define LIPSTICKSURFACEINTERFACE

#include <QtCompositor/QWaylandSurfaceInterface>

#include "lipstickglobal.h"

class LIPSTICK_EXPORT LipstickOomScoreOp : public QWaylandSurfaceOp
{
public:
    enum { Type = QWaylandSurfaceOp::UserType + 1 };
    LipstickOomScoreOp(int score);

    int score() const { return m_score; }

private:
    int m_score;
};

#endif
