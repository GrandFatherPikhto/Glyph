#ifndef CHARMAPTABLEMODEL_H
#define CHARMAPTABLEMODEL_H

#include <QStringList>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "GlyphModels_global.h"

#include "glyphcontext.h"

class AppContext;
class AppSettings;
class CharmapManager;
class GlyphManager;
class ProfileManager;

class GLYPHMODELS_EXPORT CharmapModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit CharmapModel(AppContext *appContext, QObject *parent = nullptr);
    ~CharmapModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    // void refresh(const QString &query);
    QVariant getData(int row, int column) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int columnCount(const QModelIndex &parent) const override;

    int unicode(int row);
    int checked(int row);
    QChar character(int row);

private:
    QStringList m_headers;
    AppContext *m_appContext;
    ProfileManager *m_profileManager;
    GlyphManager *m_glyphManager;
    AppSettings *m_appSettings;
};

#endif // CHARMAPTABLEMODEL_H
