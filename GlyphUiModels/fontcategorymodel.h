#ifndef FONTCATEGORYMODEL_H
#define FONTCATEGORYMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "appcontext.h"

class FontCategoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FontCategoryModel(AppContext *appContext, QObject *parent = nullptr);

    // Переопределение методов модели
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    void initNames ();

    QMap<QChar::Category, QString> m_names;
    AppContext *m_appContext;
};

#endif // FONTCATEGORYMODEL_H
