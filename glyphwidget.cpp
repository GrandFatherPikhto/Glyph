#include "glyphwidget.h"
#include "ui_glyphwidget.h"

GlyphWidget::GlyphWidget(GlyphManager *glyphManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GlyphWidget)
    , m_glyphManager(glyphManager)
    , m_glyphMeta(nullptr)
    , m_glyphRectEnable(false)
    , m_templateLayerEnable(false)
    , m_gridLayerEnable(false)
    , m_previewLayerEnable(false)
    , m_userLayerEnable(false)
    , m_baselineEnable(false)
    , m_bitmapRectEnable(false)
    , m_gridDimension(-1)
    , m_bitmapDimension(-1)
{
    ui->setupUi(this);
}

GlyphWidget::~GlyphWidget()
{
    delete ui;
}

void GlyphWidget::calcRects ()
{
    if (!m_glyphMeta)
        return;

    m_bitmapDimension = m_glyphMeta->bitmapDimension();
    m_gridDimension = m_glyphMeta->gridDimension();

    if (m_gridDimension < 0)
        m_gridDimension = m_glyphMeta->bitmapDimension();

    m_gridCellSize = width() < height() ? width() / m_gridDimension : height() / m_gridDimension;

    m_padding = (m_gridDimension - m_bitmapDimension) / 2;

    m_renderRect.setLeft((width() - m_gridDimension * m_gridCellSize) / 2);
    m_renderRect.setTop((height() - m_gridDimension * m_gridCellSize) / 2);
    m_renderRect.setWidth(m_gridDimension * m_gridCellSize);
    m_renderRect.setHeight(m_gridDimension * m_gridCellSize);

    m_glyphRect.setLeft(m_renderRect.left() + (m_glyphMeta->glyphRect().left() + m_padding) * m_gridCellSize);
    m_glyphRect.setTop(m_renderRect.top() + (m_glyphMeta->glyphRect().top() + m_padding) * m_gridCellSize);
    m_glyphRect.setWidth(m_glyphMeta->glyphRect().width() * m_gridCellSize);
    m_glyphRect.setHeight(m_glyphMeta->glyphRect().height() * m_gridCellSize);

    m_bitmapRect = QRect(
        QPoint(m_renderRect.left() + m_padding * m_gridCellSize, m_renderRect.top() + m_padding * m_gridCellSize),
        QSize(m_bitmapDimension * m_gridCellSize, m_bitmapDimension * m_gridCellSize)
        );

    m_userGlyphRect = QRect(
         QPoint(m_renderRect.left() + m_padding * m_gridCellSize, m_renderRect.top() + m_padding * m_gridCellSize),
         QSize(m_glyphMeta->gridDimension() * m_gridCellSize, m_glyphMeta->gridDimension() * m_gridCellSize)
    );
}


void GlyphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (!m_glyphMeta)
        return;

    QPainter painter(this);
    // painter.scale(1.2, 1.2);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing, false);

    if (m_userLayerEnable)
    {
        paintUserGlyph(painter);
    }

    if (m_templateLayerEnable) {
        paintTemplateGlyph(painter);
    }

    if (m_previewLayerEnable)
    {
        paintPreviewGlyph(painter);
    }

    if (m_gridLayerEnable)
    {
        paintGrid(painter);
    }

    if (m_glyphRectEnable)
    {
        painter.setPen(Qt::black);
        painter.fillRect(m_glyphRect, QColor(0x33, 0, 0, 0x11));
        painter.drawRect(m_glyphRect);
    }

    if (m_baselineEnable)
    {
        paintBaseLine(painter);
    }

    if (m_bitmapRectEnable)
    {
        paintBitmapRect(painter);
    }

    m_glyphMeta->resetDirty();

    painter.end();
}

void GlyphWidget::paintBitmapRect(QPainter &painter)
{
    if (!m_glyphMeta)
        return;
    QPen pen(QColor(0x33, 0x33, 0x33, 0xEE));
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawRect(m_bitmapRect);
}

void GlyphWidget::paintGrid (QPainter &painter)
{
    if (!m_glyphMeta)
        return;

    // Горизонтальные линии
    for (int y = 0; y <= m_gridDimension; ++y) {
        painter.drawLine(
            m_renderRect.left(),
            m_renderRect.top() + y * m_gridCellSize,
            m_renderRect.left() + m_gridDimension * m_gridCellSize,
            m_renderRect.top() + y * m_gridCellSize
            );
    }

    // Вертикальные линии
    for (int x = 0; x <= m_gridDimension; ++x) {
        painter.drawLine(
            m_renderRect.left() + x * m_gridCellSize,
            m_renderRect.top(),
            m_renderRect.left() + x * m_gridCellSize,
            m_renderRect.top() + m_gridDimension * m_gridCellSize
            );
    }
}

void GlyphWidget::paintBaseLine(QPainter &painter)
{
    QPen pen(QColor(0x0, 0x0, 0xEE, 0xEF));
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawLine(
        0, //m_renderRect.left(),
        m_renderRect.top() + (m_gridDimension - m_padding) * m_gridCellSize,
        // m_renderRect.left() + m_gridDimension * m_gridCellSize,
        width(),
        m_renderRect.top() + (m_gridDimension - m_padding) * m_gridCellSize
        );

    painter.drawLine(
        m_renderRect.left() + m_padding * m_gridCellSize,
        0, //m_renderRect.top(),
        m_renderRect.left() + m_padding * m_gridCellSize,
        height()//m_renderRect.top() + m_gridDimension * m_gridCellSize
        );
}

void GlyphWidget::paintTemplateGlyph(QPainter &painter)
{
    QSharedPointer<QImage> templateGlyph =
        m_glyphManager->getTemplateGlyph(m_glyphMeta->glyphKey());

    calcRects();

    if (templateGlyph) {
        QImage scaled = templateGlyph->scaled(
            m_glyphRect.width(),
            m_glyphRect.height(),
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation);
        painter.drawImage(
            m_glyphRect.left(),
            m_glyphRect.top(),
            scaled);
    }
}

void GlyphWidget::paintPreviewGlyph(QPainter &painter)
{
    QSharedPointer<QImage> previewGlyph =
        m_glyphManager->getPreviewGlyph(m_glyphMeta->glyphKey(), QSize(width(), height()));
    if (previewGlyph)
    {
        float scaleX = static_cast<float>(previewGlyph->width()) / static_cast<float>(m_glyphMeta->previewRect().width());
        float scaleY = static_cast<float>(previewGlyph->height()) / static_cast<float>(m_glyphMeta->previewRect().height());
        QImage scaled = previewGlyph->scaled(
            m_gridCellSize * m_glyphMeta->glyphRect().width() * scaleX,
            m_gridCellSize * m_glyphMeta->glyphRect().height() * scaleY,
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation);

        painter.drawImage(m_glyphRect.left(), m_glyphRect.top(), scaled);
    }
}

void GlyphWidget::paintUserGlyph(QPainter &painter)
{
    QSharedPointer<QImage> userGlyph =
        m_glyphManager->getUserGlyph(m_glyphMeta->glyphKey());

    if (userGlyph)
    {
        QImage scaled = userGlyph->scaled(
            m_glyphRect.width(),
            m_glyphRect.height(),
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation);

        painter.drawImage(
            m_glyphRect.left(),
            m_glyphRect.top(),
            scaled);
    }
}


void GlyphWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    if (m_glyphMeta)
    {
        m_glyphMeta->setResized();
    }
}

void GlyphWidget::setGlyphMeta(QSharedPointer<GlyphMeta> newGlyphMeta)
{
    if (!m_glyphMeta || m_glyphMeta != newGlyphMeta)
    {
        m_glyphMeta = newGlyphMeta;
        m_bitmapDimension = m_glyphMeta->bitmapDimension();
    }

    update();
}

void GlyphWidget::setGridDimension(int newValue)
{
    m_gridDimension = newValue;
    update();
}


void GlyphWidget::enableGrid(bool enable)
{
    m_gridLayerEnable = enable;
    if (m_glyphMeta)
    {
        m_glyphMeta->setDirty();
    }

    update();
}

void GlyphWidget::enablePreviewLayer(bool enable)
{
    if (m_glyphMeta)
    {
        m_glyphMeta->setDirty();
    }
    // qDebug() << "Preview Layer" << enable;
    m_previewLayerEnable = enable;
    update ();
}

void GlyphWidget::enableTemplateLayer(bool enable)
{
    if (m_glyphMeta)
    {
        m_glyphMeta->setDirty();
    }
    // qDebug() << "Template Layer" << enable;
    m_templateLayerEnable = enable;
    update ();
}

void GlyphWidget::enableUserLayer(bool enable)
{
    if (m_glyphMeta)
    {
        m_glyphMeta->setDirty();
    }
    qDebug() << "User Layer" << enable;
    m_userLayerEnable = enable;
    update ();
}

void GlyphWidget::enableGlyphRectLayer(bool enable)
{
    if (m_glyphMeta)
    {
        m_glyphMeta->setDirty();
    }
    // qDebug() << "Glyph Rect Layer" << enable;
    m_glyphRectEnable = enable;
    update ();
}

void GlyphWidget::enableBitmapRectLayer(bool enable)
{
    m_bitmapRectEnable = enable;
    qDebug() << "Bitmap Rect: " << enable;
    update ();
}

void GlyphWidget::enableBaselineLayer(bool enable)
{
    m_baselineEnable = enable;
    update ();
}


void GlyphWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}
