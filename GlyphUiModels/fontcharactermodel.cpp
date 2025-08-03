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

    m_headers.append("Selected");
    m_headers.append("Unicode");
    m_headers.append("Character");
    m_headers.append("Script");
    m_headers.append("Category");
    m_headers.append("Decomposition");
    m_headers.append("HTML");

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

    if (orientation == Qt::Horizontal && section >= 0 && section < m_headers.size())
        return m_headers.at(section);

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

bool FontCharacterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == 0 && role == Qt::CheckStateRole) {
        // m_characters[index.row()].setSelected(value.toBool());
        const QChar ch = m_fontManager->filteredCharacterAt(index.row());
        emit dataChanged(index, index, {Qt::CheckStateRole});
        return true;
    }
    return false;
}

QVariant FontCharacterModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(m_fontManager != nullptr);

    if (!index.isValid() || index.row() >= m_fontManager->filteredSize())
        return QVariant();

    int idx = index.row();

    const QChar ch = m_fontManager->filteredCharacterAt(idx);
    bool selected = false;

    switch (index.column()) {
        case 0: // Выбрать символ для обработки
            if (role == Qt::DisplayRole || role == Qt::EditRole)
                return QVariant(); // Пустое значение для чекбокса
            if (role == Qt::CheckStateRole)
                return  selected ? Qt::Checked :  Qt::Unchecked;
            break;
        case 1: // Unicode код
            if (role == Qt::DisplayRole)
            {
                return QString("U+%1").arg(int(ch.unicode()), 4, 16, QLatin1Char('0')).toUpper();
            }
            break;

        case 2: // Символ
            if (role == Qt::DisplayRole || role == Qt::EditRole)
                return QString(ch);
            if (role == Qt::FontRole)
            {
                return QFont(m_font.family(), m_fontManager->fontSize());
            }
            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;
            break;

        case 3: // Язык
            if (role == Qt::DisplayRole)
            {
                return ch.script() == QChar::Script_Unknown ? "Unknown" : m_unicodeMetadata->scriptName(ch);
            }
            break;
        case 4: // Категория символа
            if (role == Qt::DisplayRole)
                return QChar::category(ch.unicode()) == QChar::Other_NotAssigned ? "N/A" : m_unicodeMetadata->categoryName(ch);
            break;

        case 5:
            if (role == Qt::DisplayRole)
            {
                return QVariant(ch.decomposition());
            }
            break;

        case 6: // HTML-код
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


