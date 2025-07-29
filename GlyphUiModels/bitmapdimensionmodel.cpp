#include "bitmapdimensionmodel.h"

BitmapDimensionModel::BitmapDimensionModel(AppContext *appContext, QObject *parent)
    : QAbstractListModel(parent)
    , m_appContext(appContext)
{
    QObject::connect(m_appContext->glyphManager()->bitmapDimensions(), BitmapDimensions::bitmapDimensionsChanged, this, [=](){
        emit layoutChanged();
    });
}

int BitmapDimensionModel::rowCount(const QModelIndex &parent) const
{
    return m_appContext->glyphManager()->bitmapDimensions()->size();
}

QVariant BitmapDimensionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_appContext->glyphManager()->bitmapDimensions()->size())
        return QVariant();

    QSharedPointer<BitmapDimension> dim = m_appContext->glyphManager()->bitmapDimensions()->bitmapDimensionAt(index.row());
    if (dim.isNull())
        return QVariant();
        
    QString name = QString("%1x%1").arg(dim->bitmapDimension());
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << name;

    switch(role) {
    case Qt::DisplayRole:
        return name;   // Название
    case Qt::EditRole: // Добавьте эту строку
        return name;   // Название
    case Qt::UserRole:
        return dim->bitmapDimension();  // ID
    default:
        return QVariant();
    }

    return QVariant();
}
