#ifndef GLYPHMANAGER_H
#define GLYPHMANAGER_H

#include <QObject>
#include <QVector>
#include <QHash>
#include <QMutex>

#include <algorithm>

#include "appsettings.h"
#include "glyphmeta.h"
#include "glyphkey.h"
#include "fontmanager.h"
#include "iglyphrenderer.h"
#include "bitmapdimension.h"
#include "bitmapdimensions.h"

class GlyphManager : public QObject
{
    Q_OBJECT
public:

enum ImageType {
        ImageUser,
        ImageTemplate,
        ImageDraw,
        ImagePreview
    };

    explicit GlyphManager(FontManager *fontManager, AppSettings *appSettings, QObject *parent = nullptr);
    ~GlyphManager();

    QSharedPointer<GlyphMeta> findOrCreate(const QChar &character = QChar(), int bitmapDimension = -1, int glyphSize = -1, bool temporary = false);

    QSharedPointer<GlyphMeta> find(const GlyphKey &key);
    bool remove(const GlyphKey &key);
    bool append(QSharedPointer<GlyphMeta> glyphMeta);

    void setBitmapDimension(int value);
    void setGlyphSize(int value);

    int bitmapDimension() { return m_bitmapDimension; }
    int glyphSize() {return m_glyphSize; }

    GlyphKey glyphKey ()
    {
        if (m_glyphMeta.isNull())
            return GlyphKey();
        return m_glyphMeta->key();
    }

    void sort();

    int size()
    {
        return m_metaGlyphs.size();
    }

    QSharedPointer<GlyphMeta> at(int index) {
        if (index < m_metaGlyphs.size())
        {
            return m_metaGlyphs.at(index);
        }

        return nullptr;
    }

    QSharedPointer<IGlyphRenderer> getRenderer (ImageType type);

    BitmapDimensions * bitmapDimensions() { return m_bitmapDimensions; }

    void renderTemplateImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size = QSize(), QSharedPointer<IGlyphRenderer> userRenderer = QSharedPointer<IGlyphRenderer>());
    void renderPreviewImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size, QSharedPointer<IGlyphRenderer> userRenderer = QSharedPointer<IGlyphRenderer>());
    void renderDrawImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size = QSize(), QSharedPointer<IGlyphRenderer> userRenderer = QSharedPointer<IGlyphRenderer>());

    QSharedPointer<GlyphMeta> execClearDrawLayer (const QColor &bgColor = QColor());
    QSharedPointer<GlyphMeta> execPasteTemplateToDrawLayer(const QColor &color = QColor(), const QColor &bgColor = QColor());
    void execRenderGlyphLayers (const QSize &size);

    void setGlyphOffsetMoveLeft();
    void setGlyphOffsetMoveTop();
    void setGlyphOffsetMoveRight();
    void setGlyphOffsetMoveDown();
    void setGlyphOffsetReset();

public slots:

signals:
    void glyphDataChanged ();
    void glyphChanged (QSharedPointer<GlyphMeta> glyph);
    void layerDrawChanged(QSharedPointer<GlyphMeta> glyph);
    void renderGlyphLayers(const QSize &size /** size Preview Layer */);
    void glyphTemplateLayerRendered(QSharedPointer<GlyphMeta> glyph);
    void glyphDrawLayerRendered(QSharedPointer<GlyphMeta> glyph);
    void glyphPreviewLayerRendered(QSharedPointer<GlyphMeta> glyph);

    void pasteTemplateToDrawLayer ();
    void clearDrawLayer ();

    void glyphOffsetLeft ();
    void glyphOffsetTop ();
    void glyphOffsetRight ();
    void glyphOffsetDown ();
    void glyphOffsetReset ();

private:
    const GlyphKey currentGlyphParams(const QChar &ch, int bitmapDimension, int glyphSize = -1);
    void filterGlyphs();
    void updateData();
    void setupSignals ();

    QVector<QSharedPointer<GlyphMeta>> m_metaGlyphs;
    QVector<int> m_filteredGlyphs;
    QHash<GlyphKey, int> m_index;

    QSharedPointer<IGlyphRenderer> m_ftRender;
    QSharedPointer<IGlyphRenderer> m_drawRender;

    QSharedPointer<GlyphMeta> m_glyphMeta;

    mutable QMutex m_mutex;    

    FontManager *m_fontManager;
    BitmapDimensions *m_bitmapDimensions;

    int m_bitmapDimension;
    int m_glyphSize;
    QChar m_character;
    
    AppSettings *m_appSettings;
};

#endif // GLYPHMANAGER_H
