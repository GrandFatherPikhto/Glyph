#ifndef GLYPHMODEL_H_
#define GLYPHMODEL_H_

#include <QObject.h>
#include <QSqlQueryModel>
#include <QStringList>

#include "GlyphModels_global.h"

class AppContext;

class GLYPHMODELS_EXPORT GlyphModel: public QSqlQueryModel {
    Q_OBJECT
public:
    explicit GlyphModel(AppContext *appContext, QObject *parent = nullptr);
    ~GlyphModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant getData(int row, int column) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int columnCount(const QModelIndex &parent) const override;

private:
    AppContext *m_appContext;

    QStringList m_headers;
};

#endif // GLYPHMODEL_H_