#include <QChar>

#include "charmapmodel.h"
#include "appcontext.h"
#include "appsettings.h"
#include "glyphmanager.h"
#include "profilemanager.h"

CharmapModel::CharmapModel(AppContext *appContext, QObject *parent)
    : QSqlQueryModel{parent}
    , m_appContext(appContext)
    , m_profileManager(appContext->profileManager())
    , m_appSettings(appContext->appSettings())
    , m_glyphManager(appContext->glyphManager())
{

}

CharmapModel::~CharmapModel()
{

}

QVariant CharmapModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // Обработка первого столбца (чекбокс)
    if (index.column() == 0) {
        if (role == Qt::DisplayRole) {
            return QVariant(); // Пустое значение для отображения
        } else if (role == Qt::CheckStateRole) {
            QVariant dbValue = QSqlQueryModel::data(index, Qt::DisplayRole);
            if (dbValue.isValid())
            {
                return dbValue.toBool() ? Qt::Checked : Qt::Unchecked;
            }

            return Qt::Unchecked;
        }
    }

    // Можно добавить специальную обработку для определенных ролей или столбцов
    if (role == Qt::TextAlignmentRole && index.column() == 1) // Для первого столбца (Юникод)
        return Qt::AlignCenter;

    return QSqlQueryModel::data(index, role);
}

// Переопределяем headerData для локализации заголовков
QVariant CharmapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        // Локализованные заголовки столбцов
        switch (section)
        {
        case 0:
            return tr("Selected");
        case 1:
            return tr("Unicode");
        case 2:
            return tr("Character");
        case 3:
            return tr("Category");
        case 4:
            return tr("Script");
        case 5:
            return tr("Decomposition");
        default:
            return QSqlQueryModel::headerData(section, orientation, role);
        }
    }
    return QSqlQueryModel::headerData(section, orientation, role);
}

Qt::ItemFlags CharmapModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QSqlQueryModel::flags(index);

    // Первый столбец - чекбокс
    if (index.column() == 0) {
        flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    return flags;
}

// Добавляем метод для получения данных по индексу
QVariant CharmapModel::getData(int row, int column) const
{
    return data(index(row, column));
}

int CharmapModel::columnCount(const QModelIndex &parent) const
{
    // Добавляем 1 столбец для чекбоксов
    return QSqlQueryModel::columnCount(parent) + 1;
}

int CharmapModel::unicode(int row)
{
    const QModelIndex & index = QSqlQueryModel::index(row, 1);
    if (index.isValid())
    {
        return data(index, Qt::DisplayRole).toInt();
    }

    return -1;
}

int CharmapModel::checked(int row)
{
    const QModelIndex & index = QSqlQueryModel::index(row, 0);
    if (index.isValid())
    {
        // qDebug() << __FILE__ << __LINE__ << "Index: " << index << ", Data: " << data(index, Qt::CheckStateRole);
        return data(index, Qt::CheckStateRole).toInt();
    }

    return -1;
}

QChar CharmapModel::character(int row)
{
    const QModelIndex & index = QSqlQueryModel::index(row, 1);
    if (index.isValid())
    {
        return QChar(data(index, Qt::DisplayRole).toUInt());
    }

    return QChar();
}

GlyphContext CharmapModel::glyphContext(int row)
{
    GlyphContext context;

    QChar ch = character(row);

    if(ch == QChar())
        return context;

    context.setCharacter(ch);
    m_profileManager->defaultGlyphContext(context);

    if (m_glyphManager->findGlyph(context))
    {
        return context;
    }

    return context;
}
