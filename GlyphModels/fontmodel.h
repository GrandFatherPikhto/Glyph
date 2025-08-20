#ifndef FONTMODEL_H_
#define FONTMODEL_H_

#include <QObject>
#include <QSqlQueryModel>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "GlyphModels_global.h"
#include "fontcontext.h"

class AppContext;

class GLYPHMODELS_EXPORT FontModel: public QSqlQueryModel {
    Q_OBJECT
public:

    explicit FontModel(AppContext *appContext, QObject *parent = nullptr);
    ~FontModel();
    
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int columnCount(const QModelIndex &parent) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int idByRow(int row);
    bool FontContextbyRow(int row, FontContext &context);

private:
    QStringList m_headers;
    AppContext *m_appContext;
};

#endif // FONTMODEL_H_
