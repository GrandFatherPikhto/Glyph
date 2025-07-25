#ifndef FONTSCRIPTMODEL_H
#define FONTSCRIPTMODEL_H

#include <QAbstractListModel>
#include <QObject>

class FontScriptModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FontScriptModel(QObject *parent = nullptr);

    // Переопределение методов модели
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Добавление данных
    void addItem(QChar::Script id, const QString& name);
    void setItems(const QVector<QChar::Script> items);
    void clearItems ();

private:
    void initNames ();
    void sortItems();

    QVector<QChar::Script> m_items;
    QMap<QChar::Script, QString> m_names;
};

#endif // FONTSCRIPTMODEL_H
