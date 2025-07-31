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

class GlyphManager;
class FontManager;
class GlyphImageManager;
class UnicodeMetadata;
class AppSettings;

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
    UnicodeMetadata *unicodeMetadata () { return m_unicodeMetadata; }

    // Методы
    void saveAppContext ();
    void restoreAppContext ();

signals:

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

private:
    void initValues ();
    
    GlyphManager *m_glyphManager;
    FontManager *m_fontManager;
    GlyphImageManager *m_imageManager;
    UnicodeMetadata *m_unicodeMetadata;
    AppSettings *m_appSettings;
};

#endif // APPCONTEXT_H
