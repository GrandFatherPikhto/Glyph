#ifndef UNICODEMETASELECTIONMODEL_H_
#define UNICODEMETASELECTIONMODEL_H_

#include <QObject>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QModelIndex>
#include <QSharedPointer>

#include "GlyphModels_global.h"

#include "condition.h"

class GLYPHMODELS_EXPORT UnicodeMetadataSelectionModel : public QItemSelectionModel {
    Q_OBJECT
public:
    UnicodeMetadataSelectionModel(QAbstractItemModel *model, QSharedPointer<Condition> condition, QObject *parent = nullptr);
    ~UnicodeMetadataSelectionModel();

    // Переопределение методов выделения
    void select(const QModelIndex &index, QItemSelectionModel::SelectionFlags command) override;
    void select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command) override;
    
private:
    QVariantList selectionToIds(const QItemSelection &selection);

    QSharedPointer<Condition> m_condition;
    QString m_fieldName;
};

#endif // UNICODEMETASELECTIONMODEL_H_
