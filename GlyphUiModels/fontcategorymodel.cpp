#include "fontcategorymodel.h"
#include "appcontext.h"
#include "fontmanager.h"
#include "unicodemetadata.h"

FontCategoryModel::FontCategoryModel(AppContext *appContext, QObject *parent)
    : QAbstractListModel{parent}
    , m_appContext(appContext)
    , m_fontManager(nullptr)
    , m_unicodeMetadata(nullptr)
{
    Q_ASSERT(appContext->fontManager() != nullptr && appContext->unicodeMetadata() != nullptr);
    m_fontManager = appContext->fontManager();
    m_unicodeMetadata = appContext->unicodeMetadata();

    QObject::connect(m_fontManager, FontManager::glyphFontChanged, this, [=](const QFont &font){
        Q_UNUSED(font);
        layoutChanged();
    });
}

int FontCategoryModel::rowCount(const QModelIndex &parent) const
{
    return m_fontManager->fontCategorySize();
}

QVariant FontCategoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_fontManager->fontCategorySize())
        return QVariant();

    QChar::Category item = m_appContext->fontManager()->fontCategoryAt(index.row());
    QString name = m_unicodeMetadata->categoryName(static_cast<QChar::Category>(item));

    switch(role) {
        case Qt::DisplayRole:
            return name;   // Название
        case Qt::EditRole: // Добавьте эту строку
            return name;   // Название
        case Qt::UserRole:
            return item;  // ID
        default:
            return QVariant();
    }
}

