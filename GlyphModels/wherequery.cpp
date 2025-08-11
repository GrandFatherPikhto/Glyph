#include "wherequery.h"

WhereQuery::WhereQuery()
    : m_unicodeMSB(-1)
{

}

WhereQuery::~WhereQuery()
{

}

void WhereQuery::addClause(const QListView *lv, const QString &param)
{
    QItemSelectionModel *selectionModel = lv->selectionModel();
    if (!selectionModel->selectedIndexes().size())
        return;

    QStringList strOr;

    auto it = m_conditions.find(param);

    if (it != m_conditions.end())
    {
        strOr = it.value();
        // m_conditions.insert(param, strOr);
    }

    for (auto & Idx : selectionModel->selectedIndexes())
    {
        QModelIndex modelIdx = lv->model()->index(Idx.row(), 0);
        int categoryId = lv->model()->data(modelIdx).toInt();
        strOr << QString("%1 = %2").arg(param, QString::number(categoryId));
    }

    // m_conditions << "(" + strOr.join(" OR ") + ") ";
}

QString WhereQuery::buildWhereClause()
{
/*    
    if (m_unicodeMSB >= 0) {
        m_conditions << QString("(c.unicode >> 8) = %1").arg(m_unicodeMSB);
    }
    
    return m_conditions.isEmpty() ? "" : "WHERE " + m_conditions.join(" AND ");
*/    
}

