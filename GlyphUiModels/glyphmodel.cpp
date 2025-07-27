#include <QDebug>

#include "glyphmodel.h"

GlyphModel::GlyphModel(AppContext *appContext, QObject *parent)
    : QAbstractItemModel(parent)
    , m_appContext(appContext)
{
    m_headers.append("Unicode");
    m_headers.append("Character");
    m_headers.append("Script");
    m_headers.append("Category");
    m_headers.append("Decomposition");

    QObject::connect(m_appContext->glyphManager(), &GlyphManager::glyphDataChanged, this, [=](){
        qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "Glyph Data Changed";
    });
}

QVariant GlyphModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal && section < m_headers.size()) {
        // qDebug() << "headerData" << section << m_headers.at(section);
        return m_headers.at(section);
    }

    return QVariant();
}

QModelIndex GlyphModel::index(int row, int column, const QModelIndex &parent) const
{
    // qDebug() << "index" << row << column;
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    // Для плоских таблиц (без иерархии) просто создаем индекс
    return createIndex(row, column);
}

QModelIndex GlyphModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex(); // Плоская модель без родительских элементов
}

int GlyphModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_appContext->glyphTableSize();
}

int GlyphModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_headers.size();
}

QVariant GlyphModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int idx = index.row();

    QSharedPointer<GlyphMeta> glyphMeta =  m_appContext->glyphAt(idx);
    if (!glyphMeta)
        return QVariant();

    QChar ch = glyphMeta->character();
    
    switch (index.column())
    {
    case 0:
        if (role == Qt::DisplayRole)
        {
            return QString("U+%1").arg(int(ch.unicode()), 4, 16, QLatin1Char('0')).toUpper();
        }        
        break;
    
    case 1:
        if (role == Qt::DisplayRole || role == Qt::EditRole)
            return QString(ch);
        if (role == Qt::FontRole && glyphMeta->font() != QFont())
        {
            return glyphMeta->font();
        }
        if (role == Qt::TextAlignmentRole)
            return Qt::AlignCenter;
        break;
    case 2: // Язык
        if (role == Qt::DisplayRole)
        {
            return ch.script() == QChar::Script_Unknown ? "Unknown" : m_appContext->unicodeScriptName(ch);
        }
        break;
    case 3: // Категория символа
        if (role == Qt::DisplayRole)
            return QChar::category(ch.unicode()) == QChar::Other_NotAssigned ? "N/A" : m_appContext->unicodeCategoryName(ch);
        break;

    case 4:
        if (role == Qt::DisplayRole)
        {
            return QVariant(ch.decomposition());
        }
        break;

    default:
        break;
    }
    return QVariant();
}
