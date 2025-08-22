#include "appcontext.h"
#include "appsettings.h"
#include "profilemanager.h"
#include "glyphdrawcontext.h"
#include "glyphmanager.h"
#include "fontmanager.h"
#include "imagemanager.h"
#include "gridcontext.h"
#include "gridmanager.h"

GlyphDrawContext::GlyphDrawContext(AppContext *appContext, QObject *parent)
    : QObject{parent}
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
    , m_profileManager(appContext->profileManager())
    , m_glyphManager(appContext->glyphManager())
    , m_imageManager(appContext->imageManager())
    , m_fontManager(appContext->fontManager())
    , m_gridManager(appContext->gridManager())
{
    setupSignals();
    // setProfile(m_profileManager->profile());
    // setGlyph(m_glyphManager->glyph());
    QMargins margins = m_appSettings->value("glyphWidgetMargins").value<QMargins>();
    setMargins(margins);
}

GlyphDrawContext::~GlyphDrawContext()
{

}

void GlyphDrawContext::setupValues()
{
    setProfile(m_profileManager->profile());
    QMargins margins = m_appSettings->value("glyphWidgetMargins").value<QMargins>();
    setMargins(margins);
}

void GlyphDrawContext::setupSignals()
{
    connect(m_profileManager, &ProfileManager::profileChanged, this, &GlyphDrawContext::setProfile);
    connect(m_gridManager, &GridManager::gridItemChanged, this, &GlyphDrawContext::setGrid);
    connect(m_glyphManager, &GlyphManager::glyphChanged, this, &GlyphDrawContext::setGlyph);
    connect(m_appSettings, &AppSettings::glyphWidgetMarginsChanged, this, &GlyphDrawContext::setMargins);
}

void GlyphDrawContext::setGlyph(const GlyphContext &glyph)
{
    // qDebug() << __FILE__ << __LINE__ << glyph;
    updateContext();
}

void GlyphDrawContext::setProfile(const ProfileContext &profile)
{
    updateContext();
}

void GlyphDrawContext::setGrid(const GridContext &grid)
{
    updateContext();
}

void GlyphDrawContext::setMargins(const QMargins &margins)
{
    m_margins = margins;
    m_drawRect -= m_margins;
}

void GlyphDrawContext::setRegion(const QRegion &region)
{
    if (region.isEmpty())
        return;
    m_region = region;

    m_drawRect = m_region.boundingRect();
    m_drawRect -= m_margins;

    updateContext();
}

bool GlyphDrawContext::renderTemplate()
{
    GlyphContext glyph = m_glyphManager->glyph();
    // qDebug() << __FILE__ << __LINE__ << glyph;

    if (!glyph.isValid())
        return false;

    FontContext font = m_fontManager->fontContext();
    GridContext grid = m_gridManager->grid();

    if (m_template.isNull())
    {
        m_template = QSharedPointer<ImageContext>::create();
    }

    m_template->setCharacter(glyph.character());
    m_template->setDirty();
    m_template->setSize(QSize(glyph.size(), glyph.size()));
    m_template->setType(ImageContext::ImageTemplate);
    m_template->setColor(m_appSettings->value("glyphWidgetTemplateColor").value<QColor>());
    m_template->setBgColor(m_appSettings->value("glyphWidgetTemplateBgColor").value<QColor>());
    m_imageManager->getGlyphImage(font, m_template);
    // qDebug() << __FILE__ << __LINE__ << "Template:" << m_template;

    QSize glyphSize(m_template->width() * cellSize(),
                    m_template->height() * cellSize());

    // ProfileContext profile = m_profileManager->glyphProfile(glyph);

    int bottom = grid.height() - m_template->top();
    m_glyphRect = QRect(bitmapTopLeft() + QPoint(m_template->left() * cellSize(), bottom * cellSize()), glyphSize);

    // qDebug() << __FILE__ << __LINE__ << m_template;

    return true;
}

bool GlyphDrawContext::renderPreview()
{
    GlyphContext glyph = m_glyphManager->glyph();

    if (!glyph.isValid())
        return false;

    FontContext font = m_fontManager->fontContext();

    if (m_preview.isNull())
    {
        m_preview = QSharedPointer<ImageContext>::create();
    }

    if (!bitmapRect().size().isValid())
        return false;

    m_preview->setCharacter(glyph.character());
    m_preview->setDirty();
    m_preview->setSize(m_region.boundingRect().size());
    m_preview->setType(ImageContext::ImagePreview);
    m_preview->setColor(m_appSettings->value("glyphWidgetPreviewColor").value<QColor>());
    m_preview->setBgColor(m_appSettings->value("glyphWidgetPreviewBgColor").value<QColor>());
    m_imageManager->getGlyphImage(font, m_preview);

    // qDebug() << __FILE__ << __LINE__ << m_preview << m_region << m_region.boundingRect().size() * 2;

    return true;
}

bool GlyphDrawContext::renderDraw()
{
    GlyphContext glyph = m_glyphManager->glyph();

    if (!glyph.isValid())
        return false;

    ProfileContext profile;
    GridContext grid = m_gridManager->grid();

    if (m_draw.isNull())
    {
        m_draw = QSharedPointer<DrawContext>::create();
    }

    profile = m_profileManager->glyphProfile(glyph);
    if (!profile.isValid())
        return false;

    m_draw->setSize(QSize(grid.width(), grid.height()));
    m_draw->setGlyphId(glyph.id());

    m_imageManager->loadOrCreateDrawImage(glyph.id(), m_draw);

    return true;    
}

ProfileContext GlyphDrawContext::glyphProfile()
{
    GlyphContext glyph = m_glyphManager->glyph();
    return m_profileManager->glyphProfile(glyph);
}

QList<QLine> GlyphDrawContext::gridHorizontalLines()
{
    QList<QLine> lines;
    GridContext grid = m_gridManager->grid();

    for(int i = 0; i <= grid.dimY(); i++)
    {
        QPoint left = cellTopLeft(0, i);
        QPoint right = cellTopLeft(grid.dimX(), i);

        lines.append(QLine(left, right));
    }

    return lines;
}

QList<QLine> GlyphDrawContext::gridVerticalLines()
{
    QList<QLine> lines;
    GridContext grid = m_gridManager->grid();

    for(int i = 0; i <= grid.dimX(); i++)
    {
        QPoint top = cellTopLeft(i, 0);
        QPoint bottom = cellTopLeft(i, grid.dimY());

        lines.append(QLine(top, bottom));
    }

    return lines;
}

int GlyphDrawContext::cellSize () {
    GridContext grid = m_gridManager->grid();
    int cellX = m_drawRect.width() / grid.dimX();
    int cellY = m_drawRect.height() / grid.dimY ();

    return cellX < cellY ? cellX : cellY;
}

QPoint GlyphDrawContext::cellTopLeft(int col, int row)
{
    int size = cellSize();
    return m_drawRect.topLeft() + QPoint(col * size, row * size);
}

QPoint GlyphDrawContext::glyphTopLeft()
{
    GridContext grid = m_gridManager->grid();
    if (!grid.isValid())
        return QPoint();

    return m_drawRect.topLeft() + QPoint(grid.left(), grid.top()) * cellSize();
}

QRect GlyphDrawContext::cellRect(int col, int row)
{
    int size = cellSize();
    return QRect(cellTopLeft(col, row), QSize(size, size));
}

QPoint GlyphDrawContext::bitmapTopLeft()
{
    GridContext grid = m_gridManager->grid();
    if (!grid.isValid())
        return QPoint();

    return m_drawRect.topLeft() + QPoint(grid.left(), grid.top()) * cellSize();
}

QRect GlyphDrawContext::bitmapRect()
{
    GridContext grid = m_gridManager->grid();

    if (!grid.isValid())
        return QRect();

    return QRect(bitmapTopLeft(), QSize(grid.width(), grid.height()) * cellSize());
}

bool GlyphDrawContext::clickGrid(const QPoint &point, int &col, int &row)
{
    row = -1;
    col = -1;

    GridContext grid = m_gridManager->grid();

    if (!m_drawRect.contains(point))
    {
        return false;
    }

    for (int x = 0; x < grid.dimX(); x ++)
    {
        for (int y = 0; y < grid.dimY(); y ++)
        {
            QRect rect = cellRect(x, y);
            if (rect.contains(point))
            {
                col = x;
                row = y;
                return true;
            }
        }
    }

    return false;
}

int GlyphDrawContext::baseLineY()
{
    GlyphContext glyph = m_glyphManager->glyph();
    GridContext grid = m_gridManager->grid();

    if (!grid.isValid() || !glyph.isValid())
        return -1;
        
    int baseline = grid.top() + grid.height() - glyph.baseline();

    QPoint pointBaseline = cellTopLeft(0, baseline);

    return pointBaseline.y();
}

int GlyphDrawContext::lineLeftX()
{
    GlyphContext glyph = m_glyphManager->glyph();
    GridContext grid = m_gridManager->grid();

    if (!grid.isValid() || !glyph.isValid())
        return -1;

    int leftline = grid.left() + glyph.offsetLeft();
    QPoint pointLeftLine = cellTopLeft(leftline, 0);        
    return pointLeftLine.x();
}

bool GlyphDrawContext::clickBitmap(const QPoint &point, int &col, int &row)
{
    row = -1;
    col = -1;

    GridContext grid = m_gridManager->grid();

    if (!grid.isValid())
        return false;

    if (!bitmapRect().contains(point))
    {
        return false;
    }

    for (int x = 0; x < grid.dimX(); x ++)
    {
        for (int y = 0; y < grid.dimY(); y ++)
        {
            QRect rect = cellRect(x + grid.left(), y + grid.top());
            if (rect.contains(point))
            {
                col = x;
                row = y;

                return true;
            }
        }
    }

    return false;
}

QSharedPointer<QImage> GlyphDrawContext::templateImage () const
{
    if (m_template.isNull())
        return QSharedPointer<QImage>();
    return m_template->image();
}

QSharedPointer<QImage> GlyphDrawContext::previewImage () const
{
    if (m_preview.isNull())
        return QSharedPointer<QImage>();
    return m_preview->image();
}

QSharedPointer<QImage> GlyphDrawContext::drawImage () const
{
    if (m_draw.isNull())
        return QSharedPointer<QImage>();
    return m_draw->image();
}

void GlyphDrawContext::updateContext()
{
    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    renderTemplate();
    renderPreview();
    renderDraw();
}
