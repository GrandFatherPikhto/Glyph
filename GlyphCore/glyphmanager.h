#ifndef GLYPHMANAGER_H_
#define GLYPHMANAGER_H_

#include <QVector>
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QSharedPointer>

#include <algorithm>

#include "glyphcontext.h"
#include "appsettings.h"
#include "glyphkey.h"
#include "fontmanager.h"
#include "bitmapdimension.h"
#include "bitmapdimensions.h"

class GlyphManager : public QObject
{
    Q_OBJECT
public:
    explicit GlyphManager(FontManager *fontManager, AppSettings *appSettings, QObject *parent = nullptr);
    ~GlyphManager();

    QSharedPointer<GlyphContext> findOrCreate(const QChar &character = QChar(), int bitmapDimension = -1, int glyphSize = -1, bool temporary = true);

signals:
    void glyphsHashChanged ();
    void glyphFontChanged (const QFont &font); 
    
private:
    void setupSignals ();
    void resetHash    ();
    void updateHash   ();
    void sortHash     ();
    
    bool append(QSharedPointer<GlyphContext> glyph);
    bool remove(const GlyphKey &key);
    QSharedPointer<GlyphContext> find(const GlyphKey &key);

    // DIs
    AppSettings *m_appSettings;
    FontManager *m_fontManager;

    QVector<QSharedPointer<GlyphContext>> m_glyphs;
    QHash<GlyphKey, int> m_index;

    BitmapDimensions *m_bitmapDimensions; //< Все размерности глифов

    QSharedPointer<GlyphContext> m_glyph; //< Текущий глиф. Используется, чтобы в findOrCreate не дёргать лишний раз хэш

    mutable QMutex m_mutex;
};


#endif // GLYPHMANAGER_H_