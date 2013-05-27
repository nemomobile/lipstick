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

#include "windowpixmapitem.h"

#include <QSGGeometryNode>
#include <QSGSimpleMaterial>
#include <QWaylandSurfaceItem>
#include "lipstickcompositor.h"

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

private slots:
    void providerDestroyed();
    void textureChanged();

private:
    void setTexture(QSGTexture *texture);

    QSGSimpleMaterial<SurfaceTextureState> *m_material;
    QRectF m_rect;

    QSGTextureProvider *m_provider;
    QSGTexture *m_texture;
    QSGGeometry m_geometry;
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
: m_material(0), m_provider(0), m_texture(0), m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
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

    if (m_texture) {
        QSize ts = m_texture->textureSize();
        QRectF sourceRect(0, 0, ts.width(), ts.height());
        QSGGeometry::updateTexturedRectGeometry(&m_geometry, m_rect,
                                                m_texture->convertToNormalizedSourceRect(sourceRect));
    }
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

void SurfaceNode::setBlending(bool b)
{
    m_material->setFlag(QSGMaterial::Blending, b);
}

void SurfaceNode::setTexture(QSGTexture *texture)
{
    m_material->state()->setTexture(texture);

    m_texture = texture;

    if (m_texture) {
        QSize ts = m_texture->textureSize();
        QRectF sourceRect(0, 0, ts.width(), ts.height());
        QSGGeometry::updateTexturedRectGeometry(&m_geometry, m_rect,
                                                m_texture->convertToNormalizedSourceRect(sourceRect));
    }

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
: m_item(0), m_shaderEffect(0), m_id(0), m_opaque(false)
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

    return node;
}

void WindowPixmapItem::geometryChanged(const QRectF &n, const QRectF &o)
{
    QQuickItem::geometryChanged(n, o);

    if (m_shaderEffect) m_shaderEffect->setSize(n.size());
}

void WindowPixmapItem::updateItem()
{
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
