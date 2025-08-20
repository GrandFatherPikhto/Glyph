#ifndef UNICODEMETADATAMANAGER_H
#define UNICODEMETADATAMANAGER_H

#include <QMap>
#include <QString>
#include <QObject>
#include <QSqlQuery>

#include "GlyphCore_global.h"

class AppContext;
class DbManager;

class GLYPHCORE_EXPORT UnicodeMetadata : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        ScriptType = 0x01,
        CategoryType = 0x02,
        DecompositionType = 0x03
    } UnicodeMetadataType;

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

signals:
    

private:
    void setupValues ();
    void setupSignals ();
    
    bool initTable (const QString &tableName);
    bool fillScriptTable();
    bool fillCategoryTable();
    bool fillDecompositionTable();
    void loadScripts();
    void loadCategories();
    void loadDecompositions();

    // DI
    AppContext *m_appContext;
    DbManager *m_dbManager;

    QMap<QChar::Script, QString> m_scripts;
    QMap<QChar::Category, QString> m_categories;
    QMap<QChar::Decomposition, QString> m_decompositions;

    QString m_scriptTable;
    QString m_categoryTable;
    QString m_decompositionTable;
};

#endif // UNICODEMETADATAMANAGER_H
