#include "glyphmodel.h"
#include "appcontext.h"

GlyphModel::GlyphModel(AppContext *appContext, QObject *parent)
    : QSqlQueryModel{parent}
    , m_appContext(appContext)
    , m_headers(QStringList({"Unicode", "Character"}))
{

}

GlyphModel::~GlyphModel()
{

}

QVariant GlyphModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0: { // Unicode
          const QModelIndex &idx = QSqlQueryModel::index(index.row(), 1);
          if (idx.isValid())
          {
            return QSqlQueryModel::data(idx).toUInt();
          }  
        } break;
        
        case 1: {
            const QModelIndex &idx = QSqlQueryModel::index(index.row(), 1);
            if (idx.isValid())
            {
                return QSqlQueryModel::data(idx).toChar();
            }
        } break;

        default:
            return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole && (index.column() == 1 || index.column() == 0))
        return Qt::AlignCenter;

    return QSqlQueryModel::data(index, role);
}

// Переопределяем headerData для локализации заголовков
QVariant GlyphModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if (section >= m_headers.size())
            return QSqlQueryModel::headerData(section, orientation, role);

        return m_headers.at(section);
    }

    return QSqlQueryModel::headerData(section, orientation, role);
}

Qt::ItemFlags GlyphModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QSqlQueryModel::flags(index);

    return flags;
}

// Добавляем метод для получения данных по индексу
QVariant GlyphModel::getData(int row, int column) const
{
    return data(index(row, column));
}

int GlyphModel::columnCount(const QModelIndex &parent) const
{
    // Добавляем 1 столбец для чекбоксов
    return m_headers.size();
}

