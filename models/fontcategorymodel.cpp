#include "fontcategorymodel.h"

FontCategoryModel::FontCategoryModel(QObject *parent)
    : QAbstractListModel{parent}
{
    initNames();
}

int FontCategoryModel::rowCount(const QModelIndex &parent) const
{
    return m_items.size();
}

QVariant FontCategoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return QVariant();

    QString name = QString("Unknown Script");
    QChar::Category item = m_items[index.row()];
    auto it = m_names.find(static_cast<QChar::Category>(item));
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

void FontCategoryModel::setItems(const QVector<QChar::Category> items)
{
    beginResetModel();
    m_items.clear ();
    endResetModel();

    beginInsertRows(QModelIndex(), 0, items.size());
    m_items = items;
    endInsertRows();
}

void FontCategoryModel::addItem(QChar::Category id, const QString &name)
{
    if (!m_items.contains(id))
    {
        beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
        m_items.append(id);
        endInsertRows ();
        sortItems ();
    }
}

void FontCategoryModel::sortItems() {

}

void FontCategoryModel::clearItems()
{
    beginResetModel();
    m_items.clear();
    endResetModel ();
}

void FontCategoryModel::initNames ()
{
    m_names.clear ();
    m_names = {
        {QChar::Mark_NonSpacing, "NonSpacing (Mn)"},
        {QChar::Mark_SpacingCombining, "SpacingCombining (Mc)"},
        {QChar::Mark_Enclosing, "Enclosing (Me)"},
        {QChar::Number_DecimalDigit, "DecimalDigit (Nd)"},
        {QChar::Number_Letter, "Letter (Nl)"},
        {QChar::Number_Other, "Other (No)"},
        {QChar::Separator_Space, "Space (Zs)"},
        {QChar::Separator_Line, "Line (Zl)"},
        {QChar::Separator_Paragraph, "Paragraph (Zp)"},
        {QChar::Other_Control, "Control (Cc)"},
        {QChar::Other_Format, "Format (Cf)"},
        {QChar::Other_Surrogate, "Surrogate (Cs)"},
        {QChar::Other_PrivateUse, "PrivateUse (Co)"},
        {QChar::Other_NotAssigned, "NotAssigned (Cn)"},
        {QChar::Letter_Uppercase, "Uppercase (Lu)"},
        {QChar::Letter_Lowercase, "Lowercase (Ll)"},
        {QChar::Letter_Titlecase, "Titlecase (Lt)"},
        {QChar::Letter_Modifier, "Modifier (Lm)"},
        {QChar::Letter_Other, "Other (Lo)"},
        {QChar::Punctuation_Connector, "Connector (Pc)"},
        {QChar::Punctuation_Dash, "Dash (Pd)"},
        {QChar::Punctuation_Open, "Open (Ps)"},
        {QChar::Punctuation_Close, "Close (Pe)"},
        {QChar::Punctuation_InitialQuote, "InitialQuote (Pi)"},
        {QChar::Punctuation_FinalQuote, "FinalQuote (Pf)"},
        {QChar::Punctuation_Other, "Other (Po)"},
        {QChar::Symbol_Math, "Math (Sm)"},
        {QChar::Symbol_Currency, "Currency (Sc)"},
        {QChar::Symbol_Modifier, "Modifier (Sk)"},
        {QChar::Symbol_Other, "Other (So)"},
    };
}

