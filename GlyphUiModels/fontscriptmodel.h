#ifndef FONTSCRIPTMODEL_H
#define FONTSCRIPTMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "appcontext.h"

class FontScriptModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FontScriptModel(AppContext *appContext, QObject *parent = nullptr);

    // Переопределение методов модели
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    void initNames ();

    AppContext *m_appContext;
    QMap<QChar::Script, QString> m_names;
};

#endif // FONTSCRIPTMODEL_H
