#ifndef GLYPHMANAGER_H_
#define GLYPHMANAGER_H_

#include <QVector>
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QSharedPointer>

#include "GlyphCore_global.h"

class AppContext;
class AppSettings;
class GlyphContext;
class GlyphKey;
class GlyphFilter;
class FontManager;
class BitmapDimension;
class DimensionManager;

class GLYPHCORE_EXPORT GlyphManager : public QObject
{
    Q_OBJECT
public:
    explicit GlyphManager(AppContext *appContext);
    ~GlyphManager();

    QSharedPointer<GlyphContext> findOrCreate(const QChar &character = QChar(), int bitmapDimension = -1, int glyphSize = -1, bool temporary = true);

    void setDimensionManager(DimensionManager * dimensionManager);
    void setFontManager(FontManager * fontManager);
    void setGlyphFilter(GlyphFilter * glyphFilter);
    void setAppSettings(AppSettings * appSettings);

    // Геттеры-обёртки
    const QChar character ();

    QSharedPointer<GlyphContext> glyphAt(int pos);
    void removeGlyphsByCharacter(const QChar &ch);

    std::shared_ptr<QVector<QSharedPointer<GlyphContext>>> glyphs();

signals:
    void glyphsHashChanged ();
    void glyphFontChanged (const QFont &font); 
    void glyphCreated (QSharedPointer<GlyphContext> context);
    void glyphChanged (QSharedPointer<GlyphContext> context);

    void changeCharacter(const QChar &character, bool temporary);
    void changeGlyphSize(int glyphSize);

    void glyphOffsetReset ();
    void glyphOffsetLeft  ();
    void glyphOffsetUp    ();
    void glyphOffsetRight ();
    void glyphOffsetDown  ();
    
private:

    void setupDiValues  ();
    void setupSignals ();
    void resetHash    ();
    void updateHash   ();
    void sortHash     ();

    
    bool append(QSharedPointer<GlyphContext> glyph);
    bool remove(const GlyphKey &key);
    QSharedPointer<GlyphContext> find(const GlyphKey &key);

    bool changeGlyph(const QChar &ch, bool temporary);

    // DIs
    AppContext *m_appContext;
    AppSettings * m_appSettings;
    FontManager * m_fontManager;
    DimensionManager * m_dimensionManager; //< Все размерности глифов
    GlyphFilter * m_glyphFilter;

    std::shared_ptr<QVector<QSharedPointer<GlyphContext>>> m_glyphs;
    QHash<GlyphKey, int> m_index;

    QSharedPointer<GlyphContext> m_glyph; //< Текущий глиф. Используется, чтобы в findOrCreate не дёргать лишний раз хэш

    mutable QMutex m_mutex;
};


#endif // GLYPHMANAGER_H_
