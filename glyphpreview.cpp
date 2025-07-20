#include "glyphpreview.h"

GlyphPreview::GlyphPreview(AppContext *appContext, QWidget *parent)
    : QWidget{parent}
    , m_appContext(appContext)
    , m_glyphMeta(nullptr)
    , m_padding(10)
{
    setMinimumHeight(20);
}


void GlyphPreview::slotSetGlyphMeta (QSharedPointer<GlyphMeta> glyphMeta)
{
    m_glyphMeta = glyphMeta;
    update();
}


void GlyphPreview::paintEvent(QPaintEvent *event)
{
    if (!m_glyphMeta)
        return;

    initContext();

    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing, false);
    // qDebug() << "Glyph: " << templateGlyph->size();
    if(m_glyphMeta->layerTemplate().isNull()) 
    {
        painter.drawImage(
            m_glyphRect.left(),
            m_glyphRect.top(),
            m_glyphMeta->layerTemplate()->scaled(
                m_glyphRect.width(),
                m_glyphRect.height(),
                Qt::IgnoreAspectRatio,
                Qt::FastTransformation));
    }
    paintGrid(painter);

    painter.end();
}

void GlyphPreview::initContext ()
{
    if (!m_glyphMeta && m_glyphMeta->bitmapDimension() > 6)
        return;

    int renderSize = width() > height() ? height() - m_padding * 2 : width() - m_padding * 2;
    
    m_renderRect = QRect(
        QPoint(m_padding, m_padding),
        QSize(renderSize, renderSize)
    );

    m_cellSize = renderSize / m_glyphMeta->bitmapDimension();

    QRect glyphRect(m_glyphMeta->templateRect());
    glyphRect.translate(QPoint(0, m_glyphMeta->bitmapDimension() - glyphRect.top() * 2));
    m_glyphRect = QRect(
        QPoint(m_renderRect.left() + (glyphRect.left()) * m_cellSize,
               m_renderRect.top() + (glyphRect.top() * m_cellSize)),
        QSize(glyphRect.width() * m_cellSize, glyphRect.height() * m_cellSize)
        );

    // qDebug() << "Preview Render: " << m_renderRect << ", Glyph: " << m_glyphRect << ", Cell: " << m_cellSize;
}

void GlyphPreview::paintGrid (QPainter &painter)
{
    painter.setPen(QColor(0x33, 0x33, 0x33, 0x33));
    if (!m_glyphMeta)
        return;

    // Горизонтальные линии
    for (int y = 0; y <= m_glyphMeta->bitmapDimension(); ++y) {
        painter.drawLine(
            m_renderRect.left(),
            m_renderRect.top() + y * m_cellSize,
            m_renderRect.left() + m_glyphMeta->bitmapDimension() * m_cellSize,
            m_renderRect.top() + y * m_cellSize
            );
    }

    // Вертикальные линии
    for (int x = 0; x <= m_glyphMeta->bitmapDimension(); ++x) {
        painter.drawLine(
            m_renderRect.left() + x * m_cellSize,
            m_renderRect.top(),
            m_renderRect.left() + x * m_cellSize,
            m_renderRect.top() + m_glyphMeta->bitmapDimension() * m_cellSize
            );
    }
}

