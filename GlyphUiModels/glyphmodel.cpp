#include <QDebug>

#include "glyphmodel.h"
#include "appcontext.h"
#include "glyphmanager.h"
#include "unicodemetadata.h"

GlyphModel::GlyphModel(AppContext *appContext, QObject *parent)
    : QAbstractItemModel(parent)
    , m_appContext(appContext)
    , m_glyphManager(nullptr)
    , m_unicodeMetadata(nullptr)
{
    Q_ASSERT(m_appContext->glyphManager() != nullptr && m_appContext->unicodeMetadata() != nullptr);
    initHeaders ();
    m_glyphManager = m_appContext->glyphManager();
    m_unicodeMetadata = m_appContext->unicodeMetadata();

    QObject::connect(m_glyphManager, &GlyphManager::glyphCreated, this, [=](QSharedPointer<GlyphContext> glyphContext) {
        Q_UNUSED(glyphContext);
        qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "Glyph Data Changed";
    });

    QObject::connect(m_glyphManager, &GlyphManager::glyphChanged, this, [=](QSharedPointer<GlyphContext> glyphContext) {
        Q_UNUSED(glyphContext);
        layoutChanged();
    });

}

void GlyphModel::initHeaders ()
{
    m_headers.append("Unicode");
    m_headers.append("Character");
    m_headers.append("Script");
    m_headers.append("Category");
    m_headers.append("Decomposition");
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

    return m_glyphManager->filteredSize();
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

    QSharedPointer<GlyphContext> glyphContext =  m_glyphManager->filteredAt(idx);
    if (glyphContext.isNull())
        return QVariant();

    QChar ch = glyphContext->character();
    
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

        if (role == Qt::FontRole && glyphContext->glyphFont() != QFont())
        {
            return glyphContext->glyphFont();
        }
        if (role == Qt::TextAlignmentRole)
            return Qt::AlignCenter;
        break;
    case 2: // Язык
        if (role == Qt::DisplayRole)
        {
            return ch.script() == QChar::Script_Unknown ? "Unknown" : m_unicodeMetadata->scriptName(ch);
        }
        break;
    case 3: // Категория символа
        if (role == Qt::DisplayRole)
            return QChar::category(ch.unicode()) == QChar::Other_NotAssigned ? "N/A" : m_unicodeMetadata->categoryName(ch);
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
