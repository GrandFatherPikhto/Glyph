#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>
#include <QImage>

#include "glyphmanager.h"
#include "fontmanager.h"
#include "settingsmanager.h"
#include "appsettings.h"

class AppContext : public QObject
{
    Q_OBJECT
public:
    explicit AppContext(QObject *parent = nullptr);
    ~AppContext();

    QSharedPointer<GlyphMeta> findOrCreateGlyph(const QChar &character, bool temporary = false)
    {
        Q_ASSERT(m_glyphManager != nullptr);  // Проверка в debug-режиме
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
        Q_ASSERT(m_glyphManager != nullptr);  // Проверка в debug-режиме
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
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        return m_appSettings->character();
    }

    void setCharacter(const QChar &newChar)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        m_appSettings->setCharacter(newChar);
    }

    int glyphSize()
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        return m_appSettings->glyphSize();
    }

    int bitmapDimension()
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        return m_appSettings->bitmapDimension();
    }

    void setBitmapDimension(int value)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        m_appSettings->setBitmapDimension(value);
    }

    void setGlyphSize(int value)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        m_appSettings->setGlyphSize(value);
    }

    const QFont& font()
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        return m_appSettings->font();
    }

    void setFont(const QFont &newFont)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        m_appSettings->setFont(newFont);
    }

    const QString & fontPath()
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        return m_appSettings->fontPath();
    }

    int size()
    {
        Q_ASSERT(m_glyphManager != nullptr);  // Проверка в debug-режиме
        return m_glyphManager->size();
    }

    QSharedPointer<GlyphMeta> glyphAt(int idx)
    {
        Q_ASSERT(m_glyphManager != nullptr);  // Проверка в debug-режиме
        return m_glyphManager->at(idx);
    }

    void clearDrawImage (QSharedPointer<GlyphMeta> glyphMeta) const
    {
        if (glyphMeta.isNull() || glyphMeta->layerDraw().isNull())
            return;

        QSharedPointer<QImage> image = glyphMeta->layerDraw();
        image->fill(Qt::white);
    }

signals:

public slots:

    void characterChanged(const QChar &character)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме       
        m_appSettings->setCharacter(character);
    }

    void glyphSizeChanged(int value)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме       
        m_appSettings->setGlyphSize(value);
    }

    void bitmapDimensionChanged (int value)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        m_appSettings->setBitmapDimension(value);
    }

    void fontChanged (const QFont &newFont)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме        
        m_appSettings->setFont(newFont);
    }

    void templateColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        m_appSettings->setTemplateColor(color);
    }

    void templateBgColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        m_appSettings->setTemplateBgColor(color);
    }

    void previewColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        m_appSettings->setPreviewColor(color);
    }

    void previewBgColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        m_appSettings->setPreviewBgColor(color);
    }

    void drawColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        m_appSettings->setDrawColor(color);
    }

    void drawBgColorChanged (const QColor &color)
    {
        Q_ASSERT(m_appSettings != nullptr);  // Проверка в debug-режиме
        m_appSettings->setDrawBgColor(color);
    }
    
private:
    GlyphManager *m_glyphManager;
    FontManager *m_fontManager;
    AppSettings *m_appSettings;
};

#endif // APPCONTEXT_H
