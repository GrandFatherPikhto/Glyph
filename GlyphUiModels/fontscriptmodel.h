#ifndef FONTSCRIPTMODEL_H
#define FONTSCRIPTMODEL_H

#include <QAbstractListModel>
#include <QObject>

class AppContext;
class FontManager;
class UnicodeMetadata;

class FontScriptModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FontScriptModel(AppContext *appContext, QObject *parent = nullptr);

    // Переопределение методов модели
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:

    AppContext *m_appContext;
    FontManager *m_fontManager;
    UnicodeMetadata *m_unicodeMetadata;
};

#endif // FONTSCRIPTMODEL_H
