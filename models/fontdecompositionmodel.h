#ifndef FONTDECOMPOSITIONMODEL_H
#define FONTDECOMPOSITIONMODEL_H

#include <QAbstractListModel>
#include <QObject>

class FontDecompositionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FontDecompositionModel(QObject *parent = nullptr);

    // Переопределение методов модели
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Добавление данных
    void addItem(QChar::Decomposition id, const QString& name);
    void setItems(const QVector<QChar::Decomposition> items);
    void clearItems ();

private:
    void initNames ();
    void sortItems();

    QVector<QChar::Decomposition> m_items;
    QMap<QChar::Decomposition, QString> m_names;
};

#endif // FONTDECOMPOSITIONMODEL_H
