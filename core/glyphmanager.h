#ifndef GLYPHMANAGER_H
#define GLYPHMANAGER_H

#include <QObject>
#include <QVector>
#include <QHash>

#include "glyphmeta.h"
#include "glyphkey.h"
#include "fontmanager.h"
#include "iglyphrenderer.h"

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

public slots:

signals:
    void glyphDataChanged();

private:
    const GlyphKey currentGlyphParams(const QChar &ch, int bitmapDimension, int glyphSize = -1);
    void updateData();

    QVector<QSharedPointer<GlyphMeta>> m_metaGlyphs;
    QHash<GlyphKey, int> m_index;

    QSharedPointer<IGlyphRenderer> m_ftRender;
    QSharedPointer<IGlyphRenderer> m_drawRender;

    QSharedPointer<GlyphKey> m_glyphKey;
    QSharedPointer<GlyphMeta> m_glyphMeta;

    FontManager *m_fontManager;
};

#endif // GLYPHMANAGER_H
