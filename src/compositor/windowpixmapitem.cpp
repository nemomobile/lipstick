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
#include <QOpenGLFramebufferObject>
#include <QWaylandSurfaceItem>
#include "lipstickcompositorwindow.h"
#include "lipstickcompositor.h"
#include "windowpixmapitem.h"

namespace {

class SurfaceTextureState {
public:
    SurfaceTextureState() : m_texture(0), m_xScale(1), m_yScale(1) {}
    void setTexture(QSGTexture *texture) { m_texture = texture; }
    QSGTexture *texture() const { return m_texture; }
    void setXOffset(float xOffset) { m_xOffset = xOffset; }
    float xOffset() const { return m_xOffset; }
    void setYOffset(float yOffset) { m_yOffset = yOffset; }
    float yOffset() const { return m_yOffset; }
    void setXScale(float xScale) { m_xScale = xScale; }
    float xScale() const { return m_xScale; }
    void setYScale(float yScale) { m_yScale = yScale; }
    float yScale() const { return m_yScale; }

private:
    QSGTexture *m_texture;
    float m_xOffset;
    float m_yOffset;
    float m_xScale;
    float m_yScale;
};

class SurfaceTextureMaterial : public QSGSimpleMaterialShader<SurfaceTextureState>
{
    QSG_DECLARE_SIMPLE_SHADER(SurfaceTextureMaterial, SurfaceTextureState)
public:
    QList<QByteArray> attributes() const;
    void updateState(const SurfaceTextureState *newState, const SurfaceTextureState *oldState);
protected:
    void initialize();
    const char *vertexShader() const;
    const char *fragmentShader() const;
private:
    int m_id_texOffset;
    int m_id_texScale;
};

class SurfaceNode : public QObject, public QSGGeometryNode
{
    Q_OBJECT
public:
    SurfaceNode();
    ~SurfaceNode();
    void setRect(const QRectF &);
    void setTextureProvider(QSGTextureProvider *, bool owned);
    void setBlending(bool);
    void setRadius(qreal radius);
    void setXOffset(qreal xOffset);
    void setYOffset(qreal yOffset);
    void setXScale(qreal xScale);
    void setYScale(qreal yScale);
    void setUpdateTexture(bool upd);

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
    bool m_providerOwned;
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
    Q_ASSERT(newState->texture());
    if (QSGTexture *tex = newState->texture())
        tex->bind();
    program()->setUniformValue(m_id_texOffset, newState->xOffset(), newState->yOffset());
    program()->setUniformValue(m_id_texScale, newState->xScale(), newState->yScale());
}

void SurfaceTextureMaterial::initialize()
{
    QSGSimpleMaterialShader::initialize();
    m_id_texOffset = program()->uniformLocation("texOffset");
    m_id_texScale = program()->uniformLocation("texScale");
}

const char *SurfaceTextureMaterial::vertexShader() const
{
    return "uniform highp mat4 qt_Matrix;                      \n"
           "attribute highp vec4 qt_VertexPosition;            \n"
           "attribute highp vec2 qt_VertexTexCoord;            \n"
           "varying highp vec2 qt_TexCoord;                    \n"
           "uniform highp vec2 texOffset;                      \n"
           "uniform highp vec2 texScale;                       \n"
           "void main() {                                      \n"
           "    qt_TexCoord = qt_VertexTexCoord * texScale + texOffset;\n"
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

SurfaceNode::~SurfaceNode()
{
    if (m_provider && m_providerOwned)
        delete m_provider;
}

void SurfaceNode::setRect(const QRectF &r)
{
    if (m_rect == r)
        return;

    m_rect = r;

    updateGeometry();
}

void SurfaceNode::setTextureProvider(QSGTextureProvider *p, bool owned)
{
    if (p == m_provider)
        return;

    if (m_provider) {
        QObject::disconnect(m_provider, SIGNAL(destroyed(QObject *)), this, SLOT(providerDestroyed()));
        QObject::disconnect(m_provider, SIGNAL(textureChanged()), this, SLOT(textureChanged()));
        m_provider = 0;
    }

    m_provider = p;
    m_providerOwned = owned;

    if (m_provider) {
        QObject::connect(m_provider, SIGNAL(destroyed(QObject *)), this, SLOT(providerDestroyed()));
        QObject::connect(m_provider, SIGNAL(textureChanged()), this, SLOT(textureChanged()));

        setTexture(m_provider->texture());
    }
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

void SurfaceNode::setXOffset(qreal offset)
{
    m_material->state()->setXOffset(offset);

    markDirty(DirtyMaterial);
}

void SurfaceNode::setYOffset(qreal offset)
{
    m_material->state()->setYOffset(offset);

    markDirty(DirtyMaterial);
}

void SurfaceNode::setXScale(qreal xScale)
{
    m_material->state()->setXScale(xScale);

    markDirty(DirtyMaterial);
}

void SurfaceNode::setYScale(qreal yScale)
{
    m_material->state()->setYScale(yScale);

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

SnapshotProgram *WindowPixmapItem::s_snapshotProgram = 0;

struct SnapshotProgram
{
    QOpenGLShaderProgram program;
    int vertexLocation;
    int textureLocation;
};

class SnapshotTextureProvider : public QSGTextureProvider
{
public:
    SnapshotTextureProvider() : t(0), fbo(0) {}
    ~SnapshotTextureProvider()
    {
        delete fbo;
        delete t;
    }
    QSGTexture *texture() const Q_DECL_OVERRIDE
    {
        return t;
    }
    QSGTexture *t;
    QOpenGLFramebufferObject *fbo;
};


WindowPixmapItem::WindowPixmapItem()
: m_item(0), m_shaderEffect(0), m_id(0), m_opaque(false), m_radius(0), m_xOffset(0), m_yOffset(0)
, m_xScale(1), m_yScale(1), m_unmapLock(0), m_hasBuffer(false), m_surfaceDestroyed(false), m_haveSnapshot(false)
, m_textureProvider(0)
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

    QSize oldSize = windowSize();
    if (m_item) {
        if (m_item->surface()) {
            disconnect(m_item->surface(), &QWaylandSurface::sizeChanged, this, &WindowPixmapItem::handleWindowSizeChanged);
            disconnect(m_item->surface(), &QWaylandSurface::configure, this, &WindowPixmapItem::configure);
            disconnect(m_item.data(), &QWaylandSurfaceItem::surfaceDestroyed, this, &WindowPixmapItem::surfaceDestroyed);
        }
        if (!m_surfaceDestroyed)
            m_item->imageRelease(this);
        m_item->setDelayRemove(false);
        m_item = 0;
        delete m_unmapLock;
        m_unmapLock = 0;
    }

    m_surfaceDestroyed = false;
    m_haveSnapshot = false;
    m_hasBuffer = false;
    m_id = id;
    updateItem();

    emit windowIdChanged();
    if (windowSize() != oldSize)
        emit windowSizeChanged();
}

void WindowPixmapItem::surfaceDestroyed()
{
    m_surfaceDestroyed = true;
    m_hasBuffer = false;
    m_unmapLock = new QWaylandUnmapLock(m_item->surface());
    m_item->imageRelease(this);
    update();
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

qreal WindowPixmapItem::xOffset() const
{
    return m_xOffset;
}

void WindowPixmapItem::setXOffset(qreal xOffset)
{
    if (m_xOffset == xOffset)
        return;

    m_xOffset = xOffset;
    if (m_item) update();

    emit xOffsetChanged();
}

qreal WindowPixmapItem::yOffset() const
{
    return m_yOffset;
}

void WindowPixmapItem::setYOffset(qreal yOffset)
{
    if (m_yOffset == yOffset)
        return;

    m_yOffset = yOffset;
    if (m_item) update();

    emit yOffsetChanged();
}

qreal WindowPixmapItem::xScale() const
{
    return m_xScale;
}

void WindowPixmapItem::setXScale(qreal xScale)
{
    if (m_xScale == xScale)
        return;

    m_xScale = xScale;
    if (m_item || m_haveSnapshot) update();

    emit xScaleChanged();
}

qreal WindowPixmapItem::yScale() const
{
    return m_yScale;
}

void WindowPixmapItem::setYScale(qreal yScale)
{
    if (m_yScale == yScale)
        return;

    m_yScale = yScale;
    if (m_item || m_haveSnapshot) update();

    emit yScaleChanged();
}

QSize WindowPixmapItem::windowSize() const
{
    return m_windowSize;
}

void WindowPixmapItem::setWindowSize(const QSize &s)
{
    if (!m_item || !m_item->surface()) {
        return;
    }

    m_item->surface()->requestSize(s);
}

void WindowPixmapItem::handleWindowSizeChanged()
{
    if (m_item->surface()->size().isValid()) {
        // Window size is retained even when surface is destroyed. This allows snapshots to continue
        // rendering correctly.
        m_windowSize = m_item->surface()->size();
        emit windowSizeChanged();
    }
}

QSGNode *WindowPixmapItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    SurfaceNode *node = static_cast<SurfaceNode *>(oldNode);

    if (m_item == 0 && !m_haveSnapshot) {
        if (node)
            node->setTextureProvider(0, false);
        delete node;
        return 0;
    }

    QSGTextureProvider *provider = 0;
    QSGTexture *texture = 0;
    if (m_item) {
        provider = m_item->textureProvider();
        texture = provider->texture();
        if (texture && !m_surfaceDestroyed) {
            m_haveSnapshot = false;
        }
    }

    if (!m_hasBuffer && texture) {
        if (!m_textureProvider) {
            SnapshotTextureProvider *prov = new SnapshotTextureProvider;
            m_textureProvider = prov;

            if (!s_snapshotProgram) {
                s_snapshotProgram = new SnapshotProgram;
                s_snapshotProgram->program.addShaderFromSourceCode(QOpenGLShader::Vertex,
                    "attribute highp vec4 vertex;\n"
                    "varying highp vec2 texPos;\n"
                    "void main(void) {\n"
                    "   texPos = vertex.xy;\n"
                    "   gl_Position = vec4(vertex.xy * 2.0 - 1.0, 0, 1);\n"
                    "}");
                s_snapshotProgram->program.addShaderFromSourceCode(QOpenGLShader::Fragment,
                    "uniform sampler2D texture;\n"
                    "varying highp vec2 texPos;\n"
                    "void main(void) {\n"
                    "   gl_FragColor = texture2D(texture, texPos);\n"
                    "}");
                if (!s_snapshotProgram->program.link())
                    qDebug() << s_snapshotProgram->program.log();

                s_snapshotProgram->vertexLocation = s_snapshotProgram->program.attributeLocation("vertex");
                s_snapshotProgram->textureLocation = s_snapshotProgram->program.uniformLocation("texture");

                connect(window(), &QQuickWindow::sceneGraphInvalidated, this, &WindowPixmapItem::cleanupOpenGL);
            }
        }
        provider = m_textureProvider;

        if (m_unmapLock) {
            SnapshotTextureProvider *prov = static_cast<SnapshotTextureProvider *>(provider);

            if (!prov->fbo || prov->fbo->size() != QSize(width(), height())) {
                delete prov->fbo;
                delete prov->t;
                prov->t = 0;
                prov->fbo = new QOpenGLFramebufferObject(width(), height());
            }

            prov->fbo->bind();
            s_snapshotProgram->program.bind();

            texture->bind();

            static GLfloat const triangleVertices[] = {
                1.f, 0.f,
                1.f, 1.f,
                0.f, 0.f,
                0.f, 1.f,
            };
            s_snapshotProgram->program.enableAttributeArray(s_snapshotProgram->vertexLocation);
            s_snapshotProgram->program.setAttributeArray(s_snapshotProgram->vertexLocation, triangleVertices, 2);

            glViewport(0, 0, width(), height());
            glDisable(GL_BLEND);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            s_snapshotProgram->program.release();

            if (!prov->t) {
                prov->t = window()->createTextureFromId(prov->fbo->texture(), prov->fbo->size(), 0);
                emit prov->textureChanged();
            }
            prov->fbo->release();
            delete m_unmapLock;
            m_unmapLock = 0;
            s_snapshotProgram->program.disableAttributeArray(s_snapshotProgram->vertexLocation);

            m_haveSnapshot = true;
        }
    } else if (!m_hasBuffer && m_textureProvider) {
        provider = m_textureProvider;
    } else if (!provider) {
        if (node)
            node->setTextureProvider(0, false);
        delete node;
        return 0;
    }
    // The else case here is no buffer and no screenshot, so no way to show a sane image.
    // It should normally not happen, though.

    if (provider != m_textureProvider) {
        delete m_textureProvider;
        m_textureProvider = 0;
    }

    if (m_surfaceDestroyed && m_item) {
        m_item->setDelayRemove(false);
    }

    if (!provider->texture()) {
        qWarning("WindowPixmapItem does not have a source texture, cover will be dropped..");
        if (node) {
            node->setTextureProvider(0, false);
            delete node;
        }
        return 0;
    }

    if (!node) node = new SurfaceNode;

    node->setTextureProvider(provider, provider == m_textureProvider);
    node->setRect(QRectF(0, 0, width(), height()));
    node->setBlending(!m_opaque);
    node->setRadius(m_radius);
    node->setXOffset(m_xOffset);
    node->setYOffset(m_yOffset);
    node->setXScale(m_xScale);
    node->setYScale(m_yScale);

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
    Q_ASSERT(m_item == 0);

    if (c && m_id) {
        LipstickCompositorWindow *w = static_cast<LipstickCompositorWindow *>(c->windowForId(m_id));

        if (!w) {
            delete m_shaderEffect;
            m_shaderEffect = 0;
            return;
        } else if (w->surface()) {
            m_item = w;
            delete m_shaderEffect; m_shaderEffect = 0;
            m_item->setDelayRemove(true);
            connect(m_item->surface(), &QWaylandSurface::sizeChanged, this, &WindowPixmapItem::handleWindowSizeChanged);
            connect(m_item->surface(), &QWaylandSurface::configure, this, &WindowPixmapItem::configure);
            connect(m_item.data(), &QWaylandSurfaceItem::surfaceDestroyed, this, &WindowPixmapItem::surfaceDestroyed);
            m_windowSize = m_item->surface()->size();
            m_unmapLock = new QWaylandUnmapLock(m_item->surface());
        } else {
            if (!m_shaderEffect) {
                m_shaderEffect = static_cast<QQuickItem *>(c->shaderEffectComponent()->create());
                Q_ASSERT(m_shaderEffect);
                m_shaderEffect->setParentItem(this);
                m_shaderEffect->setSize(QSizeF(width(), height()));
            }

            m_shaderEffect->setProperty("window", qVariantFromValue((QObject *)w));
        }

        w->imageAddref(this);

        update();
    }
}

void WindowPixmapItem::configure(bool hasBuffer)
{
    if (hasBuffer != m_hasBuffer) {
        m_hasBuffer = hasBuffer;
        if (m_hasBuffer && !m_unmapLock)
            m_unmapLock = new QWaylandUnmapLock(m_item->surface());

        update();
    }
}

void WindowPixmapItem::cleanupOpenGL()
{
    disconnect(window(), &QQuickWindow::sceneGraphInvalidated, this, &WindowPixmapItem::cleanupOpenGL);
    delete s_snapshotProgram;
    s_snapshotProgram = 0;
}

#include "windowpixmapitem.moc"
