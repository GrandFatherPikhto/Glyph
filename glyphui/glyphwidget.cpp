#include <QMouseEvent>

#include "glyphwidget.h"
#include "ui_glyphwidget.h"

#include "glyphcontext.h"
#include "appcontext.h"
#include "glyphmanager.h"
#include "appsettings.h"

GlyphWidget::GlyphWidget(AppContext *appContext, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GlyphWidget)
    , m_appContext(appContext)
    , m_glyphContext(QSharedPointer<GlyphContext>())
    // , m_glyphRectEnable(false)
    // , m_templateLayerEnable(false)
    // , m_gridLayerEnable(false)
    // , m_previewLayerEnable(false)
    // , m_drawLayerEnable(false)
    // , m_baselineEnable(false)
    // , m_bitmapRectEnable(false)
    // , m_margins(QMargins(0,0,0,0))
    // , m_xGridCells(-1)
    // , m_yGridCells(-1)
    // , m_leftCells(0)
    // , m_bottomCells(0)
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
    Q_ASSERT(m_appContext != nullptr && m_appContext->glyphManager() != nullptr && m_appContext->appSettings() != nullptr);
    m_glyphManager = m_appContext->glyphManager();
    m_appSettings = m_appContext->appSettings();
    m_margins = m_appSettings->glyphWidgetMargins();
    // m_glyphRectEnable = m_appContext->appSettings()->glyphRectLayerEnable();
    // m_templateLayerEnable = m_appContext->appSettings()->templateLayerEnable();
    // m_gridLayerEnable = m_appContext->appSettings()->gridLayerEnable();
    // m_previewLayerEnable = m_appContext->appSettings()->previewLayerEnable();
    // m_drawLayerEnable = m_appContext->appSettings()->drawLayerEnable();
    // m_baselineEnable = m_appContext->appSettings()->baselineLayerEnable();
    // m_bitmapRectEnable = m_appContext->appSettings()->bitmapRectLayerEnable();
}

void GlyphWidget::setupSignals()
{
#if 0    
    QObject::connect(m_appContext->appSettings(), &AppSettings::gridPaddingsChanged, this, [=](const GridPaddings &paddings){
        m_gridDimensions.setPaddings(paddings);
        update ();
    });

    QObject::connect(m_appContext->appSettings(), &AppSettings::templateLayerEnableChanged, this, [=](bool value){
        m_templateLayerEnable = value;
        update();
    });

    QObject::connect(m_appContext->appSettings(), &AppSettings::previewLayerEnableChanged, this, [=](bool value){
        m_previewLayerEnable = value;
        update();
    });

    QObject::connect(m_appContext->appSettings(), &AppSettings::drawLayerEnableChanged, this, [=](bool value){
        m_drawLayerEnable = value;
        update();
    });

    QObject::connect(m_appContext->appSettings(), &AppSettings::gridLayerEnableChanged, this, [=](bool value){
        m_gridLayerEnable = value;
        update ();
    });

    QObject::connect(m_appContext->appSettings(), &AppSettings::baselineLayerEnableChanged, this, [=](bool value){
        m_baselineEnable = value;
        update();
    });

    QObject::connect(m_appContext->appSettings(), &AppSettings::bitmapRectLayerEnableChanged, this, [=](bool value){
        m_bitmapRectEnable = value;
        update ();
    });

    QObject::connect(m_appContext->appSettings(), &AppSettings::glyphRectLayerEnableChanged, this, [=](bool value){
        m_glyphRectEnable = value;
        update ();
    });

    QObject::connect(m_appContext, &AppContext::clearDrawLayer, this, [=](){
        update ();
    });

    QObject::connect(m_appContext, &AppContext::pasteTemplateToDrawLayer, this, [=](){
        update ();
    });
#endif    
}

void GlyphWidget::initContext ()
{
    if (m_glyphContext.isNull())
        return;
/*
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
*/    
}


void GlyphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (m_glyphContext.isNull())
        return;

    QPainter painter(this);
    // painter.scale(1.2, 1.2);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing, false);

    if (m_appSettings->drawLayerEnable())
    {
        paintDrawGlyph(painter);
    }

    if (m_appSettings->templateLayerEnable()) {
        paintTemplateGlyph(painter);
    }

    if (m_appSettings->previewLayerEnable())
    {
        paintPreviewGlyph(painter);
    }

    if (m_appSettings->glyphRectLayerEnable())
    {
        // painter.setPen(Qt::black);
        // painter.fillRect(m_glyphRect, QColor(0x33, 0, 0, 0x11));
        // painter.drawRect(m_glyphRect);
    }

    if (m_appSettings->baselineLayerEnable())
    {
        paintBaseLines(painter);
    }

    if (m_appSettings->bitmapRectLayerEnable())
    {
        paintBitmapRect(painter);
    }

    if (m_appSettings->gridLayerEnable())
    {
        paintGrid(painter);
    }

    painter.end();
}

void GlyphWidget::paintBitmapRect(QPainter &painter)
{
    if (m_glyphContext.isNull())
        return;
    QPen pen(QColor(0x33, 0x33, 0x33, 0xEE));
    pen.setWidth(3);
    painter.setPen(pen);
    // painter.drawRect(m_gridDimensions.widgetBitmapRect(size()).translated(m_margins.left(), m_margins.top()));
}

void GlyphWidget::paintGrid (QPainter &painter)
{
    if (m_glyphContext.isNull())
        return;

    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
#if 0
    int cellSize = m_gridDimensions.widgetCellSize(size());

    // Горизонтальные линии
    for (int y = 0; y <= m_gridDimensions.rows(); ++y) {
        painter.drawLine(
            m_margins.left(),
            m_margins.top() + y * cellSize,
            m_margins.left() + m_gridDimensions.rows() * cellSize,
            m_margins.top() + y * cellSize
            );
    }

    // Вертикальные линии
    for (int x = 0; x <= m_gridDimensions.cols(); ++x) {
        painter.drawLine(
            m_margins.left() + x * cellSize,
            m_margins.top(),
            m_margins.left() + x * cellSize,
            m_margins.top() + m_gridDimensions.rows() * cellSize
            );
    }
#endif    
}

void GlyphWidget::paintBaseLines(QPainter &painter)
{
#if 0    
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
    int cellSize = m_gridDimensions.widgetCellSize(size());
    // Baseline
    painter.drawLine(
        0,
        m_margins.top() + (glyphRect.top()) * m_gridCellSize,
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
#endif        
}

void GlyphWidget::paintTemplateGlyph(QPainter &painter)
{
#if 0    
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
#endif        
}

void GlyphWidget::paintPreviewGlyph(QPainter &painter)
{
#if 0    
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;

    if (m_glyphMeta->layerPreview().isNull())
        return;
    QImage scaled = m_glyphMeta->layerPreview()->scaled(
        m_glyphMeta->templateSize(m_gridCellSize),
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);
    painter.drawImage(m_glyphRect.left(), m_glyphRect.top(), scaled);
#endif    
}

void GlyphWidget::paintDrawGlyph(QPainter &painter)
{
#if 0    
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
#endif        
}


void GlyphWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
#if 0    
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;

    m_glyphMeta->setResized();
    m_appContext->renderGlyphLayers(m_glyphMeta, QSize(width(), height()));

    initContext();
#endif    
}

void GlyphWidget::setGlyphMeta(QSharedPointer<GlyphContext> newGlyphMeta)
{
#if 0    
    if (newGlyphMeta.isNull())
        return;
    if (m_glyphMeta.isNull() || m_glyphMeta != newGlyphMeta)
    {
        m_glyphMeta = newGlyphMeta;
    }

    m_appContext->renderGlyphLayers(m_glyphMeta, QSize(width(), height()));

    initContext();
    update();
#endif    
}


void GlyphWidget::mousePressEvent(QMouseEvent *event)
{
#if 0    
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

        emit m_appContext->glyphDrawChanged(m_glyphMeta);
    }
#endif    
}

