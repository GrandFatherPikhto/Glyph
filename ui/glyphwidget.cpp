#include <QMouseEvent>

#include "glyphwidget.h"
#include "ui_glyphwidget.h"


GlyphWidget::GlyphWidget(AppContext *appContext, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GlyphWidget)
    , m_appContext(appContext)
    , m_glyphMeta(QSharedPointer<GlyphMeta>())
    , m_glyphRectEnable(false)
    , m_templateLayerEnable(false)
    , m_gridLayerEnable(false)
    , m_previewLayerEnable(false)
    , m_drawLayerEnable(false)
    , m_baselineEnable(false)
    , m_bitmapRectEnable(false)
    , m_margins(QMargins(0,0,0,0))
    , m_xGridCells(-1)
    , m_yGridCells(-1)
    , m_leftCells(0)
    , m_bottomCells(0)
{
    ui->setupUi(this);
    initValues();
    setupSignals();
}

GlyphWidget::~GlyphWidget()
{
    delete ui;
}

void GlyphWidget::initValues()
{
    Q_ASSERT(m_appContext != nullptr);

    m_glyphRectEnable = m_appContext->glyphRectLayerEnable();
    m_templateLayerEnable = m_appContext->templateLayerEnable();
    m_gridLayerEnable = m_appContext->gridLayerEnable();
    m_previewLayerEnable = m_appContext->previewLayerEnable();
    m_drawLayerEnable = m_appContext->drawLayerEnable();
    m_baselineEnable = m_appContext->baselineLayerEnable();
    m_bitmapRectEnable = m_appContext->bitmapRectLayerEnable();
    m_xGridCells = m_appContext->leftGridCells();
    m_leftCells = m_appContext->leftGridCells();
    m_bottomCells = m_appContext->bottomGridCells ();
    m_margins = m_appContext->margins();

    if (!m_glyphMeta.isNull())
    {
        m_xGridCells = m_glyphMeta->bitmapDimension() + m_appContext->leftGridCells();
        m_yGridCells = m_glyphMeta->bitmapDimension() + m_appContext->bottomGridCells();
    }
}

void GlyphWidget::setupSignals()
{
    QObject::connect(m_appContext, &AppContext::leftGridCellsChanged, this, [=](int value){
        m_leftCells = value;
        initContext ();
        update ();
    });

    QObject::connect(m_appContext, &AppContext::bottomGridCellsChanged, this, [=](int value){
        m_bottomCells = value;
        initContext();
        update ();
    });

    QObject::connect(m_appContext, &AppContext::templateLayerEnableChanged, this, [=](bool value){
        m_templateLayerEnable = value;
        update();
    });

    QObject::connect(m_appContext, &AppContext::previewLayerEnableChanged, this, [=](bool value){
        m_previewLayerEnable = value;
        update();
    });

    QObject::connect(m_appContext, &AppContext::drawLayerEnableChanged, this, [=](bool value){
        m_drawLayerEnable = value;
        update();
    });

    QObject::connect(m_appContext, &AppContext::gridLayerEnableChanged, this, [=](bool value){
        m_gridLayerEnable = value;
        update ();
    });

    QObject::connect(m_appContext, &AppContext::baselineLayerEnableChanged, this, [=](bool value){
        m_baselineEnable = value;
        update();
    });

    QObject::connect(m_appContext, &AppContext::bitmapRectLayerEnableChanged, this, [=](bool value){
        m_bitmapRectEnable = value;
        update ();
    });

    QObject::connect(m_appContext, &AppContext::glyphRectLayerEnableChanged, this, [=](bool value){
        m_glyphRectEnable = value;
        update ();
    });

    QObject::connect(m_appContext, &AppContext::clearDrawLayer, this, [=](){
        update ();
    });

    QObject::connect(m_appContext, &AppContext::pasteTemplateToDrawLayer, this, [=](){
        update ();
    });
}

void GlyphWidget::initContext ()
{
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;

    m_leftCells = m_appContext->leftGridCells();
    m_bottomCells = m_appContext->bottomGridCells ();

    m_xGridCells = m_glyphMeta->bitmapDimension() + m_appContext->leftGridCells();
    m_yGridCells = m_glyphMeta->bitmapDimension() + m_appContext->bottomGridCells();

    QRect drawRect = rect() - m_appContext->margins();

    int cellWidth = drawRect.width() / m_xGridCells;
    int cellHeight = drawRect.height() / m_yGridCells;
    m_gridCellSize = width() > height() ? cellHeight : cellWidth;

    int xGridDimension = m_gridCellSize * m_xGridCells;
    int yGridDimension = m_gridCellSize * m_yGridCells;

    int renderDimension = m_gridCellSize * m_glyphMeta->bitmapDimension();

    // Сетка с учётом, что она может быть увеличена
    QPoint ltGrid ((width() - xGridDimension) / 2, (height() - yGridDimension) / 2);
    m_gridRect = QRect(
        ltGrid,
        QSize(xGridDimension, yGridDimension)
    );

    QPoint ltRender(m_gridRect.left() + m_leftCells * m_gridCellSize, m_gridRect.top());
    m_renderRect = QRect(
        ltRender,
        QSize(renderDimension, renderDimension)
        );

    QRect glyphRect = m_glyphMeta->paintRect();
    m_glyphRect = QRect(
        ltRender + QPoint(0, glyphRect.top() * m_gridCellSize),
        QSize(glyphRect.width()*m_gridCellSize, glyphRect.height()*m_gridCellSize)
    );
}


void GlyphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (m_glyphMeta.isNull())
        return;

    QPainter painter(this);
    // painter.scale(1.2, 1.2);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing, false);

    if (m_drawLayerEnable)
    {
        paintDrawGlyph(painter);
    }

    if (m_templateLayerEnable) {
        paintTemplateGlyph(painter);
    }

    if (m_previewLayerEnable)
    {
        paintPreviewGlyph(painter);
    }

    if (m_glyphRectEnable)
    {
        painter.setPen(Qt::black);
        painter.fillRect(m_glyphRect, QColor(0x33, 0, 0, 0x11));
        painter.drawRect(m_glyphRect);
    }

    if (m_baselineEnable)
    {
        paintBaseLines(painter);
    }

    if (m_bitmapRectEnable)
    {
        paintBitmapRect(painter);
    }

    if (m_gridLayerEnable)
    {
        paintGrid(painter);
    }

    m_glyphMeta->resetDirty();
    m_glyphMeta->resetResized();

    painter.end();
}

void GlyphWidget::paintBitmapRect(QPainter &painter)
{
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;
    QPen pen(QColor(0x33, 0x33, 0x33, 0xEE));
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawRect(m_renderRect);
}

void GlyphWidget::paintGrid (QPainter &painter)
{
    if (m_glyphMeta.isNull())
        return;

    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    // Горизонтальные линии
    for (int y = 0; y <= m_yGridCells; ++y) {
        painter.drawLine(
            m_gridRect.left(),
            m_gridRect.top() + y * m_gridCellSize,
            m_gridRect.left() + m_xGridCells * m_gridCellSize,
            m_gridRect.top() + y * m_gridCellSize
            );
    }

    // Вертикальные линии
    for (int x = 0; x <= m_xGridCells; ++x) {
        painter.drawLine(
            m_gridRect.left() + x * m_gridCellSize,
            m_gridRect.top(),
            m_gridRect.left() + x * m_gridCellSize,
            m_gridRect.top() + m_yGridCells * m_gridCellSize
            );
    }
}

void GlyphWidget::paintBaseLines(QPainter &painter)
{
    if (m_glyphMeta.isNull())
        return;

    QPen penHorz(QColor(0xEE, 0x55, 0x55, 0xEE));
    penHorz.setWidth(2);
    painter.setPen(penHorz);
    // Горизонтальная линия (baseline)
    QRect glyphRect(m_glyphMeta->templateRect());
    int disp = glyphRect.top() - glyphRect.height();
    glyphRect.translate(QPoint(0, m_glyphMeta->bitmapDimension() - glyphRect.top()));
    if (disp < 0)
        glyphRect.translate(QPoint(0, disp));
    glyphRect.translate(m_glyphMeta->offset());
    // Baseline
    painter.drawLine(
        0,
        m_renderRect.top() + (glyphRect.top()) * m_gridCellSize,
        width(),
        m_renderRect.top() + (glyphRect.top()) * m_gridCellSize
        );

    QPen penVert(QColor(0x55, 0xFF, 0x55, 0xEE));
    penVert.setWidth(2);
    painter.setPen(penVert);
    painter.drawLine(
        m_renderRect.left() - glyphRect.left() * m_gridCellSize,
        0,
        m_renderRect.left() - glyphRect.left() * m_gridCellSize,
        height()
        );
}

void GlyphWidget::paintTemplateGlyph(QPainter &painter)
{
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid() || m_glyphMeta->layerTemplate().isNull())
        return;

    QImage scaled = m_glyphMeta->layerTemplate()->scaled(
        m_glyphMeta->templateSize(m_gridCellSize),
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation);

    painter.drawImage(
        m_glyphRect.left(),
        m_glyphRect.top(),
        scaled);
}

void GlyphWidget::paintPreviewGlyph(QPainter &painter)
{
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;

    if (m_glyphMeta->layerPreview().isNull())
        return;
    QImage scaled = m_glyphMeta->layerPreview()->scaled(
        m_glyphMeta->templateSize(m_gridCellSize),
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);
    painter.drawImage(m_glyphRect.left(), m_glyphRect.top(), scaled);
}

void GlyphWidget::paintDrawGlyph(QPainter &painter)
{
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid() || m_glyphMeta->layerDraw().isNull())
        return;

    // qDebug() << "Draw Glyph Layer";
    QImage scaled = m_glyphMeta->layerDraw()->scaled(
        m_renderRect.size(),
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation);

    painter.drawImage(
        m_renderRect.topLeft(),
        scaled);
}


void GlyphWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;

    m_glyphMeta->setResized();
    m_appContext->renderGlyphLayers(m_glyphMeta, QSize(width(), height()));

    initContext();
}

void GlyphWidget::setGlyphMeta(QSharedPointer<GlyphMeta> newGlyphMeta)
{
    if (newGlyphMeta.isNull())
        return;
    if (m_glyphMeta.isNull() || m_glyphMeta != newGlyphMeta)
    {
        m_glyphMeta = newGlyphMeta;
    }

    m_glyphMeta->setDirty();
    m_glyphMeta->setResized();
    m_appContext->renderGlyphLayers(m_glyphMeta, QSize(width(), height()));

    initContext();
    update();
}


void GlyphWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid() || m_glyphMeta->layerTemplate().isNull() || m_glyphMeta->layerDraw().isNull())
        return;

    qDebug() << __FILE__ << __LINE__ << event->pos();

    QPoint clickPoint = event->pos();

    if (!m_renderRect.contains(clickPoint))
        return;

    // Вычисляем координаты клика относительно m_userGlyphRect
    const QPoint relativePos = event->pos() - m_renderRect.topLeft();

    // Определяем индексы сегмента
    const int xSegment = relativePos.x() / m_gridCellSize;
    const int ySegment = relativePos.y() / m_gridCellSize;

    // Проверяем, чтобы индексы были в допустимых пределах
    if (xSegment >= 0 && xSegment < m_glyphMeta->bitmapDimension() &&
        ySegment >= 0 && ySegment < m_glyphMeta->bitmapDimension())
    {
        if (m_glyphMeta->layerDraw()->valid(xSegment, ySegment))
        {
            QRgb pixelColor = m_glyphMeta->layerDraw()->pixel(xSegment, ySegment);
            QColor newColor = (QColor(pixelColor) == m_appContext->drawColor()) ? m_appContext->drawBgColor() : m_appContext->drawColor();
            m_glyphMeta->layerDraw()->setPixelColor(xSegment, ySegment, newColor);
            update();
        }
    }
}

