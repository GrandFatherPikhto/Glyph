#ifndef GLYPHMANAGER_H
#define GLYPHMANAGER_H

#include <QObject>
#include <QVector>
#include <QHash>

#include "glyph.h"
#include "glyphkey.h"

class GlyphManager : public QObject
{
    Q_OBJECT
public:
    explicit GlyphManager(QObject *parent = nullptr);

    QSharedPointer<Glyph> findOrCreate(const QChar &character, int gridSize);
    //int addItem(const Glyph &newGlyphEntry);
    void sortGlyphs();

    signals:

private:
    QVector<QSharedPointer<Glyph>> m_glyphs;
    QHash<GlyphKey, int> m_index;
};
#endif // GLYPHMANAGER_H
