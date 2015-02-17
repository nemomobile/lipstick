/***************************************************************************
**
** Copyright (C) 2015 Jolla Ltd.
** Contact: Gunnar Sletta <gunnar.sletta@jollamobile.com>
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

#ifndef HWCIMAGE_H
#define HWCIMAGE_H

#include <QUrl>
#include <QImage>
#include <QQuickItem>

#include "lipstickglobal.h"

class HwcImageNode;

class LIPSTICK_EXPORT HwcImage : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(qreal darkness READ darkness WRITE setDarkness NOTIFY darknessChanged)

    Q_ENUMS(Status)

public:
    enum Status { Null, Ready, Loading, Error };

	HwcImage();
	~HwcImage();

	void setSource(const QUrl &url);
	QUrl source() const { return m_source; }

    void setDarkness(qreal darkness);
    qreal darkness() const { return m_darkness; }

    Status status() const { return m_status; }

protected:
	void updatePolish();

	QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data);

signals:
    void sourceChanged();
    void statusChanged();
    void darknessChanged();

private:
    HwcImageNode *updateActualPaintNode(QSGNode *node);

	QUrl m_source;
    QImage m_image;
    Status m_status;
    qreal m_darkness;
};

#endif