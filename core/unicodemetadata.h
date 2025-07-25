#ifndef UNICODEMETADATAMANAGER_H
#define UNICODEMETADATAMANAGER_H

#include <QMap>
#include <QString>
#include <QObject>

class UnicodeMetadata : public QObject
{
    Q_OBJECT
public:
    explicit UnicodeMetadata (QObject *parent = nullptr) ;

    // Получение названий
    QString scriptName(QChar::Script script) const;
    QString categoryName(QChar::Category category) const;
    QString decompositionName(QChar::Decomposition decomposition) const;

    // Полные списки
    const QMap<QChar::Script, QString>& allScripts() const;
    const QMap<QChar::Category, QString>& allCategories() const;
    const QMap<QChar::Decomposition, QString>& allDecompositions() const;

private:

    void loadScripts();
    void loadCategories();
    void loadDecompositions();

    QMap<QChar::Script, QString> m_scripts;
    QMap<QChar::Category, QString> m_categories;
    QMap<QChar::Decomposition, QString> m_decompositions;
};

#endif // UNICODEMETADATAMANAGER_H
