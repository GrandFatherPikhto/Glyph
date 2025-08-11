#include <QSet>

#include "unicodemetadataselectionmodel.h"

UnicodeMetadataSelectionModel::UnicodeMetadataSelectionModel(QAbstractItemModel *model, QSharedPointer<Condition> condition, QObject *parent)
    : QItemSelectionModel{model, parent}
    , m_condition(condition)
{

}   

UnicodeMetadataSelectionModel::~UnicodeMetadataSelectionModel()
{

}

// Выделение одного элемента
void UnicodeMetadataSelectionModel::select(const QModelIndex &index, SelectionFlags command)
{
    if (!index.isValid())
        return;

    // Пример: запрет выделения скрытых элементов (если есть флаг Qt::UserRole)
    if (index.data(Qt::UserRole).toBool()) {
        qDebug() << "Item is hidden and cannot be selected!";
        return;
    }

    // Проверка лимита выделенных элементов
    if (command.testFlag(QItemSelectionModel::Select)) {
        // emit selectionLimitReached(m_maxSelectedItems);
        qDebug() << __FILE__ << __LINE__ << index;
        return;
    }

    QItemSelectionModel::select(index, command);
}

// Выделение группы элементов
void UnicodeMetadataSelectionModel::select(const QItemSelection &selection, SelectionFlags command)
{
    QVariantList ids = selectionToIds(selection);

    switch(command)
    {
    case SelectionFlag::Deselect:
        if (!ids.empty()) {
            m_condition->removeValue(ids);
        }
        break;
    case SelectionFlag::Select:
        if (!ids.empty()) {
            m_condition->appendValue(ids);
        }
        break;
    default:
        break;
    }

    QItemSelectionModel::select(selection, command);
}

QVariantList UnicodeMetadataSelectionModel::selectionToIds(const QItemSelection &selection)
{
#if 0
    QVariantList ids;
    ids.reserve(selection.size()); // Оптимизация: резервируем память
    std::transform (
        selection.begin(), 
        selection.end(), 
        std::back_inserter(ids), [=](const QItemSelectionRange &range) {
            QModelIndex index = range.topLeft();
            return model()->data(index.sibling(index.row(), 0)).toInt();
    });
#endif

    QVariantList ids;
    if (selection.indexes().size())
    {
        const auto &selections = selection.indexes();
        for (auto const &idx : selections)
        {
            const QModelIndex & idIdx = model()->index(idx.row(), 0);
            if (idIdx.isValid())
            {
                ids.append(model()->data(idIdx));
            }
        }
    }

    return ids;
}
