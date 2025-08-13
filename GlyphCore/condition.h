#ifndef CONDITION_H
#define CONDITION_H

#include <QDebugStateSaver>
#include <QDebug>
#include <QString>
#include <QVariant>
#include <QMap>
#include <QPair>
#include <QSqlQuery>

#include "GlyphCore_global.h"

class GLYPHCORE_EXPORT Condition {
public:
    enum class ConditionOperator {
        NOP = 0x00,
        EQ = 0x01,
        LE = 0x02,
        GE = 0x03,
        LT = 0x04,
        GT = 0x05
    };

    explicit Condition(const QString &field = QString(), const QString &placeholder = QString(), ConditionOperator op = ConditionOperator::EQ, const QVariantList &values = QVariantList())
        : m_field(field)
        , m_placeholder(placeholder)
        , m_op(op)
        , m_values(values)
    {

    }

    Condition(const Condition &condition)
        : m_field(condition.m_field)
        , m_placeholder(condition.m_placeholder)
        , m_op(condition.m_op)
        , m_values(condition.m_values)
        , m_binds(condition.m_binds)
    {

    }

    ~Condition () {}


    bool isValid() {
        return !(
               m_field.isEmpty()
            || m_placeholder.isEmpty()
            || m_op == ConditionOperator::NOP
            );
    }

    const QString & field() const { return m_field; }
    const QString & placeholder() const { return m_placeholder; }
    ConditionOperator op() const { return m_op; } 

    const QVariantList & values() const
    {
        return m_values;
    }

    const QList<QPair<QString, QVariant>> & binds() const 
    {
        return m_binds;
    }

    QString opName() const
    {
        switch (m_op) {
        case ConditionOperator::EQ:
            return "EQ";
        case ConditionOperator::LE:
            return "LE";
        case ConditionOperator::GE:
            return "GE";
        case ConditionOperator::LT:
            return "LT";
        case ConditionOperator::GT:
            return "GT";
        case ConditionOperator::NOP:
            return "NOP";
        default:
            return "NOP";
        }
    }

    void setValue(const QVariant &value, int index = 0)
    {
        if (m_values.size() == 0 && index == 0)
        {
            m_values.append(value);
        }

        if(m_values.size() <= index)
            return;

        m_values[index] = value;
    }

    void setValue(const QVariantList &values)
    {
        m_values = values;
    }

    void resetValue()
    {
        m_values.clear();
    }

    void appendValue(const QVariantList &values)
    {
        m_values.append(values);
        // qDebug() << __FILE__ << __LINE__ << m_values;
    }

    void appendValue(const QVariant &value)
    {
        m_values.append(value);
    }

    void removeValue(const QVariantList &values)
    {
        // m_values.removeAll(values);
        // qDebug() << __FILE__ << __LINE__ << values;
        auto newEnd = std::remove_if(m_values.begin(), m_values.end(), [=](const QVariant &item) {
            // qDebug() << __FILE__ << __LINE__ << item << values.contains(item);
            return values.contains(item);
        });
        m_values.erase(newEnd, m_values.end());
        // qDebug() << __FILE__<< __LINE__ << m_values;
    }

    void removeValue(const QVariant &value)
    {
        m_values.removeAll(value);
        // std::remove_if(m_values.begin(), m_values.end(), [=](const QVariant &item) {
        //     return item == value;
        // });
    }

    int size()
    {
        return m_values.size();
    }

    QString prepareSql ()
    {
        m_binds.clear();

        if (m_values.size() == 0)
            return QString();

        switch(m_op)
        {
        case ConditionOperator::EQ:
            return sqlEq();
        case ConditionOperator::LE:
            return sqlLeq();
        case ConditionOperator::GE:
            return sqlGeq();
        case ConditionOperator::LT:
            return sqlLt();
        case ConditionOperator::GT:
            return sqlGt();
        default:
            return QString();
        }

        return QString();
    }

    void bindQuery(QSqlQuery &query)
    {

    }

    const Condition & operator= (const Condition &condition)
    {
        m_field = condition.m_field;
        m_placeholder = condition.m_placeholder;
        m_op = condition.m_op;
        m_values = condition.m_values;
        m_binds = condition.m_binds;

        return *this;
    }


    bool operator== (const Condition &condition) const
    {
        return (
               m_field == condition.m_field
            && m_placeholder == condition.m_placeholder
            && m_op == condition.m_op
            && m_values == condition.m_values
            );
    }

    bool operator< (const Condition &condition) const
    {
        if (m_field != condition.m_field)
            return m_field < condition.m_field;
        if (m_placeholder != condition.m_placeholder)
            return m_placeholder < condition.m_placeholder;
        if (m_op != condition.m_op)
            return m_op < condition.m_op;

        return m_values.size() < condition.m_values.size();
    }

private:

    QString sqlLeq()
    {
        if (m_values.size() == 1)
        {
            QString placeholder = placeholderName();
            m_binds.append({placeholder, m_values.at(0)});
            return QString("%1 <= %2").arg(m_field, placeholder);
        }

        return QString();
    }

    QString sqlGeq()
    {
        if (m_values.size() == 1)
        {
            QString placeholder = placeholderName();
            m_binds.append({placeholder, m_values.at(0)});
            return QString("%1 >= %2").arg(m_field, placeholder);
        }

        return QString();
    }

    QString sqlLt()
    {
        if (m_values.size() == 1)
        {
            QString placeholder = placeholderName();
            m_binds.append({placeholder, m_values.at(0)});
            return QString("%1 < %2").arg(m_field, placeholder);
        }

        return QString();
    }

    QString sqlGt()
    {
        if (m_values.size() == 1)
        {
            QString placeholder = placeholderName();
            m_binds.append({placeholder, m_values.at(0)});
            return QString("%1 > %2").arg(m_field, placeholder);
        }

        return QString();
    }

    QString sqlEq()
    {
        QString sql;
        if (m_values.size() == 1)
        {
            QString placeholder = placeholderName();
            sql = QString("%1 = %2").arg(m_field, placeholder);
            m_binds.append({placeholder, m_values.first()});
        } else if (m_values.size() > 1)
        {
            QStringList inClauses;
            const QVariantList &values = m_values;
            for (const QVariant &value : values)
            {
                QString placeholder = placeholderName();
                inClauses << placeholder;
                m_binds.append({placeholder, value});
            }

            sql = QString("%1 IN( %2 )").arg(m_field, inClauses.join(", "));
        }

        return sql;
    }

    inline QString placeholderName()
    {
        return QString(":%1_%2").arg(m_placeholder, QString::number(m_binds.size()));
    }

    QString m_field;
    QString m_placeholder;
    ConditionOperator m_op;
    QVariantList m_values;
    QList<QPair<QString, QVariant>> m_binds;
};

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator <<(QDebug debug, const Condition &condition)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "Condition("
        << "Field: "
        << condition.field()
        << ", Placeholder: "
        << condition.placeholder()
        << ", Operator"
        << condition.opName()
        << ", Values: "
        << condition.values()
        << ", Binds: "
        << condition.binds()
        << ")";

    return debug;
}
#endif

#endif // CONDITION_H
