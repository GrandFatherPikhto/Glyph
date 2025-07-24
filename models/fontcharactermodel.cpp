#include "fontcharactermodel.h"
#include "appcontext.h"

FontCharacterModel::FontCharacterModel(AppContext *appContext, QObject *parent)
    : QAbstractItemModel(parent)
    , m_appContext(appContext)
    , m_MSBFilter(-1)
    , m_fontSize(9)
{
    m_characters.clear();
    m_fontCharacters.clear();
    m_scriptFilter.clear();
    m_categoryFilter.clear();
    QObject::connect(this, &FontCharacterModel::setUnicodeMSB, this, &FontCharacterModel::setUnicodeMSBFilter);
}

QVariant FontCharacterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return "Selected";
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

    return m_characters.size();
}

int FontCharacterModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 6;
}

bool FontCharacterModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.column() == 0 && role == Qt::CheckStateRole) {
        m_characters[index.row()].setSelected(value.toBool());
        emit dataChanged(index, index, {Qt::CheckStateRole});
        return true;
    }
    return false;
}

QVariant FontCharacterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_characters.size())
        return QVariant();

    int idx = index.row();

    if (idx >= m_characters.length())
        return QVariant();

    const QChar ch = m_characters.at(idx).getChar();
    const bool selected = m_characters.at(idx).isSelected();


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
                return QFont(m_font.family(), m_fontSize);
            }
            if (role == Qt::TextAlignmentRole)
                return Qt::AlignCenter;
            break;

        case 3: // Язык
            if (role == Qt::DisplayRole)
            {
                return ch.script() == QChar::Script_Unknown ? "Unknown" : m_appContext->unicodeScriptName(ch);
            }
            break;
        case 4: // Категория символа
            if (role == Qt::DisplayRole)
                return QChar::category(ch.unicode()) == QChar::Other_NotAssigned ? "N/A" : m_appContext->unicodeCategoryName(ch);
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

// Добавим вспомогательный метод для получения символа по индексу
QChar FontCharacterModel::characterAt(const QModelIndex &index) const {
    if (!index.isValid() || index.row() >= m_characters.size())
        return QChar();

    return m_characters.at(index.row()).getChar();
}

const FontCharItem & FontCharacterModel::fontCharItemAt(const QModelIndex &index) const
{
    return m_characters.at(index.row());
}

bool FontCharacterModel::isSelected(const QModelIndex &index) const
{
    return m_characters.at(index.row()).isSelected();
}

void FontCharacterModel::setUnicodeMSBFilter(qint16 msb)
{
    m_MSBFilter = msb;
    filterCharList();
}

Qt::ItemFlags FontCharacterModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() == 0) {
        flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }

    return flags;
}

void FontCharacterModel::filterCharList()
{
    beginResetModel(); // Сообщаем представлению о начале изменений    
    m_characters.clear();
    for (const auto& charItem : std::as_const(m_fontCharacters)) {
        if ((m_categoryFilter.isEmpty() || m_categoryFilter.contains(static_cast<QChar::Category>(charItem.getChar().category()))) &&
            (m_scriptFilter.isEmpty() || m_scriptFilter.contains(charItem.getChar().script())) &&
            (m_MSBFilter < 0 || charItem.getChar().row() == m_MSBFilter))
        {
            m_characters.append(charItem);
        }
    }
    endResetModel(); // Сообщаем представлению об окончании изменений    
}

void FontCharacterModel::fillCharList ()
{
    m_fontCharacters.clear();

    QFontMetrics fm(m_font);
    QVector<quint64> supportedChars = m_appContext->fontManager()->listSupportedChars(m_font);

    for (quint64 code : supportedChars) { // Пропускаем управляющие символы
        if (code > 65536)
            continue;

        QChar ch(static_cast<uint16_t>(code));
        //  &&
        if (fm.inFont(ch) && ch.category() != QChar::Other_NotAssigned)
        {
            QRect bbox = fm.boundingRect(ch);
            if (bbox.width() > 1 && bbox.height() > 1) {
                if (!m_categories.contains(ch.category()))
                {
                    m_categories.append(ch.category());
                }

                if (!m_scripts.contains(ch.script()))
                {
                    m_scripts.append(ch.script());
                }

                // if (!m_decompositions.contains(ch.decomposition()))
                // {
                //     m_decompositions.append(ch.decompositionTag());
                // }
                m_fontCharacters.append({ch, false});
            }
        }
        QLoggingCategory::setFilterRules("qt.text.font.db=true");
    }

    // emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1));
    emit categoriesChanged(m_categories);
    emit scriptsChanged(m_scripts);
    emit decompositionTagsChanged(m_decompositions);
}

void FontCharacterModel::handleSelectionCategoriesChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    const QModelIndexList selectedIndexes = selected.indexes();
    for (const QModelIndex &index : selectedIndexes) {
        // qDebug() << "Selected" << index.data(Qt::UserRole).toString();
        quint32 id = index.data(Qt::UserRole).toUInt();
        if (!m_categoryFilter.contains(index.data(Qt::UserRole))) {
            m_categoryFilter.append(id);
        }
    }

    const QModelIndexList deselectedIndexes = deselected.indexes();
    for (const QModelIndex &index : deselectedIndexes) {
        // qDebug() << "Deselected" << index.data(Qt::UserRole).toString();
        // Находим позицию элемента
        quint32 id = index.data(Qt::UserRole).toUInt();
        int idx = m_categoryFilter.indexOf(id);
        if (idx != -1) {
            m_categoryFilter.remove(idx);
        }
    }

    filterCharList();
}

void FontCharacterModel::handleSelectionScriptsChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    const QModelIndexList selectedIndexes = selected.indexes();
    for (const QModelIndex &index : selectedIndexes) {
        quint32 id = index.data(Qt::UserRole).toUInt();
        if (!m_scriptFilter.contains(index.data(Qt::UserRole))) {
            m_scriptFilter.append(id);
        }
    }

    const QModelIndexList deselectedIndexes = deselected.indexes();
    for (const QModelIndex &index : deselectedIndexes) {
        // Находим позицию элемента
        quint32 id = index.data(Qt::UserRole).toUInt();
        int idx = m_scriptFilter.indexOf(id);
        if (idx != -1) {
            m_scriptFilter.remove(idx);
        }
    }

    filterCharList();
}

QFont FontCharacterModel::font() const
{
    return m_font;
}

void FontCharacterModel::setFont(const QFont &newFont)
{
    if (m_font == newFont)
        return;
    m_font = newFont;
    fillCharList();
    filterCharList();

    emit fontChanged();
}

void FontCharacterModel::resetFont()
{
    setFont({}); // TODO: Adapt to use your actual default value
    fillCharList();
    filterCharList();

    emit fontChanged();
}
