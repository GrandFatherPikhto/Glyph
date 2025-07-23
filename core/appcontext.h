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

#include "glyphmanager.h"
#include "fontmanager.h"
#include "unicodemetadata.h"

class AppContext : public QObject
{
    Q_OBJECT
public:
    explicit AppContext(QObject *parent = nullptr);
    ~AppContext();

    void setupSignals();

    QSharedPointer<GlyphMeta> findOrCreateGlyph(const QChar &character = QChar(), bool temporary = false)
    {
        Q_ASSERT(m_glyphManager != nullptr);
        return m_glyphManager->findOrCreate(
            character == QChar() ? m_character : character,
            m_bitmapDimension,
            m_glyphSize,
            m_font, 
            m_fontPath, 
            temporary);
    }

    GlyphManager * glyphManager() { return m_glyphManager; }
    FontManager * fontManager() { return m_fontManager; }

    // Геттеры
    const QColor & templateColor () {return m_templateColor; }
    const QColor & templateBgColor () {return m_templateBgColor; }
    const QColor & previewColor () {return m_previewColor; }
    const QColor & previewBgColor () {return m_previewBgColor; }
    const QColor & drawColor() { return m_drawColor; }
    const QColor & drawBgColor() { return m_drawBgColor; }
    int glyphSize() { return m_glyphSize; }
    int bitmapDimension () { return m_bitmapDimension; }
    const QFont & font() { return m_font; }
    const QString & fontPath () { return m_fontPath; }
    const QChar & character() { return m_character; }
    bool gridLayerEnable () { return m_gridLayerEnable; }
    bool templateLayerEnable () { return m_templateLayerEnable; }
    bool previewLayerEnable () { return m_previewLayerEnable; }
    bool drawLayerEnable () { return m_drawLayerEnable; }
    bool glyphRectLayerEnable () { return m_glyphRectLayerEnable; }
    bool baselineLayerEnable      () { return m_baselineLayerEnable; }
    bool bitmapRectLayerEnable ()  { return m_bitmapRectLayerEnable; } 
    int leftGridCells () { return m_leftGridCells; }
    int bottomGridCells () { return m_bottomGridCells; }
    const QMargins & margins() { return m_margins; }

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

    void leftGridCellsChanged   (int value);
    void bottomGridCellsChanged (int value);

    void templateColorChanged (const QColor &color);
    void templateBgColorChanged (const QColor &color);
    void previewColorChanged (const QColor &color);
    void previewBgColorChanged (const QColor &color);
    void drawColorChanged (const QColor &color);
    void drawBgColorChanged (const QColor &color);

    void gridLayerEnableChanged(bool value);
    void templateLayerEnableChanged(bool value);
    void previewLayerEnableChanged(bool value);
    void drawLayerEnableChanged(bool value);
    void glyphRectLayerEnableChanged(bool value);
    void baselineLayerEnableChanged(bool value);
    void bitmapRectLayerEnableChanged(bool value);

    void marginsChanged (const QMargins &value);

    void pasteTemplateToDrawLayer();
    void clearDrawLayer ();
    
public slots:
// Сеттеры
    void setLeftGridCells (int value) { 
        if (m_leftGridCells != value)
        {
            m_leftGridCells = value; 
            emit leftGridCellsChanged(m_leftGridCells);
        }
    }

    void setBottomGridCells (int value) { 
        if (m_bottomGridCells != value)
        {
            m_bottomGridCells = value;
            emit bottomGridCellsChanged(m_bottomGridCells);
        } 
    }

    void setTemplateColor(const QColor &color) { 
        if (m_templateColor != color)
        {
            m_templateColor = color;
            emit templateColorChanged(m_templateColor);
        }
    }

    void setTemplateBgColor(const QColor &color) {
        if( m_templateBgColor != color)
        {
            m_templateBgColor = color;
            emit templateBgColorChanged(m_templateBgColor);
        }
    }

    void setPreviewColor(const QColor &color) { 
        if (m_previewColor != color)
        {
            m_previewColor = color;
            emit previewColorChanged(m_previewColor);
        }
    }

    void setPreviewBgColor(const QColor &color) {
        if (m_previewBgColor != color)
        {
            m_previewBgColor = color;
            emit previewBgColorChanged(m_previewBgColor);
        }
    }

    void setDrawColor(const QColor &color) { 
        if (m_drawColor != color)
        {
            m_drawColor = color;
            emit drawColorChanged(m_drawColor);
        }
    }

    void setDrawBgColor(const QColor &color) {
        if (m_drawBgColor != color)
        {
            m_drawBgColor = color;
            emit drawBgColorChanged(m_drawBgColor);
        }
    }

    void setGlyphSize(int value) {
        if (m_glyphSize != value)
        {
            m_glyphSize = value;
            glyphUpdate();
        }
    }

    void setGlyphMoveLeft()
    {
        QSharedPointer<GlyphMeta> glyphMeta = findOrCreateGlyph();
        if (glyphMeta.isNull())
            return;
        glyphMeta->moveLeft();
        emit glyphChanged(glyphMeta);
    }

    void setGlyphMoveTop()
    {
        QSharedPointer<GlyphMeta> glyphMeta = findOrCreateGlyph();
        if (glyphMeta.isNull())
            return;
        glyphMeta->moveTop();
        emit glyphChanged(glyphMeta);
    }

    void setGlyphMoveDown()
    {
        QSharedPointer<GlyphMeta> glyphMeta = findOrCreateGlyph();
        if (glyphMeta.isNull())
            return;
        glyphMeta->moveDown();
        emit glyphChanged(glyphMeta);
    }

    void setGlyphMoveRight()
    {
        QSharedPointer<GlyphMeta> glyphMeta = findOrCreateGlyph();
        if (glyphMeta.isNull())
            return;
        glyphMeta->moveRight();
        emit glyphChanged(glyphMeta);
    }

    void resetGlyphOffset()
    {
        QSharedPointer<GlyphMeta> glyphMeta = findOrCreateGlyph();
        if (glyphMeta.isNull())
            return;
        glyphMeta->resetOffset();
        emit glyphChanged(glyphMeta);
    }

    void setBitmapDimension(int value) {
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

    void setFont(const QFont &newFont) { 
        Q_ASSERT(m_fontManager != nullptr);
        if (m_font != newFont)
        {
            m_font = newFont; 
            m_fontPath = m_fontManager->findFontPath(m_font);
            glyphUpdate();
        }
    }

    void setFontPath(const QString &newFontPath) { 
        if (m_fontPath != newFontPath)
        {
            m_fontPath = newFontPath;
            glyphUpdate();
        }
    }

    void setCharacter(const QChar &newChar)
    {
        if (m_character != newChar)
        {
            m_character = newChar;
            glyphUpdate();
        }
    }
    
    void setGridLayerEnable (bool value) { 
        if (m_gridLayerEnable != value)
        {
            m_gridLayerEnable = value; 
            emit gridLayerEnableChanged(m_gridLayerEnable);
        }
    }

    void setTemplateLayerEnable (bool value) {
        if (m_templateLayerEnable != value)
        {
            m_templateLayerEnable = value;
            emit templateLayerEnableChanged(m_templateLayerEnable);
        } 
    }

    void setPreviewLayerEnable (bool value) { 
        if (m_previewLayerEnable != value)
        {
            m_previewLayerEnable = value;
            emit previewLayerEnableChanged(m_previewLayerEnable);
        } 
    }

    void setDrawLayerEnable (bool value) {
        if(m_drawLayerEnable != value)
        {
            m_drawLayerEnable = value;
            emit drawLayerEnableChanged(m_drawLayerEnable);
        }
    }

    void setGlyphRectLayerEnable (bool value) { 
        if (m_glyphRectLayerEnable != value)
        {
            m_glyphRectLayerEnable = value;
            emit glyphRectLayerEnableChanged (m_glyphRectLayerEnable);
        }
    }

    void setBaselineLayerEnable (bool value) { 
        if (m_baselineLayerEnable != value)
        {
            m_baselineLayerEnable = value;
            emit baselineLayerEnableChanged(m_baselineLayerEnable);
        }
    }

    void setBitmapRectLayerEnable (bool value) {
        if (m_bitmapRectLayerEnable != value)
        {
            m_bitmapRectLayerEnable = value;
            emit bitmapRectLayerEnableChanged(m_bitmapRectLayerEnable);
        }
    }

    void renderGlyphLayers (QSharedPointer<GlyphMeta> glyphMeta, const QSize & preivewSize)
    {
        Q_ASSERT(m_glyphManager != nullptr);
        m_glyphManager->renderTemplateImage(glyphMeta, m_templateColor, m_templateBgColor);
        m_glyphManager->renderPreviewImage(glyphMeta, m_previewColor, m_previewBgColor, preivewSize);
        m_glyphManager->renderDrawImage(glyphMeta, m_drawColor, m_drawBgColor);
    }

    void setMargins(const QMargins &value)
    {
        if (m_margins != value)
        {
            m_margins = value;
            emit marginsChanged(m_margins);
        }
    }

private:

    void glyphUpdate()
    {
        if (m_character != QChar() && m_glyphSize >= 6 && m_bitmapDimension >= 6)
        {
            QSharedPointer<GlyphMeta> glyphMeta = findOrCreateGlyph();
            emit glyphChanged(glyphMeta);
        }
    }


    GlyphManager *m_glyphManager;
    FontManager *m_fontManager;
    UnicodeMetadata *m_unicodeMetadata;

    int m_glyphSize;
    int m_bitmapDimension;
    int m_leftGridCells;
    int m_bottomGridCells;

    QMargins m_margins;

    bool m_gridLayerEnable;
    bool m_templateLayerEnable;
    bool m_previewLayerEnable;
    bool m_drawLayerEnable;
    bool m_glyphRectLayerEnable;
    bool m_baselineLayerEnable;
    bool m_bitmapRectLayerEnable;

    QChar m_character;

    QFont m_font;
    QString m_fontPath;

    QColor m_templateBgColor;
    QColor m_templateColor;
    QColor m_previewBgColor;
    QColor m_previewColor;
    QColor m_drawBgColor;
    QColor m_drawColor;

};

#endif // APPCONTEXT_H
