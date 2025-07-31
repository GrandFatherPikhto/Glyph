#include "glyphpreview.h"
#include "appcontext.h"
#include "appsettings.h"
#include "glyphmanager.h"

GlyphPreview::GlyphPreview(AppContext *appContext, QWidget *parent)
    : QWidget{parent}
    , m_appContext(appContext)
    , m_glyphManager(nullptr)
    , m_appSettings(nullptr)
    , m_glyphContext(QSharedPointer<GlyphContext>())
    , m_margins(QMargins(2,2,2,2))
{
    setMinimumHeight(20);
    initValues   ();
    setupSignals ();
}

void GlyphPreview::initValues ()
{
    Q_ASSERT(m_appContext != nullptr && m_appContext->appSettings() != nullptr && m_appContext->glyphManager() != nullptr);

    m_glyphManager = m_appContext->glyphManager ();
    m_appSettings = m_appContext->appSettings ();
}

void GlyphPreview::setupSignals ()
{
    QObject::connect(m_glyphManager, &GlyphManager::glyphChanged, this, [=](QSharedPointer<GlyphContext> glyphContext){
        m_glyphContext = glyphContext;
        update();
    });
}

void GlyphPreview::paintEvent(QPaintEvent *event)
{
    if (m_glyphContext.isNull())
        return;

    initContext();

    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing, false);
#if 0
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
#endif
    painter.end();
}

void GlyphPreview::initContext ()
{
#if 0    
    if (!m_glyphMeta && m_glyphMeta->bitmapDimension() > 6)
        return;

    int renderDimension = width() > height() ? height() : width ();
    m_cellSize = renderDimension / m_glyphMeta->bitmapDimension();
    int left = (width() - m_glyphMeta->bitmapDimension()*m_cellSize) / 2;
    int top = (height() - m_glyphMeta->bitmapDimension()*m_cellSize) / 2;
    m_renderRect = QRect(QPoint(left, top), QSize(renderDimension, renderDimension));
    m_renderRect -= m_margins;
#endif    
}

void GlyphPreview::paintGrid (QPainter &painter)
{
#if 0    
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
#endif    
}

