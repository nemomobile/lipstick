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
#include <QPointer>

#include "lipstickglobal.h"

class HwcImageNode;
class HwcImageLoadRequest;

class QSGTexture;

class LIPSTICK_EXPORT HwcImage : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QColor overlayColor READ overlayColor WRITE setOverlayColor NOTIFY overlayColorChanged)
    Q_PROPERTY(qreal pixelRatio READ pixelRatio WRITE setPixelRatio NOTIFY pixelRatioChanged)
    Q_PROPERTY(QSize textureSize READ textureSize WRITE setTextureSize NOTIFY textureSizeChanged)
    Q_PROPERTY(int maxTextureSize READ maxTextureSize WRITE setMaxTextureSize NOTIFY maxTextureSizeChanged)
    Q_PROPERTY(QString effect READ effect WRITE setEffect NOTIFY effectChanged)
    Q_PROPERTY(QQuickItem *rotationHandler READ rotationHandler WRITE setRotationHandler NOTIFY rotationHandlerChanged)
    Q_PROPERTY(bool asynchronous READ isAsynchronous WRITE setAsynchronous NOTIFY asynchronousChanged)

    Q_ENUMS(Status)

public:
    enum Status { Null, Ready, Loading, Error };

	HwcImage();
	~HwcImage();

	void setSource(const QUrl &url);
	QUrl source() const { return m_source; }

    void setOverlayColor(const QColor &color);
    QColor overlayColor() const { return m_overlayColor; }

    Status status() const { return m_status; }

    void setMaxTextureSize(int size);
    int maxTextureSize() const { return m_maxTextureSize; }

    void setTextureSize(const QSize &size);
    QSize textureSize() const { return m_textureSize; }

    void setEffect(const QString &effect);
    QString effect() const { return m_effect; }

    void setPixelRatio(qreal);
    qreal pixelRatio() const { return m_pixelRatio; }

    void setAsynchronous(bool is);
    bool isAsynchronous() const { return m_asynchronous; }

    void setRotationHandler(QQuickItem *rotationHandler);
    QQuickItem *rotationHandler() const { return m_rotationHandler; }

    static bool hasEffectReferences(QQuickItem *item);

protected:
    bool event(QEvent *event);
	void updatePolish();

	QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data);

signals:
    void sourceChanged();
    void statusChanged();
    void overlayColorChanged();
    void textureSizeChanged();
    void effectChanged();
    void pixelRatioChanged();
    void asynchronousChanged();
    void rotationHandlerChanged();
    void maxTextureSizeChanged();

private slots:
    void handlerRotationChanged();
    void onWindowChange();
    void onSync();

private:
    friend class HwcImageLoadRequest;
    void apply(HwcImageLoadRequest *);
    QMatrix4x4 reverseTransform() const;
    HwcImageNode *updateActualPaintNode(QSGNode *node);

    HwcImageLoadRequest *m_pendingRequest;
    QPointer<QQuickWindow> m_window;
    QQuickItem *m_rotationHandler;
	QUrl m_source;
    QImage m_image;
    QSize m_textureSize;
    QString m_effect;
    Status m_status;
    QColor m_overlayColor;
    qreal m_pixelRatio;
    qreal m_textureRotation;
    int m_maxTextureSize;
    bool m_asynchronous : 1;
    bool m_usedInEffect : 1;
    bool m_updateImage : 1;
};

#endif
