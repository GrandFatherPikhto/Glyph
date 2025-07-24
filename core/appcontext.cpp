#include "appcontext.h"

AppContext::AppContext(QObject *parent)
    : QObject{parent}
    , m_glyphManager(nullptr)
    , m_fontManager(nullptr)
    , m_unicodeMetadata(nullptr)
    , m_templateColor(QColor(0x00, 0x00, 0x55, 0x55))
    , m_templateBgColor(Qt::white)
    , m_previewColor(QColor(0x66, 0x22, 0x00, 0x33))
    , m_previewBgColor(Qt::white)
    , m_drawColor(Qt::gray)
    , m_drawBgColor(Qt::white)
    , m_fontPath(QString(""))
    , m_font(QFont())
    , m_glyphSize(-1)
    , m_bitmapDimension(-1)
    , m_character(QChar())
    , m_gridLayerEnable(false)
    , m_templateLayerEnable(false)
    , m_previewLayerEnable(false)
    , m_drawLayerEnable(false)
    , m_glyphRectLayerEnable(false)
    , m_baselineLayerEnable(false)
    , m_bitmapRectLayerEnable(false)
    , m_leftGridCells(0)
    , m_bottomGridCells(0)
    , m_margins(QMargins(0, 0, 0, 0))
{
    m_fontManager = new FontManager(this);
    m_glyphManager = new GlyphManager(this);
    m_unicodeMetadata = new UnicodeMetadata (this);

    restoreAppContext();

    setupSignals();
}

AppContext::~AppContext()
{
    saveAppContext();
}

void AppContext::setupSignals()
{
    QObject::connect(this, &AppContext::clearDrawLayer, this, [=](){
        QSharedPointer<GlyphMeta> glyphMeta = findOrCreateGlyph();
        if (glyphMeta.isNull() || glyphMeta->layerDraw().isNull())
            return;
        
        glyphMeta->layerDraw()->fill(m_drawBgColor);
        emit glyphDrawChanged(glyphMeta);
    });

    QObject::connect(this, &AppContext::pasteTemplateToDrawLayer, this, [=](){
        qDebug() << "Paste Template to User Layer";
        QSharedPointer<GlyphMeta> glyphMeta = findOrCreateGlyph();
        if (glyphMeta.isNull() || glyphMeta->layerTemplate().isNull() || glyphMeta->layerDraw().isNull())
            return;

        QImage srcImg = QImage(*(glyphMeta->layerTemplate().data()));
        QRect glyphRect = glyphMeta->paintRect();
        for (int x = 0; x < glyphMeta->layerTemplate()->width(); x++)
        {
            for (int y = 0; y < glyphMeta->layerTemplate()->height(); y++)
            {
                if (srcImg.pixelColor(x, y).alpha())
                {
                    srcImg.setPixelColor(x, y, QColor(0x0, 0x0, 0x0, 0xFF));
                }
            }
        }
        QPainter painter(glyphMeta->layerDraw().data());
        painter.drawImage(glyphRect, srcImg);
        painter.end();
        emit glyphDrawChanged(glyphMeta);
    });
}

void AppContext::saveAppContext()
{
    QSettings settings;

    settings.beginGroup("ApplicationSettings");
    
    settings.setValue("glyph/template/color", m_templateColor);
    settings.setValue("glyph/template/bgColor", m_templateBgColor);
    settings.setValue("glyph/preview/color", m_previewColor);
    settings.setValue("glyph/preview/bgColor", m_previewBgColor);
    settings.setValue("glyph/draw/color", m_drawColor);
    settings.setValue("glyph/draw/bgColor", m_drawBgColor);

    settings.setValue("glyph/size", m_glyphSize);
    settings.setValue("glyph/bitmapDimension", m_bitmapDimension);
    settings.setValue("glyph/font", m_font);
    settings.setValue("glyph/fontPath", m_fontPath);
    settings.setValue("glyph/character", m_character.unicode());

    settings.setValue("glyph/gridLayerEnable", m_gridLayerEnable);
    settings.setValue("glyph/templateLayerEnable", m_templateLayerEnable);
    settings.setValue("glyph/previewLayerEnable", m_previewLayerEnable);
    settings.setValue("glyph/drawLayerEnable", m_drawLayerEnable);
    settings.setValue("glyph/glyphRectLayerEnable", m_glyphRectLayerEnable);
    settings.setValue("glyph/baselineLayerEnable", m_baselineLayerEnable);
    settings.setValue("glyph/bitmapRectLayerEnable", m_bitmapRectLayerEnable);

    settings.setValue("glyph/leftGridCells", m_leftGridCells);
    settings.setValue("glyph/bottomGridCells", m_bottomGridCells);

    settings.setValue("glyph/margins/left", m_margins.left());
    settings.setValue("glyph/margins/top", m_margins.top());
    settings.setValue("glyph/margins/right", m_margins.right());
    settings.setValue("glyph/margins/bottom", m_margins.bottom());

    settings.endGroup();
}

void AppContext::restoreAppContext()
{
    QSettings settings;

    settings.beginGroup("ApplicationSettings");

    QColor templateColor = settings.value("glyph/template/color").value<QColor>();
    if (templateColor.isValid())
        m_templateColor = templateColor;

    QColor templateBgColor = settings.value("glyph/template/bgColor").value<QColor>();
    if (templateBgColor.isValid())
        m_templateBgColor = templateBgColor;

    QColor previewColor = settings.value("glyph/preview/color").value<QColor>();
    if (previewColor.isValid())
       m_previewColor = previewColor;

    QColor previewBgColor = settings.value("glyph/preview/bgColor").value<QColor>();
    if (previewBgColor.isValid())
        m_previewBgColor = previewBgColor;

    QColor drawColor = settings.value("glyph/draw/color").value<QColor>();
    if (drawColor.isValid())
        m_drawColor = drawColor;

    QColor drawBgColor = settings.value("glyph/draw/bgColor").value<QColor>();
    if (drawBgColor.isValid())
        m_drawBgColor = drawBgColor;

    m_glyphSize = settings.value("glyph/size").toInt();
    m_bitmapDimension = settings.value("glyph/bitmapDimension").toInt();
    m_fontPath = settings.value("glyph/fontPath").toString();
    
    int unicode = settings.value("glyph/character").toInt();
    if (unicode >= 0)
    {
        m_character = QChar(unicode);
    }

    m_gridLayerEnable = settings.value("glyph/gridLayerEnable").toBool();
    m_templateLayerEnable = settings.value("glyph/templateLayerEnable").toBool();
    m_previewLayerEnable = settings.value("glyph/previewLayerEnable").toBool();
    m_drawLayerEnable = settings.value("glyph/drawLayerEnable").toBool();
    m_glyphRectLayerEnable = settings.value("glyph/glyphRectLayerEnable").toBool();
    m_baselineLayerEnable = settings.value("glyph/baselineLayerEnable").toBool();
    m_bitmapRectLayerEnable = settings.value("glyph/bitmapRectLayerEnable").toBool();

    m_leftGridCells = settings.value("glyph/leftGridCells", 0).toInt();
    m_bottomGridCells = settings.value("glyph/bottomGridCells", 0).toInt();
    m_margins = QMargins(
        settings.value("glyph/margins/left", 50).toInt(),
        settings.value("glyph/margins/top", 50).toInt(),
        settings.value("glyph/margins/right", 50).toInt(),
        settings.value("glyph/margins/bottom", 50).toInt()
        );


    settings.endGroup();
}

void AppContext::renderGlyphLayers (QSharedPointer<GlyphMeta> glyphMeta, const QSize & preivewSize)
{
    Q_ASSERT(m_glyphManager != nullptr);
    if (glyphMeta.isNull())
        return;

    if (glyphMeta->isDirty())
    {
        m_glyphManager->renderTemplateImage(glyphMeta, m_templateColor, m_templateBgColor);
        m_glyphManager->renderDrawImage(glyphMeta, m_drawColor, m_drawBgColor);
        emit glyphRendered(glyphMeta);
        emit glyphDrawChanged(glyphMeta);
    }

    if (glyphMeta->isResized())
    {
        m_glyphManager->renderPreviewImage(glyphMeta, m_previewColor, m_previewBgColor, preivewSize);
        emit glyphPreviewRendered(glyphMeta);
    }
}
