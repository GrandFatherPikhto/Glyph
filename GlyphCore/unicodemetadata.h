#ifndef UNICODEMETADATAMANAGER_H
#define UNICODEMETADATAMANAGER_H

#include <QMap>
#include <QString>
#include <QObject>

#include "GlyphCore_global.h"

class AppContext;

class GLYPHCORE_EXPORT UnicodeMetadata : public QObject
{
    Q_OBJECT
public:
    explicit UnicodeMetadata (AppContext *appContext) ;

    // Получение названий
    QString scriptName(const QChar &character) { return scriptName(character.script()); }
    QString scriptName(QChar::Script script) const;
    QString categoryName(const QChar &character) { return categoryName(character.category()); }
    QString categoryName(QChar::Category category) const;
    QString decompositionName(const QChar &character) { return decompositionName(character.decompositionTag()); }
    QString decompositionName(QChar::Decomposition decomposition) const;

    // Полные списки
    const QMap<QChar::Script, QString>& allScripts() const;
    const QMap<QChar::Category, QString>& allCategories() const;
    const QMap<QChar::Decomposition, QString>& allDecompositions() const;

private:

    void loadScripts();
    void loadCategories();
    void loadDecompositions();

    // DI
    AppContext *m_appContext;

    QMap<QChar::Script, QString> m_scripts;
    QMap<QChar::Category, QString> m_categories;
    QMap<QChar::Decomposition, QString> m_decompositions;
};

#endif // UNICODEMETADATAMANAGER_H
