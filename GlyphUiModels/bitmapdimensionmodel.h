#ifndef BITMAPDIMENSIONMODEL_H
#define BITMAPDIMENSIONMODEL_H

#include <QAbstractListModel>

#include "appcontext.h"

class GlyphManager;
class AppContext;

class BitmapDimensionModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BitmapDimensionModel(AppContext *appContext, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    AppContext *m_appContext;
    GlyphManager * m_glyphManager;
    DimensionManager * m_dimensionManager;
};

#endif // BITMAPDIMENSIONMODEL_H
