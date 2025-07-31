#ifndef GLYPHMANAGER_H_
#define GLYPHMANAGER_H_

#include <QVector>
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QSharedPointer>

#include <algorithm>

#include "appsettings.h"
#include "glyphcontext.h"
#include "appsettings.h"
#include "glyphkey.h"
#include "fontmanager.h"
#include "bitmapdimension.h"
#include "bitmapdimensions.h"

class AppContext;

class GlyphManager : public QObject
{
    Q_OBJECT
public:
    explicit GlyphManager(AppContext *appContext);
    ~GlyphManager();

    QSharedPointer<GlyphContext> findOrCreate(const QChar &character = QChar(), int bitmapDimension = -1, int glyphSize = -1, bool temporary = true);

    BitmapDimensions * bitmapDimensions() { return m_bitmapDimensions; }

    // Геттеры-обёртки
    const QChar character ();

    QSharedPointer<GlyphContext> glyphAt(int pos);
    QSharedPointer<GlyphContext> filteredAt(int pos);

    void setCharacterFilter (const QString &filter);

    int filteredSize ();

signals:
    void glyphsHashChanged ();
    void glyphFontChanged (const QFont &font); 
    void glyphCreated (QSharedPointer<GlyphContext> context);
    void glyphChanged (QSharedPointer<GlyphContext> context);

    void changeCharacter(const QChar &character, bool temporary);
    void changeGlyphSize(int glyphSize);
    void changeCurrentGlyphByFilteredPos(int pos);

    void glyphOffsetReset ();
    void glyphOffsetLeft  ();
    void glyphOffsetUp    ();
    void glyphOffsetRight ();
    void glyphOffsetDown  ();
    
private:
    void setupSignals ();
    void resetHash    ();
    void updateHash   ();
    void sortHash     ();
    void filterGlyphs ();
    
    bool append(QSharedPointer<GlyphContext> glyph);
    bool remove(const GlyphKey &key);
    QSharedPointer<GlyphContext> find(const GlyphKey &key);

    // DIs
    AppSettings *m_appSettings;
    FontManager *m_fontManager;

    QVector<QSharedPointer<GlyphContext>> m_glyphs;
    QVector<QSharedPointer<GlyphContext>> m_filtered;
    QHash<GlyphKey, int> m_index;

    QString m_characterFilter;

    BitmapDimensions *m_bitmapDimensions; //< Все размерности глифов

    QSharedPointer<GlyphContext> m_glyph; //< Текущий глиф. Используется, чтобы в findOrCreate не дёргать лишний раз хэш

    mutable QMutex m_mutex;
};


#endif // GLYPHMANAGER_H_