#include "appcontext.h"
#include "fontmanager.h"
#include "unicodemetadata.h"
#include "fontdecompositionmodel.h"


FontDecompositionModel::FontDecompositionModel(AppContext *appContext, QObject *parent)
    : QAbstractListModel{parent}
    , m_appContext(appContext)
    , m_fontManager(nullptr)
    , m_unicodeMetadata(nullptr)
{
    Q_ASSERT(m_appContext->fontManager() != nullptr && m_appContext->unicodeMetadata() != nullptr);

    m_fontManager = appContext->fontManager();
    m_unicodeMetadata = appContext->unicodeMetadata();

    QObject::connect(m_fontManager, &FontManager::glyphFontChanged, this, [=](const QFont &font){
        Q_UNUSED(font);
        emit layoutChanged();
    });
}

int FontDecompositionModel::rowCount(const QModelIndex &parent) const
{
    return m_appContext->fontManager()->fontDecompositionSize();
}

QVariant FontDecompositionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_appContext->fontManager()->fontDecompositionSize())
        return QVariant();

    QChar::Decomposition item = m_appContext->fontManager()->fontDecompositionAt(index.row());
    QString name = m_unicodeMetadata->decompositionName(static_cast<QChar::Decomposition>(item));
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

