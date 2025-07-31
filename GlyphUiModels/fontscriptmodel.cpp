#include "fontscriptmodel.h"
#include "appcontext.h"
#include "fontmanager.h"
#include "unicodemetadata.h"

FontScriptModel::FontScriptModel(AppContext *appContext, QObject *parent)
    : QAbstractListModel{parent}
    , m_appContext(appContext)
{
    Q_ASSERT(appContext->fontManager() != nullptr && appContext->unicodeMetadata() != nullptr);
    m_fontManager = appContext->fontManager();
    m_unicodeMetadata = appContext->unicodeMetadata();

    QObject::connect(m_fontManager, &FontManager::glyphFontChanged, this, [=](const QFont &font){
        Q_UNUSED(font);
        emit layoutChanged();
    });
}

int FontScriptModel::rowCount(const QModelIndex &parent) const
{
    return m_fontManager->fontScriptSize();
}

QVariant FontScriptModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_fontManager->fontScriptSize())
        return QVariant();

    QChar::Script item = m_fontManager->fontScriptAt(index.row());
    QString name = m_unicodeMetadata->scriptName(static_cast<QChar::Script>(item));

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

