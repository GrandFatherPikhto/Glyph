#include "fontmodel.h"
#include "appcontext.h"

FontModel::FontModel(AppContext *appContext, QObject *parent)
    : QSqlQueryModel{parent}
    , m_appContext(appContext)
    , m_headers({"Id", "Family", "Style", "System", "File", "Path"})
{

}

FontModel::~FontModel()
{}

int FontModel::columnCount(const QModelIndex &parent) const
{
    return m_headers.size();
}

QVariant FontModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if (section >= m_headers.size())
            return QSqlQueryModel::headerData(section, orientation, role);

        return m_headers.at(section);
    }

    return QSqlQueryModel::headerData(section, orientation, role);
}

QVariant FontModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    return QSqlQueryModel::data(index, role);
}

Qt::ItemFlags FontModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QSqlQueryModel::flags(index);

    return flags;
}

int FontModel::idByRow(int row)
{
    QModelIndex idIdx = QSqlQueryModel::index(row, 0);
    qDebug() << __FILE__ << __LINE__ << idIdx;
    if (!idIdx.isValid())
        return -1;

    return data(idIdx).toInt();
}
