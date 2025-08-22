#include "profilesmodel.h"

#include "appcontext.h"
#include "profilemanager.h"

ProfilesModel::ProfilesModel(AppContext *appContext, QObject *parent)
    : QSqlQueryModel(parent)
    , m_appContext(appContext)
    , m_profileManager(appContext->profileManager())
{
    
}

int ProfilesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return QSqlQueryModel::rowCount(parent) + 1;
}

QVariant ProfilesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() == 0)
    {
        // qDebug() << __FILE__ << __LINE__ << index << m_profileManager->defaultProfile();
        switch(role)
        {
            case Qt::DisplayRole:
            switch(index.column()) {
                case 0:
                    return m_profileManager->defaultProfile().id();
                case 1:
                    return m_profileManager->defaultProfile().name();
                default:
                    return QVariant();
            }
            case Qt::UserRole:
                return QVariant(m_profileManager->defaultProfile());
            default:
                return QVariant();
        }
    }
    
    QModelIndex sourceIndex = createIndex(index.row() - 1, index.column());
    return QSqlQueryModel::data(sourceIndex, role);
}
