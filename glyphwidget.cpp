#include "glyphwidget.h"
#include "ui_glyphwidget.h"

GlyphWidget::GlyphWidget(GlyphManager *glyphManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GlyphWidget)
    , m_glyphManager(glyphManager)
    , m_glyphMeta(nullptr)
{
    ui->setupUi(this);
}

GlyphWidget::~GlyphWidget()
{
    delete ui;
}

void GlyphWidget::calcCellSize ()
{
    if (m_glyphMeta->gridSize())
    {
        m_gridCellSize = width() < height() ? width() / m_glyphMeta->gridSize() : height() / m_glyphMeta->gridSize();
    }
}

void GlyphWidget::calcGlyphRect ()
{
    calcCellSize();
    m_glyphRect.setLeft(m_renderRect.left() + (m_glyphMeta->glyphRect().left()) * m_gridCellSize);
    m_glyphRect.setTop(m_renderRect.top() + m_glyphMeta->glyphRect().top() * m_gridCellSize);
    m_glyphRect.setWidth(m_glyphMeta->glyphRect().width() * m_gridCellSize);
    m_glyphRect.setHeight(m_glyphMeta->glyphRect().height() * m_gridCellSize);
}

void GlyphWidget::calcRenderRect ()
{
    if (m_glyphMeta && m_glyphMeta->isValid() && width() > m_glyphMeta->gridSize() && height() > m_glyphMeta->gridSize())
    {
        calcCellSize();
        m_renderRect.setLeft((width() - m_glyphMeta->gridSize() * m_gridCellSize) / 2);
        m_renderRect.setTop((height() - m_glyphMeta->gridSize() * m_gridCellSize) / 2);
        m_renderRect.setWidth(m_glyphMeta->gridSize() * m_gridCellSize);
        m_renderRect.setHeight(m_glyphMeta->gridSize() * m_gridCellSize);

        qDebug() << __FILE__ << __LINE__ << m_glyphMeta->gridSize() << m_glyphRect << m_renderRect;
    }
}


void GlyphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (!m_glyphMeta)
        return;

    qDebug() << "Paint" << m_glyphMeta->toString();
    calcRenderRect();

    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing, false);

    if (m_templateGlyphEnable) {
        QSharedPointer<QImage> templateGlyph = m_glyphManager->getTemplateGlyph(m_glyphMeta->glyphKey());
        qDebug() << __FILE__ << __LINE__ << m_glyphMeta->toString();
        qDebug() << __FILE__ << __LINE__ << templateGlyph->width() << templateGlyph->height();
        calcGlyphRect();
        painter.drawImage(
            m_glyphRect.left(),
            m_glyphRect.top(),
            templateGlyph->scaled(m_glyphRect.width(), m_glyphRect.height(),  Qt::IgnoreAspectRatio, Qt::FastTransformation)
            );
    }

    if (m_gridEnable)
    {
        paintGrid(painter);
    }

    if (m_countourEnable)
    {
    }

    painter.end();
}

void GlyphWidget::paintGrid (QPainter &painter)
{
    if (!m_glyphMeta)
        return;

    qDebug() << "Paint Grid";
    // Горизонтальные линии
    for (int y = 0; y <= m_glyphMeta->gridSize(); ++y) {
        painter.drawLine(
            m_renderRect.left(),
            m_renderRect.top() + y * m_gridCellSize,
            m_renderRect.left() + m_glyphMeta->gridSize() * m_gridCellSize,
            m_renderRect.top() + y * m_gridCellSize
            );
    }

    // Вертикальные линии
    for (int x = 0; x <= m_glyphMeta->gridSize(); ++x) {
        painter.drawLine(
            m_renderRect.left() + x * m_gridCellSize,
            m_renderRect.top(),
            m_renderRect.left() + x * m_gridCellSize,
            m_renderRect.top() + m_glyphMeta->gridSize() * m_gridCellSize
            );
    }
}

void GlyphWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
}

void GlyphWidget::setGlyphMeta(QSharedPointer<GlyphMeta> newGliphMeta)
{
    if (!m_glyphMeta || m_glyphMeta != newGliphMeta)
    {
        m_glyphMeta = newGliphMeta;
        qDebug() << m_glyphMeta->toString();
        update();
    }
}


void GlyphWidget::enableGrid(bool enable)
{
    m_gridEnable = enable;
    update();
}

void GlyphWidget::enableContour(bool enable)
{
    m_countourEnable = enable;
}

void GlyphWidget::enableTemplateGlyph(bool enable)
{
    m_templateGlyphEnable = enable;
}

void GlyphWidget::enableGlyphGrid(bool enable)
{
    m_glyphGridEnable = enable;
}


void GlyphWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}
