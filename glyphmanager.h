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

    QSharedPointer<GlyphMeta> findOrCreate(const QChar &character, int gridSize, const QFont font = QFont(), const QString &fontPath = QString());
    QSharedPointer<GlyphMeta> findOrCreate(const GlyphKey &key, const QFont font = QFont(), const QString &fontPath = QString());

    QSharedPointer<QImage> getTemplateGlyph(const GlyphKey &key, QSharedPointer<IGlyphRender> renderer = nullptr);
    QSharedPointer<QImage> getUserGlyph(const GlyphKey &key, QSharedPointer<IGlyphRender> renderer = nullptr);
    QSharedPointer<QImage> getPreviewGlyph(const GlyphKey &key, QSharedPointer<IGlyphRender> renderer = nullptr);
    
    void sortGlyphs();

    signals:

private:
    QVector<QSharedPointer<GlyphMeta>> m_metaGlyphs;
    QHash<GlyphKey, int> m_index;
    QHash<GlyphKey, QSharedPointer<QImage>> m_userGlyphs;
    QHash<GlyphKey, QSharedPointer<QImage>> m_templateGlyphs;
    QHash<GlyphKey, QSharedPointer<QImage>> m_previewGlyphs;

    QSharedPointer<IGlyphRender> m_ftRender;
};
#endif // GLYPHMANAGER_H
