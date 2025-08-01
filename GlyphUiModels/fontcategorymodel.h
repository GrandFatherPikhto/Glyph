#ifndef FONTCATEGORYMODEL_H
#define FONTCATEGORYMODEL_H

#include <QAbstractListModel>
#include <QObject>

class AppContext;
class FontManager;
class UnicodeMetadata;

class FontCategoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FontCategoryModel(AppContext *appContext, QObject *parent = nullptr);

    // Переопределение методов модели
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:

    AppContext *m_appContext;
    
    FontManager * m_fontManager;
    UnicodeMetadata * m_unicodeMetadata;
};

#endif // FONTCATEGORYMODEL_H
