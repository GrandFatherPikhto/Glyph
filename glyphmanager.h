#ifndef GLYPHMANAGER_H
#define GLYPHMANAGER_H

#include <QObject>
#include <QVector>
#include <QHash>

#include "glyphmeta.h"
#include "glyphkey.h"
#include "iglyphrender.h"
#include "appsettings.h"

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

    explicit GlyphManager(AppSettings *applicationSettings, QObject *parent = nullptr);
    ~GlyphManager();

    QSharedPointer<GlyphMeta> findOrCreate(const QChar &character, int bitmapDimension, int glyphSize, const QFont &font = QFont(), const QString &fontPath = QString(), bool temporary = false);

    QSharedPointer<GlyphMeta> find(const GlyphKey &key);
    
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

    void renderGlyphLayers(QSharedPointer<GlyphMeta> glyphMeta, const QSize &previewSize);

    QSharedPointer<GlyphKey> getTemporaryGlyphKey ()
    {
        return m_glyphKey;
    }

    QSharedPointer<IGlyphRender> getRenderer (GlyphManager::ImageType type);

public slots:

    signals:

private:
    void updateSelected();
    void updateData();

    void renderTemplateImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size = QSize(), QSharedPointer<IGlyphRender> userRenderer = QSharedPointer<IGlyphRender>());
    void renderPreviewImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size, QSharedPointer<IGlyphRender> userRenderer = QSharedPointer<IGlyphRender>());
    void renderDrawImage (QSharedPointer<GlyphMeta> glyphMeta, const QColor &color, const QColor &bgColor, const QSize &size = QSize(), QSharedPointer<IGlyphRender> userRenderer = QSharedPointer<IGlyphRender>());

    AppSettings *m_applicationSettings;

    QVector<QSharedPointer<GlyphMeta>> m_metaGlyphs;
    QHash<GlyphKey, int> m_index;

    QSharedPointer<IGlyphRender> m_ftRender;
    QSharedPointer<IGlyphRender> m_drawRender;

    QSharedPointer<GlyphKey> m_glyphKey;
    QSharedPointer<GlyphMeta> m_glyphMeta;
};

#endif // GLYPHMANAGER_H
