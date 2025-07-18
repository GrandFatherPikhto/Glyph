#ifndef GLYPHMANAGER_H
#define GLYPHMANAGER_H

#include <QObject>
#include <QVector>
#include <QHash>

#include "glyphmeta.h"
#include "glyphkey.h"
#include "iglyphrender.h"

class GlyphManager : public QObject
{
    Q_OBJECT
public:
    explicit GlyphManager(QObject *parent = nullptr);
    ~GlyphManager();

    QSharedPointer<GlyphMeta> findOrCreate(const QChar &character, int bitmapDimension, int glyphSize = -1, const QFont &font = QFont(), const QString &fontPath = QString());
    QSharedPointer<GlyphMeta> find(const GlyphKey &key);

    QSharedPointer<QImage> getTemplateGlyph(const GlyphKey &key, const QColor &color /* = QColor(0x00, 0x00, 0x55, 0x55) */, QSharedPointer<IGlyphRender> renderer = QSharedPointer<IGlyphRender>());
    QSharedPointer<QImage> getUserGlyph(const GlyphKey &key, const QColor &color /* = Qt::black */, QSharedPointer<IGlyphRender> renderer = QSharedPointer<IGlyphRender>());
    QSharedPointer<QImage> getPreviewGlyph(const GlyphKey &key, const QSize &previewSize, const QColor &color /* = QColor(0x66, 0x22, 0x00, 0x33) */, QSharedPointer<IGlyphRender> renderer = QSharedPointer<IGlyphRender>());
    
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
        return m_temporaryGlyphKey;
    }

    QSharedPointer<GlyphMeta> getTemporaryGlyphMeta(const QChar &character, int bitmapDimension, int glyphSize, const QFont &font, const QString &fontPath);
    QSharedPointer<QImage> getTemporaryTemplateLayer(const QColor &color, QSharedPointer<IGlyphRender> renderer = QSharedPointer<IGlyphRender>());
    QSharedPointer<QImage> getTemporaryPreviewLayer(const QSize &size, const QColor &color, QSharedPointer<IGlyphRender> renderer = QSharedPointer<IGlyphRender>());
    QSharedPointer<QImage> getTemporaryUserLayer(const QColor &color, QSharedPointer<IGlyphRender> renderer = QSharedPointer<IGlyphRender>());


public slots:
    

    signals:

private:
    void updateSelected();
    void updateData();
    void sortGlyphs();

    QVector<QSharedPointer<GlyphMeta>> m_metaGlyphs;
    QHash<GlyphKey, int> m_index;

    QHash<GlyphKey, QSharedPointer<QImage>> m_userGlyphs;
    QHash<GlyphKey, QSharedPointer<QImage>> m_templateGlyphs;
    QHash<GlyphKey, QSharedPointer<QImage>> m_previewGlyphs;

    QSharedPointer<IGlyphRender> m_ftRender;
    QSharedPointer<IGlyphRender> m_userGlyphRender;

    QSharedPointer<GlyphKey> m_temporaryGlyphKey;
    QSharedPointer<GlyphMeta> m_temporaryGlyphMeta;
    QSharedPointer<QImage> m_temporaryTemplateLayer;
    QSharedPointer<QImage> m_temporaryPreviewLayer;
    QSharedPointer<QImage> m_temporaryUserLayer;
};
#endif // GLYPHMANAGER_H
