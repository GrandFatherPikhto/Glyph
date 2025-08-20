#include <QPainter>
#include <QWidget>
#include <QPen>

#include "glyphdraw.h"

#include "appcontext.h"
#include "appsettings.h"
#include "glyphmanager.h"
#include "profilemanager.h"
#include "imagemanager.h"
#include "fontmanager.h"

#include "glyphcontext.h"

#include "glyphdrawcontext.h"

GlyphDraw::GlyphDraw(QWidget *parent)
    : QWidget{parent}
    , m_drawContext(nullptr)
    , m_appSettings(nullptr)
    , m_profileManager(nullptr)
    , m_imageManager(nullptr)
    , m_fontManager(nullptr)
{

}

GlyphDraw::~GlyphDraw()
{

}

void GlyphDraw::setAppContext(AppContext *appContext)
{
    m_appContext = appContext;
    m_appSettings = m_appContext->appSettings();
    m_glyphManager = m_appContext->glyphManager();
    m_profileManager = m_appContext->profileManager();
    m_imageManager = m_appContext->imageManager();
    m_fontManager = m_appContext->fontManager();

    m_drawContext = new GlyphDrawContext(m_appContext, this);

    setProfile(m_profileManager->profile());
    setGlyph(m_glyphManager->glyph());

    setupSignals();
}

void GlyphDraw::setupSignals()
{
    QObject::connect(m_glyphManager, &GlyphManager::glyphChanged, this, &GlyphDraw::setGlyph);
    QObject::connect(m_profileManager, &ProfileManager::profileChanged, this, &GlyphDraw::setProfile);
}

void GlyphDraw::paintEvent(QPaintEvent *event)
{
    m_drawContext->setRegion(event->region());

    QPainter painter(this);

    drawGrid(painter);
    drawBitmapRect(painter);
    drawBaseLine(painter);
    drawLeftLine(painter);
    drawTemplate(painter);
    drawPreview(painter);

    painter.end();
}

void GlyphDraw::setGlyph(const GlyphContext &glyph)
{
    update();
}

void GlyphDraw::setProfile(const ProfileContext &context)
{
    m_profile = context;
    update();
}

void GlyphDraw::drawGrid(QPainter &painter)
{
    QColor color = m_appSettings->value("glyphWidgetGridColor").value<QColor>();

    QPen gridPen(color);
    gridPen.setWidth(m_appSettings->value("glyphWidgetGridLineWidth").toInt());
    painter.setPen(gridPen);
    QList<QLine> horizontalLines = m_drawContext->gridHorizontalLines();
    if (horizontalLines.size())
        painter.drawLines(horizontalLines.constData(), horizontalLines.size());
    QList<QLine> verticalLines = m_drawContext->gridVerticalLines();
    if (verticalLines.size())
        painter.drawLines(verticalLines.constData(), verticalLines.size());
}

void GlyphDraw::drawBitmapRect(QPainter &painter)
{
    QColor color = m_appSettings->value("glyphWidgetBitmapRectColor").value<QColor>();
    QPen pen(color);
    pen.setWidth(m_appSettings->value("glyphWidgetBitmapRectLineWidth").toInt());
    pen.setStyle(static_cast<Qt::PenStyle>(m_appSettings->value("glyphWidgetBitmapRectLineStyle").toInt()));
    painter.setPen(pen);
    painter.drawRect(m_drawContext->bitmapRect());
}

void GlyphDraw::drawBaseLine(QPainter &painter)
{
    QColor color = m_appSettings->value("glyphWidgetBaseLineColor").value<QColor>();
    QPen pen(color);
    pen.setWidth(m_appSettings->value("glyphWidgetBaseLineWidth").toInt());
    pen.setStyle(static_cast<Qt::PenStyle>(m_appSettings->value("glyphWidgetBaseLineStyle").toInt()));
    painter.setPen(pen);
    int baselineY = m_drawContext->baseLineY();
    painter.drawLine(QPoint(0, baselineY), QPoint(size().width(), baselineY));
}

void GlyphDraw::drawLeftLine(QPainter &painter)
{
    QColor color = m_appSettings->value("glyphWidgetLeftLineColor").value<QColor>();
    QPen pen(color);
    pen.setWidth(m_appSettings->value("glyphWidgetLeftLineWidth").toInt());
    pen.setStyle(static_cast<Qt::PenStyle>(m_appSettings->value("glyphWidgetLeftLineStyle").toInt()));
    painter.setPen(pen);
    int leftlineX = m_drawContext->lineLeftX();
    painter.drawLine(QPoint(leftlineX, 0), QPoint(leftlineX, size().height()));
}

void GlyphDraw::drawTemplate(QPainter &painter)
{
    QSharedPointer<QImage> img = m_drawContext->imageTemplate();
    if (img.isNull())
        return;

    painter.drawImage(m_drawContext->glyphRect(), *(img.data()));
}

void GlyphDraw::drawPreview(QPainter &painter)
{
    QSharedPointer<QImage> img = m_drawContext->imagePreview();
    if (img.isNull())
        return;

    painter.drawImage(m_drawContext->glyphRect(), *(img.data()));
}


void GlyphDraw::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}


void GlyphDraw::mousePressEvent(QMouseEvent *event)
{
    int col, row;
    m_drawContext->clickBitmap(event->pos(), col, row);
    qDebug() << __FILE__ << __LINE__ << col << row;
}
