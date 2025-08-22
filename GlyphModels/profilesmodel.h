#ifndef PROFILESMODEL_H
#define PROFILESMODEL_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QSqlQueryModel>
#include <QSqlTableModel>

#include "GlyphModels_global.h"

#include "profilecontext.h"

class AppContext;
class ProfileManager; 

class GLYPHMODELS_EXPORT ProfilesModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ProfilesModel(AppContext *appContext, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
        AppContext *m_appContext;
        ProfileManager *m_profileManager;
};

#endif // PROFILESMODEL_H
