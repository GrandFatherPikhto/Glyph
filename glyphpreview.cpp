#include "glyphpreview.h"

GlyphPreview::GlyphPreview(GlyphManager *glyphManager, QWidget *parent)
    : QWidget{parent}
    , m_glyphManager(glyphManager)
    , m_glyphMeta(nullptr)
    , m_gridDimension(-1)
    , m_padding(0)
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

    calcRects();

    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing, false);

    QSharedPointer<QImage> templateGlyph =
        m_glyphManager->getTemplateGlyph(m_glyphMeta->glyphKey());
    // qDebug() << "Glyph: " << templateGlyph->size();
    if(templateGlyph) {
        painter.drawImage(
            m_glyphRect.left(),
            m_glyphRect.top(),
            templateGlyph->scaled(
                m_glyphRect.width(),
                m_glyphRect.height(),
                Qt::IgnoreAspectRatio,
                Qt::FastTransformation));
    }
    paintGrid(painter);

    painter.end();
}

void GlyphPreview::calcRects ()
{
    if (!m_glyphMeta && m_gridDimension >= m_glyphMeta->bitmapDimension())
        return;

    m_gridDimension = m_glyphMeta->gridDimension();

    int minSize = width() < height() ? width() : height();
    int rectSize = minSize * 4 / 5;

    m_cellSize = rectSize / m_gridDimension;
    m_padding = (m_gridDimension - m_glyphMeta->bitmapDimension()) / 2;

    m_renderRect = QRect(
        QPoint((width() - rectSize) / 2, (height() - rectSize) / 2),
        QSize(minSize, minSize)
        );
    // qDebug() << "Src Glyph Rect: " << m_glyphMeta->glyphRect() << m_glyphMeta->glyphRect().width();
    m_glyphRect = QRect(
        QPoint(m_renderRect.left() + (m_glyphMeta->glyphRect().left() + m_padding) * m_cellSize,
               m_renderRect.top() + (m_glyphMeta->glyphRect().top() + m_padding) * m_cellSize),
        QSize(m_glyphMeta->glyphRect().width() * m_cellSize, m_glyphMeta->glyphRect().height() * m_cellSize)
        );

    // qDebug() << "Preview Render: " << m_renderRect << ", Glyph: " << m_glyphRect << ", Cell: " << m_cellSize;
}

void GlyphPreview::paintGrid (QPainter &painter)
{
    painter.setPen(QColor(0x33, 0x33, 0x33, 0x33));
    if (!m_glyphMeta || m_gridDimension <= 0)
        return;

    // Горизонтальные линии
    for (int y = 0; y <= m_gridDimension; ++y) {
        painter.drawLine(
            m_renderRect.left(),
            m_renderRect.top() + y * m_cellSize,
            m_renderRect.left() + m_gridDimension * m_cellSize,
            m_renderRect.top() + y * m_cellSize
            );
    }

    // Вертикальные линии
    for (int x = 0; x <= m_gridDimension; ++x) {
        painter.drawLine(
            m_renderRect.left() + x * m_cellSize,
            m_renderRect.top(),
            m_renderRect.left() + x * m_cellSize,
            m_renderRect.top() + m_gridDimension * m_cellSize
            );
    }
}


void GlyphPreview::slotSetGridDimension(int newValue)
{
    m_gridDimension = newValue;
}
