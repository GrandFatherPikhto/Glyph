#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>
#include <QImage>

#include "glyphmanager.h"
#include "fontmanager.h"
#include "appsettings.h"
#include "unicodemetadata.h"

class AppContext : public QObject
{
    Q_OBJECT
public:
    explicit AppContext(QObject *parent = nullptr);
    ~AppContext();

    QSharedPointer<GlyphMeta> findOrCreateGlyph(const QChar &character, bool temporary = false)
    {
        Q_ASSERT(m_glyphManager != nullptr);
        return m_glyphManager->findOrCreate(
            character, 
            m_appSettings->glyphSize(), 
            m_appSettings->bitmapDimension(), 
            m_appSettings->font(), 
            m_appSettings->fontPath(), 
            temporary);
    }

    QSharedPointer<GlyphMeta> findOrCreateCurrentGlyph(bool temporary = false)
    {
        Q_ASSERT(m_glyphManager != nullptr);
        return m_glyphManager->findOrCreate(
            m_appSettings->character(), 
            m_appSettings->glyphSize(), 
            m_appSettings->bitmapDimension(), 
            m_appSettings->font(), 
            m_appSettings->fontPath(), 
            temporary);
    }

    GlyphManager * glyphManager() { return m_glyphManager; }
    FontManager * fontManager() { return m_fontManager; }
    AppSettings * appSettings() { return m_appSettings; }

    void renderGlyphLayers(QSharedPointer<GlyphMeta> glyphMeta, const QSize &previewSize)
    {
        m_glyphManager->renderGlyphLayers(glyphMeta, previewSize);
    }

    const QChar & character()
    {
        Q_ASSERT(m_appSettings != nullptr);
        return m_appSettings->character();
    }

    int glyphSize()
    {
        Q_ASSERT(m_appSettings != nullptr);
        return m_appSettings->glyphSize();
    }

    int bitmapDimension()
    {
        Q_ASSERT(m_appSettings != nullptr);
        return m_appSettings->bitmapDimension();
    }

    const QFont& font()
    {
        Q_ASSERT(m_appSettings != nullptr);
        return m_appSettings->font();
    }

    const QString & fontPath()
    {
        Q_ASSERT(m_appSettings != nullptr);
        return m_appSettings->fontPath();
    }

    int size()
    {
        Q_ASSERT(m_glyphManager != nullptr);
        return m_glyphManager->size();
    }

    QSharedPointer<GlyphMeta> glyphAt(int idx)
    {
        Q_ASSERT(m_glyphManager != nullptr);
        return m_glyphManager->at(idx);
    }

    void clearDrawImage (QSharedPointer<GlyphMeta> glyphMeta) const
    {
        if (glyphMeta.isNull() || glyphMeta->layerDraw().isNull())
            return;

        QSharedPointer<QImage> image = glyphMeta->layerDraw();
        image->fill(Qt::white);
    }

    const QColor & templateColor () 
    {
        Q_ASSERT(m_appSettings != nullptr);
        return m_appSettings->templateColor(); 
    }

    const QColor & templateBgColor () {
        Q_ASSERT(m_appSettings != nullptr);
        return m_appSettings->templateBgColor(); 
    }
    const QColor & previewColor () {
        Q_ASSERT(m_appSettings != nullptr);
        return m_appSettings->previewColor();
    }
    const QColor & previewBgColor () {
        Q_ASSERT(m_appSettings != nullptr);
        return m_appSettings->previewBgColor(); 
    }
    const QColor & drawColor() { 
        Q_ASSERT(m_appSettings != nullptr);
        return m_appSettings->drawColor(); 
    }
    const QColor & drawBgColor() 
    { 
        Q_ASSERT(m_appSettings != nullptr);
        return m_appSettings->drawBgColor(); 
    }

    QString unicodeScriptName(const QChar &ch)
    {
        Q_ASSERT(m_unicodeMetadata != nullptr);
        return m_unicodeMetadata->scriptName(ch.script());
    }

    QString unicodeScriptName(QChar::Script script)
    {
        Q_ASSERT(m_unicodeMetadata != nullptr);
        return m_unicodeMetadata->scriptName(script);
    }

    QString unicodeCategoryName(const QChar &ch)
    {
        Q_ASSERT(m_unicodeMetadata != nullptr);
        return m_unicodeMetadata->categoryName(ch.category());
    }

    QString unicodeCategoryName(QChar::Category category)
    {
        Q_ASSERT(m_unicodeMetadata != nullptr);
        return m_unicodeMetadata->categoryName(category);
    }


    QString unicodeDecompositionName (const QChar &ch)
    {
        Q_ASSERT(m_unicodeMetadata != nullptr);
        return m_unicodeMetadata->decompositionName(ch.decompositionTag());
    }

signals:

public slots:


    void characterChanged(const QChar &newChar)
    {
        Q_ASSERT(m_appSettings != nullptr);
        m_appSettings->setCharacter(newChar);
    }

    void glyphSizeChanged(int value)
    {
        Q_ASSERT(m_appSettings != nullptr);
        Q_ASSERT(m_glyphManager != nullptr);
        m_appSettings->setGlyphSize(value);
    }

    void bitmapDimensionChanged (int value)
    {
        Q_ASSERT(m_appSettings != nullptr);
        Q_ASSERT(m_glyphManager != nullptr);
        m_appSettings->setBitmapDimension(value);
    }

    void fontChanged (const QFont &newFont)
    {
        Q_ASSERT(m_appSettings != nullptr);
        Q_ASSERT(m_glyphManager != nullptr);
        m_appSettings->setFont(newFont);
    }

    void templateColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);
        Q_ASSERT(m_glyphManager != nullptr);
        m_appSettings->setTemplateColor(color);
    }

    void templateBgColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);
        Q_ASSERT(m_glyphManager != nullptr);
        m_appSettings->setTemplateBgColor(color);
    }

    void previewColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);
        Q_ASSERT(m_glyphManager != nullptr);
        m_appSettings->setPreviewColor(color);
    }

    void previewBgColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);
        Q_ASSERT(m_glyphManager != nullptr);
        m_appSettings->setPreviewBgColor(color);
    }

    void drawColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);
        Q_ASSERT(m_glyphManager != nullptr);
        m_appSettings->setDrawColor(color);
    }

    void drawBgColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);
        Q_ASSERT(m_glyphManager != nullptr);
        m_appSettings->setDrawBgColor(color);
    }
    
private:
    GlyphManager *m_glyphManager;
    FontManager *m_fontManager;
    AppSettings *m_appSettings;
    UnicodeMetadata *m_unicodeMetadata;
};

#endif // APPCONTEXT_H
