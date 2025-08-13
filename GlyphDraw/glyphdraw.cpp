#include <QPainter>
#include <QWidget>
#include <QPen>

#include "glyphdraw.h"

#include "appcontext.h"
#include "appsettings.h"
#include "glyphmanager.h"
#include "profilemanager.h"

#include "glyphcontext.h"

#include "drawcontext.h"

GlyphDraw::GlyphDraw(QWidget *parent)
    : QWidget{parent}
    , m_drawContext(nullptr)
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

    m_drawContext = new DrawContext(m_appContext, this);

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

    painter.end();
}

void GlyphDraw::setGlyph(const GlyphContext &context)
{
    m_glyph = context;
    // qDebug() << __FILE__ << __LINE__ << m_glyph;
}

void GlyphDraw::setProfile(const ProfileContext &context)
{
    m_profile = context;
    // qDebug() << __FILE__ << __LINE__ << m_profile;
    update();
}

void GlyphDraw::drawGrid(QPainter &painter)
{
    QColor color = m_appSettings->gridColor();
    // QColor bgColor = m_appSettings->gridBgColor();

    QPen gridPen(color);
    gridPen.setWidth(m_appSettings->widgetGridLineWidth());
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
    QColor color = m_appSettings->bitmapRectColor();
    QPen pen(color);
    pen.setWidth(m_appSettings->widgetBitmapRectLineWidth());
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    // qDebug() << __FILE__ << __LINE__ << pen;
    painter.drawRect(m_drawContext->bitmapRect());
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
