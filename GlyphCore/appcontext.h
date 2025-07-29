#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>
#include <QImage>
#include <QSettings>
#include <QColor>
#include <QPoint>
#include <QRect>
#include <QString>
#include <QMargins>
#include <QPainter>

#include "glyphmanager.h"
#include "fontmanager.h"
#include "unicodemetadata.h"
#include "appsettings.h"

class AppContext : public QObject
{
    Q_OBJECT
public:
    explicit AppContext(QObject *parent = nullptr);
    ~AppContext();

    void setupSignals();

    GlyphManager * glyphManager() { return m_glyphManager; }
    FontManager * fontManager() { return m_fontManager; }
    AppSettings * appSettings() { return m_appSettings; }

    const QFont & glyphFont() { return m_fontManager->glyphFont(); }
    const QString & fontPath () { return m_fontManager->glyphFontPath(); }
    int fontMSB() { return m_fontMSB; }
    const QVector<QChar::Category> & fontCategories () { return m_fontManager->fontCategories(); }
    const QVector<QChar::Script> & fontScripts () { return m_fontManager->fontScripts(); }
    const QVector<QChar::Decomposition> & fontDecompositions () { return m_fontManager->fontDecompositions(); }

    // Обёртки
    int glyphTableSize()
    {
        Q_ASSERT(m_glyphManager != nullptr);
        return m_glyphManager->size();
    }

    QSharedPointer<GlyphMeta> glyphAt(int pos)
    {
        Q_ASSERT(m_glyphManager != nullptr);
        return m_glyphManager->at(pos);
    }

    QString unicodeScriptName(const QChar &ch)
    {
        return unicodeScriptName(ch.script());
    }

    QString unicodeScriptName(QChar::Script value)
    {
        Q_ASSERT(m_unicodeMetadata != nullptr);
        return m_unicodeMetadata->scriptName(value);
    }

    QString unicodeCategoryName(const QChar &ch)
    {
        return unicodeCategoryName(ch.category());
    }

    QString unicodeCategoryName(QChar::Category value)
    {
        Q_ASSERT(m_unicodeMetadata != nullptr);
        return m_unicodeMetadata->categoryName(value);
    }

    // Методы
    void saveAppContext ();
    void restoreAppContext ();

signals:
    void glyphChanged(QSharedPointer<GlyphMeta> glyphMeta);
    void glyphRendered(QSharedPointer<GlyphMeta> glyphMeta);
    void glyphPreviewRendered(QSharedPointer<GlyphMeta> glyphMeta);
    void glyphDrawChanged(QSharedPointer<GlyphMeta> glyphMeta);

    void leftGridCellsChanged   (int value);
    void bottomGridCellsChanged (int value);

    void fontMSBChanged(int value);
    void fontChanged (const QFont &newFont);
    void fontCategoriesChanged (const QVector<QChar::Category> &value);
    void fontScriptsChanged (const QVector<QChar::Script> &value);
    void fontDecompositionsChanged (const QVector<QChar::Decomposition> &value);

    void pasteTemplateToDrawLayer();
    void clearDrawLayer ();
    
public slots:
// Сеттеры
/*     void setGlyphSize(int value) {
        if (m_glyphSize != value)
        {
            m_glyphSize = value;
            glyphUpdate();
        }
    }

 */
/*     void setBitmapDimension(int value) {
        if (m_bitmapDimension != value)
        {
            m_bitmapDimension = value;
            QSharedPointer<GlyphMeta> glyphMeta = findOrCreateGlyph();
            if (glyphMeta.isNull())
            {
                return;
            }
            glyphUpdate();
        }
    }
 */
    void setGlyphFont(const QFont &newFont) { 
        Q_ASSERT(m_fontManager != nullptr);
        m_fontManager->setGlyphFont(newFont);
    }

/*     void setCharacter(const QChar &newChar, bool temporary = false)
    {
        // if (m_character != newChar)
        {
            m_character = newChar;
            glyphUpdate(temporary);
        }
    }
 */
        

    void renderGlyphLayers (QSharedPointer<GlyphMeta> glyphMeta, const QSize & preivewSize);

/*     void setMargins(const QMargins &value)
    {
        if (m_margins != value)
        {
            m_margins = value;
            emit marginsChanged(m_margins);
        }
    }
 */
private:


/*     void glyphUpdate(bool temporary = true)
    {
        if (m_character != QChar() && m_glyphSize >= 6 && m_bitmapDimension >= 6)
        {
            QSharedPointer<GlyphMeta> glyphMeta = findOrCreateGlyph(m_character, temporary);
            emit glyphChanged(glyphMeta);
        }
    }
 */

    GlyphManager *m_glyphManager;
    FontManager *m_fontManager;
    UnicodeMetadata *m_unicodeMetadata;
    AppSettings *m_appSettings;

    // QChar m_character;
    int m_fontMSB;


};

#endif // APPCONTEXT_H
