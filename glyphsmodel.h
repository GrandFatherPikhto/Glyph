#ifndef GLYPHSMODEL_H
#define GLYPHSMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

#include "glyphmanager.h"
#include "appcontext.h"

class GlyphsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit GlyphsModel(AppContext *appContext, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    AppContext *m_appContext;
    QStringList m_headers;
};

#endif // GLYPHSMODEL_H
