/***************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Aaron Kennedy <aaron.kennedy@jollamobile.com>
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

#include <QtCore/qmath.h>
#include <QSGGeometryNode>
#include <QSGSimpleMaterial>
#include <QWaylandSurfaceItem>
#include "lipstickcompositorwindow.h"
#include "lipstickcompositor.h"
#include "windowpixmapitem.h"

namespace {

class SurfaceTextureState {
public:
    SurfaceTextureState() : m_texture(0) {}
    void setTexture(QSGTexture *texture) { m_texture = texture; }
    QSGTexture *texture() const { return m_texture; }

private:
    QSGTexture *m_texture;
};

class SurfaceTextureMaterial : public QSGSimpleMaterialShader<SurfaceTextureState>
{
    QSG_DECLARE_SIMPLE_SHADER(SurfaceTextureMaterial, SurfaceTextureState)
public:
    QList<QByteArray> attributes() const;
    void updateState(const SurfaceTextureState *newState, const SurfaceTextureState *oldState);
protected:
    const char *vertexShader() const;
    const char *fragmentShader() const;
};

class SurfaceNode : public QObject, public QSGGeometryNode
{
    Q_OBJECT
public:
    SurfaceNode();
    void setRect(const QRectF &);
    void setTextureProvider(QSGTextureProvider *);
    void setBlending(bool);
    void setRadius(qreal radius);

private slots:
    void providerDestroyed();
    void textureChanged();

private:
    void setTexture(QSGTexture *texture);
    void updateGeometry();

    QSGSimpleMaterial<SurfaceTextureState> *m_material;
    QRectF m_rect;
    qreal m_radius;

    QSGTextureProvider *m_provider;
    QSGTexture *m_texture;
    QSGGeometry m_geometry;
    QRectF m_textureRect;
};

QList<QByteArray> SurfaceTextureMaterial::attributes() const
{
    QList<QByteArray> attributeList;
    attributeList << "qt_VertexPosition";
    attributeList << "qt_VertexTexCoord";
    return attributeList;
}

void SurfaceTextureMaterial::updateState(const SurfaceTextureState *newState,
                                         const SurfaceTextureState *)
{
    if (newState->texture())
        newState->texture()->bind();
}

const char *SurfaceTextureMaterial::vertexShader() const
{
    return "uniform highp mat4 qt_Matrix;                      \n"
           "attribute highp vec4 qt_VertexPosition;            \n"
           "attribute highp vec2 qt_VertexTexCoord;            \n"
           "varying highp vec2 qt_TexCoord;                    \n"
           "void main() {                                      \n"
           "    qt_TexCoord = qt_VertexTexCoord;               \n"
           "    gl_Position = qt_Matrix * qt_VertexPosition;   \n"
           "}";
}

const char *SurfaceTextureMaterial::fragmentShader() const
{
    return "varying highp vec2 qt_TexCoord;                    \n"
           "uniform sampler2D qt_Texture;                      \n"
           "uniform lowp float qt_Opacity;                     \n"
           "void main() {                                      \n"
           "    gl_FragColor = texture2D(qt_Texture, qt_TexCoord) * qt_Opacity; \n"
           "}";
}

SurfaceNode::SurfaceNode()
: m_material(0), m_radius(0), m_provider(0), m_texture(0),
  m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 0)
{
    setGeometry(&m_geometry);
    m_material = SurfaceTextureMaterial::createMaterial();
    setMaterial(m_material);
}

void SurfaceNode::setRect(const QRectF &r)
{
    if (m_rect == r)
        return;

    m_rect = r;

    updateGeometry();
}

void SurfaceNode::setTextureProvider(QSGTextureProvider *p)
{
    if (p == m_provider)
        return;

    if (m_provider) {
        QObject::disconnect(m_provider, SIGNAL(destroyed(QObject *)), this, SLOT(providerDestroyed()));
        QObject::disconnect(m_provider, SIGNAL(textureChanged()), this, SLOT(textureChanged()));
        m_provider = 0;
    }

    m_provider = p;

    QObject::connect(m_provider, SIGNAL(destroyed(QObject *)), this, SLOT(providerDestroyed()));
    QObject::connect(m_provider, SIGNAL(textureChanged()), this, SLOT(textureChanged()));

    setTexture(m_provider->texture());
}

void SurfaceNode::updateGeometry()
{
    if (m_texture) {
        QSize ts = m_texture->textureSize();
        QRectF sourceRect(0, 0, ts.width(), ts.height());
        QRectF textureRect = m_texture->convertToNormalizedSourceRect(sourceRect);

        if (m_radius) {
            float radius = qMin(float(qMin(m_rect.width(), m_rect.height()) * 0.5f), float(m_radius));
            int segments = qBound(5, qCeil(radius * (M_PI / 6)), 18);
            float angle = 0.5f * float(M_PI) / segments;

            m_geometry.allocate((segments + 1) * 2 * 2);

            QSGGeometry::TexturedPoint2D *v = m_geometry.vertexDataAsTexturedPoint2D();
            QSGGeometry::TexturedPoint2D *vlast = v + (segments + 1) * 2 * 2 - 2;

            float textureXRadius = radius * textureRect.width() / m_rect.width();
            float textureYRadius = radius * textureRect.height() / m_rect.height();

            float c = 1; float cosStep = qFastCos(angle);
            float s = 0; float sinStep = qFastSin(angle);

            for (int ii = 0; ii <= segments; ++ii) {
                float px = m_rect.left() + radius - radius * c;
                float tx = textureRect.left() + textureXRadius - textureXRadius * c;

                float px2 = m_rect.right() - radius + radius * c;
                float tx2 = textureRect.right() - textureXRadius + textureXRadius * c;

                float py = m_rect.top() + radius - radius * s;
                float ty = textureRect.top() + textureYRadius - textureYRadius * s;

                float py2 = m_rect.bottom() - radius + radius * s;
                float ty2 = textureRect.bottom() - textureYRadius + textureYRadius * s;

                v[0].x = px; v[0].y = py;
                v[0].tx = tx; v[0].ty = ty;

                v[1].x = px; v[1].y = py2;
                v[1].tx = tx; v[1].ty = ty2;

                vlast[0].x = px2; vlast[0].y = py;
                vlast[0].tx = tx2; vlast[0].ty = ty;

                vlast[1].x = px2; vlast[1].y = py2;
                vlast[1].tx = tx2; vlast[1].ty = ty2;

                v += 2;
                vlast -= 2;

                float t = c;
                c = c * cosStep - s * sinStep;
                s = s * cosStep + t * sinStep;
            }
        } else {
            m_geometry.allocate(4);
            QSGGeometry::updateTexturedRectGeometry(&m_geometry, m_rect, textureRect);
        }

        markDirty(DirtyGeometry);
    }
}

void SurfaceNode::setBlending(bool b)
{
    m_material->setFlag(QSGMaterial::Blending, b);
}

void SurfaceNode::setRadius(qreal radius)
{
    if (m_radius == radius)
        return;

    m_radius = radius;

    updateGeometry();
}

void SurfaceNode::setTexture(QSGTexture *texture)
{
    m_material->state()->setTexture(texture);

    QRectF tr;
    if (texture) tr = texture->convertToNormalizedSourceRect(QRect(QPoint(0,0), texture->textureSize()));

    bool ug = !m_texture || tr != m_textureRect;

    m_texture = texture;
    m_textureRect = tr;

    if (ug) updateGeometry();

    markDirty(DirtyMaterial);
}

void SurfaceNode::textureChanged()
{
    setTexture(m_provider->texture());
}

void SurfaceNode::providerDestroyed()
{
    m_provider = 0;
    setTexture(0);
}

}

WindowPixmapItem::WindowPixmapItem()
: m_item(0), m_shaderEffect(0), m_id(0), m_opaque(false), m_radius(0)
{
    setFlag(ItemHasContents);
}

WindowPixmapItem::~WindowPixmapItem()
{
    setWindowId(0);
}

int WindowPixmapItem::windowId() const
{
    return m_id;
}

void WindowPixmapItem::setWindowId(int id)
{
    if (m_id == id)
        return;
    
    if (m_item) {
        m_item->imageRelease();
        m_item = 0;
    }

    m_id = id;
    updateItem();

    emit windowIdChanged();
}

bool WindowPixmapItem::opaque() const
{
    return m_opaque;
}

void WindowPixmapItem::setOpaque(bool o)
{
    if (m_opaque == o)
        return;

    m_opaque = o;
    if (m_item) update();

    emit opaqueChanged();
}

qreal WindowPixmapItem::radius() const
{
    return m_radius;
}

void WindowPixmapItem::setRadius(qreal r)
{
    if (m_radius == r)
        return;

    m_radius = r;
    if (m_item) update();

    emit radiusChanged();
}

QSize WindowPixmapItem::windowSize() const
{
    if (!m_item || !m_item->surface()) {
        return QSize();
    }

    return m_item->surface()->size();
}

void WindowPixmapItem::setWindowSize(const QSize &s)
{
    if (!m_item || !m_item->surface()) {
        return;
    }

    m_item->surface()->requestSize(s);
}

QSGNode *WindowPixmapItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    SurfaceNode *node = static_cast<SurfaceNode *>(oldNode);

    if (m_item == 0) {
        delete node;
        return 0;
    }

    if (!node) node = new SurfaceNode;

    node->setTextureProvider(m_item->textureProvider());
    node->setRect(QRectF(0, 0, width(), height()));
    node->setBlending(!m_opaque);
    node->setRadius(m_radius);

    return node;
}

void WindowPixmapItem::geometryChanged(const QRectF &n, const QRectF &o)
{
    QQuickItem::geometryChanged(n, o);

    if (m_shaderEffect) m_shaderEffect->setSize(n.size());
}

void WindowPixmapItem::updateItem()
{
    if (m_item && m_item->surface()) {
        disconnect(m_item->surface(), SIGNAL(sizeChanged()), this, SIGNAL(windowSizeChanged()));
    }

    LipstickCompositor *c = LipstickCompositor::instance();
    m_item = 0;

    if (c && m_id) {
        LipstickCompositorWindow *w = static_cast<LipstickCompositorWindow *>(c->windowForId(m_id));

        if (!w) {
            delete m_shaderEffect;
            m_shaderEffect = 0;
            return;
        } else if (w->surface()) {
            m_item = w;
            delete m_shaderEffect; m_shaderEffect = 0;
            connect(m_item->surface(), SIGNAL(sizeChanged()), this, SIGNAL(windowSizeChanged()));
        } else {
            if (!m_shaderEffect) {
                m_shaderEffect = static_cast<QQuickItem *>(c->shaderEffectComponent()->create());
                Q_ASSERT(m_shaderEffect);
                m_shaderEffect->setParentItem(this);
                m_shaderEffect->setSize(QSizeF(width(), height()));
            }

            m_shaderEffect->setProperty("window", qVariantFromValue((QObject *)w));
        }

        w->imageAddref();

        update();
    }
}

#include "windowpixmapitem.moc"
