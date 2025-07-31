#include "fontcharactermodel.h"
#include "appcontext.h"
#include "fontmanager.h"
#include "glyphmanager.h"
#include "unicodemetadata.h"

FontCharacterModel::FontCharacterModel(AppContext *appContext, QObject *parent)
    : QAbstractItemModel{parent}
    , m_appContext(appContext)
{
    Q_ASSERT(m_appContext->glyphManager() != nullptr && m_appContext->unicodeMetadata() != nullptr);
    m_fontManager = m_appContext->fontManager();
    m_unicodeMetadata = m_appContext->unicodeMetadata();

    setupSignals ();
}

FontCharacterModel::~FontCharacterModel()
{

}

void FontCharacterModel::setupSignals()
{
    QObject::connect(m_fontManager, &FontManager::filteredCharactersChanged, this, [=](const QVector<QChar> &items){
        Q_UNUSED(items)
        // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "Character List Changed";
        emit layoutChanged ();
    });

    QObject::connect(m_fontManager, &FontManager::glyphFontChanged, this, [=](const QFont &font){
        Q_UNUSED(font);
        emit layoutChanged();
    });
}

QVariant FontCharacterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 1: return "Unicode";
            case 2: return "Character";
            case 3: return "Script";
            case 4: return "Category";
            case 5: return "Decomposition";
            case 6: return "HTML";
        }
    }

    return QVariant();
}

QModelIndex FontCharacterModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    // Для плоских таблиц (без иерархии) просто создаем индекс
    return createIndex(row, column);
}


QModelIndex FontCharacterModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex(); // Плоская модель без родительских элементов
}

int FontCharacterModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_appContext->fontManager()->filteredSize();
}

int FontCharacterModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 6;
}

QVariant FontCharacterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_appContext->fontManager()->filteredSize())
        return QVariant();

    int idx = index.row();

    const QChar ch = m_appContext->fontManager()->filteredCharacterAt(idx);

    switch (index.column()) {
        case 0: // Unicode код
            if (role == Qt::DisplayRole)
            {
                return QString("U+%1").arg(int(ch.unicode()), 4, 16, QLatin1Char('0')).toUpper();
            }
            break;

        case 1: // Символ
            if (role == Qt::DisplayRole || role == Qt::EditRole)
                return QString(ch);
            if (role == Qt::FontRole)
            {
                return QFont(m_font.family(), m_appContext->fontManager()->fontSize());
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

        case 5: // HTML-код
            if (role == Qt::DisplayRole)
                return QString("&#%1;").arg(ch.unicode());
            break;

        default:
            return QVariant();
    }

    return QVariant();
}

Qt::ItemFlags FontCharacterModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() == 0) {
        flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }

    return flags;
}


