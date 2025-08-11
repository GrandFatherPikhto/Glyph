#include "charmapquery.h"
#include "appcontext.h"

CharmapQuery::CharmapQuery(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_msb(-1)
    , m_tableName("charmap")
    , m_sortCondition(" ORDER BY c.unicode")
{
    m_sqlBegin = QString(
                   "SELECT c.unicode, c.character, cd.name AS category, "
                   "sd.name AS script, dd.name AS decomposition "
                   "FROM %1 c "
                   "JOIN category_data cd ON c.category = cd.id "
                   "JOIN script_data sd ON c.script = sd.id "
                   "JOIN decomposition_data dd ON c.decomposition = dd.id "
                   ).arg(m_tableName);
    // qDebug() << __FILE__ << __LINE__ << m_sqlBegin;
    QObject::connect(m_appContext, &AppContext::valuesInited, this, [=](){

    });
}

CharmapQuery::~CharmapQuery()
{
    qDebug() << "CharmapFilter destroyed";
}

bool CharmapQuery::setFieldCondition(const QString &sqlFieldName, const QVector<int> &values, const QString &op)
{
    if (!values.size())
    {
        remove(CharmapKey(sqlFieldName, op));
        return false;
    }

    CharmapKey key(sqlFieldName, op);
    QStringList strValues;

    for (auto const &value : values)
    {
        strValues << QString("%1").arg(value);
    }

    m_items[key] = strValues;
    return true;
}

void CharmapQuery::setTextFilter(const QString &value)
{
    m_charsFilter = value;
}

void CharmapQuery::setMSBFilter(int value)
{
    m_msb = value;
}

QStringList CharmapQuery::find(const CharmapKey &key)
{
    const auto it = m_items.find(key);
    if(it == m_items.end())
        return QStringList();
    return it.value();
}

bool CharmapQuery::remove(const CharmapKey &key)
{
    const auto it = m_items.find(key);
    if(it == m_items.end())
        return false;
    m_items.remove(key);
    return true;
}

QString CharmapQuery::updateQuery()
{
    if (!m_items.size() && m_msb < 0 && m_charsFilter.isEmpty())
        return m_sqlBegin + m_sortCondition;

    m_bindValues.clear();

    QStringList conditions;

    const QList<CharmapKey> &keys = m_items.keys();

    for (auto &key : keys)
    {
        if (key.op == "=")
        {
            QStringList values = find(key);

            if (values.size())
            {
                if (values.size() == 1)
                {
                    m_bindValues.insert(key.bindName(), values.first());
                    conditions << "( " + key.name + " = " + key.bindName() + " )";
                } else 
                {
                    int counter = 0;
                    QStringList inConditions;
                    for(auto &value : values)
                    {
                        m_bindValues.insert(key.bindName(counter), value);
                        inConditions << key.bindName(counter);
                        counter ++;
                    }

                    conditions << key.name + " IN (" + inConditions.join(", ") + ") ";
                }
            }
        }
    }

    // qDebug() << "MSB:" << m_msb;
    if (m_msb >= 0)
    {
        QString msbCondition = "((c.unicode >> 8) = :unicode_msb)";
        conditions << msbCondition;
        m_bindValues.insert(":unicode_msb", QVariant(m_msb));
        // qDebug() << __FILE__ << __LINE__ << "MSB Filter: " << m_msb << msbCondition;
    }

    if (!m_charsFilter.isEmpty())
    {
        QStringList unicodes;
        const QString &chars = m_charsFilter;
        for (QChar ch : chars)
        {
            unicodes << QString::number(static_cast<quint32>(ch.unicode()));
        }

        conditions << "c.unicode IN(" + unicodes.join(", ") + ")";
    }

    QString sqlConditions = "WHERE " + conditions.join(" AND ");

    // qDebug() << __FILE__ << __LINE__ << m_bindValues;

    return m_sqlBegin + sqlConditions + m_sortCondition;
}


bool CharmapQuery::execQuery(QSqlQuery &query)
{
    QString sql = updateQuery();

    qDebug() << __FILE__ << __LINE__ << sql;

    if(!query.prepare(sql))
    {
        qWarning() << __FILE__ << __LINE__ << "Can't prepare sql: " << sql << query.lastError();
        return false;
    }

    const QStringList &keys = m_bindValues.keys();
    for (const QString &key : keys)
    {
        auto it = m_bindValues.find(key);
        if (it != m_bindValues.end())
        {
            query.bindValue(key, it.value());
            // qDebug() << __FILE__ << __LINE__ << key << it.value();
        }
    }

    // qDebug() << __FILE__ << __LINE__ << query.boundValueNames() << query.boundValues();

    if (!query.exec())
    {
        qWarning() << "Can't exec sql " << sql << query.lastError();
        return false;
    }

    return true;
}

