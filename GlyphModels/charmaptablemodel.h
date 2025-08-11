#ifndef CHARMAPTABLEMODEL_H
#define CHARMAPTABLEMODEL_H

#include <QStringList>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "GlyphModels_global.h"

class AppContext;

class GLYPHMODELS_EXPORT CharmapTableModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit CharmapTableModel(AppContext *appContext, QObject *parent = nullptr);
    ~CharmapTableModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void refresh(const QString &query);
    QVariant getData(int row, int column) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int columnCount(const QModelIndex &parent) const override;

private:
    QStringList m_headers;
    AppContext *m_appContext;
};

#endif // CHARMAPTABLEMODEL_H
