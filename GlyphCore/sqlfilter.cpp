#include "sqlfilter.h"

SqlFilter::SqlFilter(QObject *parent)
    : QObject{parent}
{

}

SqlFilter::~SqlFilter()
{

}

bool SqlFilter::isFilter()
{
    auto it = m_conditions.begin();
    while (it != m_conditions.end()) {
        if (it->data()->values().size()) {
            return true;
        }

        ++it;
    }

    return false;
}

bool SqlFilter::addCondition(const QString &name, QSharedPointer<Condition> condition)
{
    if (m_conditions.contains(name))
        return false;

    m_conditions.insert(name, condition);

    return true;
}

bool SqlFilter::removeCondition(const QString &name)
{
    if (!m_conditions.contains(name))
    {
        return false;
    }

    m_conditions.remove(name);

    return true;
}

bool SqlFilter::removeCondition(QSharedPointer<Condition> condition)
{
    auto it = m_conditions.begin();
    while (it != m_conditions.end()) {
        if (it->data() == condition) {
            it = m_conditions.erase(it);  // Удаляем и получаем следующий валидный итератор
            return true;
        } else {
            ++it;
        }
    }

    return false;
}

QSharedPointer<Condition> SqlFilter::getCondition(const QString & name)
{
    if (m_conditions.contains(name))
    {
        auto it = m_conditions.find(name);
        return it.value();
    }

    return QSharedPointer<Condition>();
}

void SqlFilter::reset()
{
    auto it = m_conditions.begin();
    while (it != m_conditions.end()) {
        if (it->data()->values().size()) {
            it->data()->resetValue();
        }
        ++it;
    }
}

QString SqlFilter::prepare()
{
    QString sql;
    m_binds.clear();
    QStringList items;

    if (isFilter())
    {
        auto it = m_conditions.begin();
        while (it != m_conditions.end()) {
            if (it->data()->values().size()) {
                items << it->data()->prepareSql();
                m_binds.append(it->data()->binds());
            }
            ++it;
        }
    }

    if (!items.empty())
    {
        sql = "WHERE " + items.join(" AND ");
    }
    // qDebug() << __FILE__ << __LINE__ << sql;

    return sql;
}

