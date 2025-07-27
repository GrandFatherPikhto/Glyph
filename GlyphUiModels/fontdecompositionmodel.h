#ifndef FONTDECOMPOSITIONMODEL_H
#define FONTDECOMPOSITIONMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include "appcontext.h"

class FontDecompositionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FontDecompositionModel(AppContext *appContext, QObject *parent = nullptr);

    // Переопределение методов модели
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    void initNames ();
    void sortItems();

    AppContext *m_appContext;
    QMap<QChar::Decomposition, QString> m_names;
};

#endif // FONTDECOMPOSITIONMODEL_H
