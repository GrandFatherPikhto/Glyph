#include "fontdecompositionmodel.h"

FontDecompositionModel::FontDecompositionModel(AppContext *appContext, QObject *parent)
    : QAbstractListModel{parent}
    , m_appContext(appContext)
{
    initNames();
    QObject::connect(m_appContext->fontManager(), &FontManager::glyphFontChanged, this, [=](const QFont &font){
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

    QString name = QString("Unknown Script");
    QChar::Decomposition item = m_appContext->fontManager()->fontDecompositionAt(index.row());
    auto it = m_names.find(static_cast<QChar::Decomposition>(item));
    if (it != m_names.end())
    {
        name = it.value();
    }

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

void FontDecompositionModel::initNames ()
{
    m_names.clear ();
    m_names = {
        {QChar::NoDecomposition, "NoDecomposition"},
        {QChar::Canonical, "Canonical"},
        {QChar::Circle, "Circle"},
        {QChar::Compat, "Compat"},
        {QChar::Final, "Final"},
        {QChar::Font, "Font"},
        {QChar::Fraction, "Fraction"},
        {QChar::Initial, "Initial"},
        {QChar::Isolated, "Isolated"},
        {QChar::Medial, "Medial"},
        {QChar::Narrow, "Narrow"},
        {QChar::NoBreak, "NoBreak"},
        {QChar::Small, "Small"},
        {QChar::Square, "Square"},
        {QChar::Sub, "Sub"},
        {QChar::Super, "Super"},
        {QChar::Vertical, "Vertical"},
        {QChar::Wide, "Wide"},
    };
}

