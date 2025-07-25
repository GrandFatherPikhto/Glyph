#ifndef FONTCATEGORYMODEL_H
#define FONTCATEGORYMODEL_H

#include <QAbstractListModel>
#include <QObject>

class FontCategoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FontCategoryModel(QObject *parent = nullptr);

    // Переопределение методов модели
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Добавление данных
    void addItem(QChar::Category id, const QString& name);
    void setItems(const QVector<QChar::Category> items);
    void clearItems ();

private:
    void initNames ();
    void sortItems();

    QVector<QChar::Category> m_items;
    QMap<QChar::Category, QString> m_names;
};

#endif // FONTCATEGORYMODEL_H
