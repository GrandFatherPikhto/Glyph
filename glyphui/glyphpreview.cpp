#include "glyphpreview.h"

GlyphPreview::GlyphPreview(AppContext *appContext, QWidget *parent)
    : QWidget{parent}
    , m_appContext(appContext)
    , m_glyphMeta(nullptr)
    , m_margins(QMargins(2,2,2,2))
{
    setMinimumHeight(20);

    QObject::connect(m_appContext, &AppContext::glyphDrawChanged, this, [=](QSharedPointer<GlyphMeta> glyphMeta){
        m_glyphMeta = glyphMeta;
        update();
    });

    QObject::connect(m_appContext, &AppContext::glyphChanged, this, [=](QSharedPointer<GlyphMeta> glyphMeta){
        if (!glyphMeta.isNull() && !glyphMeta->layerDraw().isNull())
        {
            m_glyphMeta = glyphMeta;
            update();
        }
    });
}

void GlyphPreview::paintEvent(QPaintEvent *event)
{
    if (!m_glyphMeta || m_glyphMeta->layerDraw().isNull())
        return;

    initContext();

    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing, false);

    if(!m_glyphMeta->layerDraw().isNull())
    {
        QSize scaleSize(m_glyphMeta->drawRect().size());
        scaleSize *= m_cellSize;
        // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_glyphMeta->toString() << m_glyphMeta->layerDraw().isNull() << scaleSize << m_cellSize;
        painter.drawImage(
            m_renderRect.topLeft(),
            m_glyphMeta->layerDraw()->scaled(
                scaleSize,
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

    int renderDimension = width() > height() ? height() : width ();
    m_cellSize = renderDimension / m_glyphMeta->bitmapDimension();
    int left = (width() - m_glyphMeta->bitmapDimension()*m_cellSize) / 2;
    int top = (height() - m_glyphMeta->bitmapDimension()*m_cellSize) / 2;
    m_renderRect = QRect(QPoint(left, top), QSize(renderDimension, renderDimension));
    m_renderRect -= m_margins;
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

