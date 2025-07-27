#ifndef GLYPHMANAGER_H
#define GLYPHMANAGER_H

#include <QObject>
#include <QVector>
#include <QHash>
#include <QMutex>

#include <algorithm>

#include "glyphmeta.h"
#include "glyphkey.h"
#include "fontmanager.h"
#include "iglyphrenderer.h"
#include "bitmapdimension.h"

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

    explicit GlyphManager(FontManager *fontManager, QObject *parent = nullptr);
    ~GlyphManager();

    QSharedPointer<GlyphMeta> findOrCreate(const QChar &character, int bitmapDimension, int glyphSize, bool temporary = false);

    QSharedPointer<GlyphMeta> find(const GlyphKey &key);
    bool remove(const GlyphKey &key);
    bool append(QSharedPointer<GlyphMeta> glyphMeta);

    
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

    QSharedPointer<GlyphKey> getTemporaryGlyphKey ()
    {
        return m_glyphKey;
    }

    QSharedPointer<IGlyphRenderer> getRenderer (GlyphManager::ImageType type);

    void renderTemplateImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size = QSize(), QSharedPointer<IGlyphRenderer> userRenderer = QSharedPointer<IGlyphRenderer>());
    void renderPreviewImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size, QSharedPointer<IGlyphRenderer> userRenderer = QSharedPointer<IGlyphRenderer>());
    void renderDrawImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size = QSize(), QSharedPointer<IGlyphRenderer> userRenderer = QSharedPointer<IGlyphRenderer>());

    int bitmapDimensionSize () { return m_bitmapDimensionValues.size(); }
    QSharedPointer<BitmapDimension> bitmapDimensionAt(int pos);
    QSharedPointer<BitmapDimension> bitmapDimension(int value);

public slots:

signals:
    void glyphDataChanged();
    void bitmapDimensionsChanged();

private:
    const GlyphKey currentGlyphParams(const QChar &ch, int bitmapDimension, int glyphSize = -1);
    void updateData();

    bool appendBitmapDimension (int bitmapSize);
    bool removeBitmapDimension (int bitmapSize);
    void generateBitmapDimensionValues ();

    QVector<QSharedPointer<GlyphMeta>> m_metaGlyphs;
    QVector<int> m_filteredGlyphs;
    QHash<GlyphKey, int> m_index;

    QHash<int, QSharedPointer<BitmapDimension>> m_bitmapDimensions;
    QVector<int> m_bitmapDimensionValues;


    QSharedPointer<IGlyphRenderer> m_ftRender;
    QSharedPointer<IGlyphRenderer> m_drawRender;

    QSharedPointer<GlyphKey> m_glyphKey;
    QSharedPointer<GlyphMeta> m_glyphMeta;

    mutable QMutex m_mutex;    

    FontManager *m_fontManager;
};

#endif // GLYPHMANAGER_H
