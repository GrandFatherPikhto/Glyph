#include <QMouseEvent>

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
    , m_padding(50)
    , m_xGridCells(-1)
    , m_yGridCells(-1)
    , m_leftCells(0)
    , m_bottomCells(0)
{
    ui->setupUi(this);
}

GlyphWidget::~GlyphWidget()
{
    delete ui;
}

void GlyphWidget::initContext ()
{
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;

    if (m_xGridCells < 0) m_xGridCells = m_glyphMeta->bitmapDimension();
    if (m_yGridCells < 0) m_yGridCells = m_glyphMeta->bitmapDimension();

    int xCell = (width() - m_padding) / m_xGridCells;
    int yCell = (height() - m_padding) / m_yGridCells;
    m_gridCellSize = xCell > yCell ? yCell : xCell;

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
    QRect glyphRect(m_glyphMeta->glyphRect());
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
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;

// qDebug() << QString("%1:%2 %3 Glyph Meta Is Null %4").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__).arg(m_glyphMeta.isNull());
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;

    m_templateGlyph =
        m_glyphManager->getTemplateGlyph(m_glyphMeta->glyphKey(), QColor(0x00, 0x00, 0x55, 0x55));

    if (m_templateGlyph.isNull())
        return;

// qDebug() << QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__) << m_templateGlyph->width() << m_templateGlyph->height();

    if (m_templateGlyph) {
        // qDebug() << "Paint Template Glyph" << m_glyphMeta->character() << "Size: " << m_glyphMeta->glyphSize() << ", Rect: " << glyphRect;
        QImage scaled = m_templateGlyph->scaled(
            m_glyphRect.width(),
            m_glyphRect.height(),
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation);
        painter.drawImage(
            m_glyphRect.left(),
            m_glyphRect.top(),
            scaled);
    }

    // qDebug() << m_glyphMeta->toString();
}

void GlyphWidget::paintPreviewGlyph(QPainter &painter)
{
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;

    m_previewGlyph =
        m_glyphManager->getPreviewGlyph(m_glyphMeta->glyphKey(), QSize(width(), height()), QColor(0x66, 0x22, 0x00, 0x33));

    if (m_previewGlyph.isNull())
        return;

    float scaleX = static_cast<float>(m_previewGlyph->width()) / static_cast<float>(m_glyphMeta->previewRect().width());
    float scaleY = static_cast<float>(m_previewGlyph->height()) / static_cast<float>(m_glyphMeta->previewRect().height());
    QImage scaled = m_previewGlyph->scaled(
        m_gridCellSize * m_glyphMeta->glyphRect().width() * scaleX,
        m_gridCellSize * m_glyphMeta->glyphRect().height() * scaleY,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);

    painter.drawImage(m_glyphRect.left(), m_glyphRect.top(), scaled);
}

void GlyphWidget::paintUserGlyph(QPainter &painter)
{
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;

    m_userGlyph =
        m_glyphManager->getUserGlyph(m_glyphMeta->glyphKey(), Qt::black);

    if (m_userGlyph.isNull())
        return;
    // qDebug() << "Draw User Glyph";
    QImage scaled = m_userGlyph->scaled(
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
    initContext();
}

void GlyphWidget::setGlyphMeta(QSharedPointer<GlyphMeta> newGlyphMeta)
{
    // qDebug() << __FILE__ << __LINE__ << __FUNCION__ << "Temporary Glyph Meta" << newGlyphMeta->toString();
    if (m_glyphMeta.isNull() || m_glyphMeta != newGlyphMeta)
    {
        m_glyphMeta = newGlyphMeta;
    }

    m_templateGlyph =
        m_glyphManager->getTemplateGlyph(m_glyphMeta->glyphKey(), QColor(0x00, 0x00, 0x55, 0x55));
    m_userGlyph =
        m_glyphManager->getUserGlyph(m_glyphMeta->glyphKey(), Qt::black);
    m_previewGlyph =
        m_glyphManager->getPreviewGlyph(m_glyphMeta->glyphKey(),
            QSize(width(), height()), QColor(0x66, 0x22, 0x00, 0x33));

    // qDebug() << QString("%1:%2 %3 %4 Template Glyph Is Null %5 %6x%7").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__).arg(m_glyphMeta->toString()).arg(m_templateGlyph.isNull()).arg(m_templateGlyph->width()).arg(m_templateGlyph->width());
    initContext();
    update();
}


void GlyphWidget::enableGrid(bool enable)
{
    m_gridLayerEnable = enable;

    if (m_glyphMeta.isNull())
        return;

    m_glyphMeta->setDirty();

    update();
}

void GlyphWidget::enablePreviewLayer(bool enable)
{
    if (m_glyphMeta.isNull())
        return;
    m_glyphMeta->setDirty();
    // qDebug() << "Preview Layer" << enable;
    m_previewLayerEnable = enable;
    update ();
}

void GlyphWidget::enableTemplateLayer(bool enable)
{
    if (m_glyphMeta.isNull())
        return;
    m_glyphMeta->setDirty();
    // qDebug() << "Template Layer" << enable;
    m_templateLayerEnable = enable;
    update ();
}

void GlyphWidget::enableUserLayer(bool enable)
{
    if (m_glyphMeta.isNull())
        return;
    m_glyphMeta->setDirty();
// qDebug() << "User Layer" << enable;
    m_userLayerEnable = enable;
    update ();
}

void GlyphWidget::enableGlyphRectLayer(bool enable)
{
    if (m_glyphMeta.isNull())
        return;

    m_glyphMeta->setDirty();
    // qDebug() << "Glyph Rect Layer" << enable;
    m_glyphRectEnable = enable;
    update ();
}

void GlyphWidget::enableBitmapRectLayer(bool enable)
{
    m_bitmapRectEnable = enable;
// qDebug() << "Bitmap Rect: " << enable;
    update ();
}

void GlyphWidget::enableBaselineLayer(bool enable)
{
    m_baselineEnable = enable;
    initContext();
    update ();
}

void GlyphWidget::setLeftGridCells(int value)
{
    if (m_glyphMeta.isNull())
        return;
    m_leftCells = value;
    m_xGridCells = m_glyphMeta->bitmapDimension() + value;
    initContext();
    update ();
}

void GlyphWidget::setBottomGridCells(int value)
{
    if (m_glyphMeta.isNull())
        return;
    m_bottomCells = value;
    m_yGridCells = m_glyphMeta->bitmapDimension() + value;
    initContext();
    update ();
}

void GlyphWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid())
        return;

    QPoint clickPoint = event->pos();

    if (!m_userGlyph || m_glyphMeta.isNull() || !m_renderRect.contains(clickPoint))
        return;

    // Рассчитываем размер одного сегмента в пикселях
    const int segmentSize = m_renderRect.width() / m_glyphMeta->bitmapDimension();

    // Вычисляем координаты клика относительно m_userGlyphRect
    const QPoint relativePos = event->pos() - m_renderRect.topLeft();

    // Определяем индексы сегмента
    const int xSegment = relativePos.x() / segmentSize;
    const int ySegment = relativePos.y() / segmentSize;

    // Проверяем, чтобы индексы были в допустимых пределах
    if (xSegment >= 0 && xSegment < m_glyphMeta->bitmapDimension() &&
        ySegment >= 0 && ySegment < m_glyphMeta->bitmapDimension())
    {
        // Теперь у вас есть координаты сегмента (xSegment, ySegment)
// qDebug() << "Clicked segment:" << xSegment << ySegment;

        // Дополнительно можно получить цвет из оригинального QImage
        if (m_userGlyph->valid(xSegment, ySegment))
        {
            QRgb pixelColor = m_userGlyph->pixel(xSegment, ySegment);
            QRgb inverted = qRgba(255 - qRed(pixelColor),
                                  255 - qGreen(pixelColor),
                                  255 - qBlue(pixelColor),
                                  qAlpha(pixelColor));
// qDebug() << "Pixel color:" << QColor(pixelColor);
            m_userGlyph->setPixelColor(xSegment, ySegment, inverted);
            update();
        }
    }
}

void GlyphWidget::clearUserLayer()
{
    if (m_glyphMeta.isNull())
        return;

    QSharedPointer<QImage> userImage = m_glyphManager->getUserGlyph(m_glyphMeta->glyphKey(), Qt::black);
    if (userImage)
    {
        userImage->fill(Qt::white);
        update();
    }
}

void GlyphWidget::pasteGlyphToUserLayer()
{
    if (m_glyphMeta.isNull())
        return;

    QSharedPointer<QImage> glyphImage = m_glyphManager->getTemplateGlyph(m_glyphMeta->glyphKey(), Qt::black);
    QSharedPointer<QImage> userImage = m_glyphManager->getUserGlyph(m_glyphMeta->glyphKey(), Qt::black);
    QImage srcImg = QImage(*glyphImage.data());
    if (glyphImage && userImage)
    {
        QRect glyphRect = m_glyphMeta->paintRect();
        for (int x = 0; x < glyphImage->width(); x++)
        {
            for (int y = 0; y < glyphImage->height(); y++)
            {
                if (srcImg.pixelColor(x, y).alpha())
                {
                    srcImg.setPixelColor(x, y, QColor(0x0, 0x0, 0x0, 0xFF));
                }
            }
        }
        QPainter painter(userImage.data());
        painter.drawImage(glyphRect, srcImg);
        painter.end();
        update ();
    }
}
