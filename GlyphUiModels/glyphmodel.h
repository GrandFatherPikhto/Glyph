#ifndef GLYPHSMODEL_H
#define GLYPHSMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class AppContext;
class GlyphManager;
class UnicodeMetadata;

class GlyphModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit GlyphModel(AppContext *appContext, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    void initHeaders ();

    AppContext *m_appContext;
    GlyphManager *m_glyphManager;
    UnicodeMetadata *m_unicodeMetadata;

    QStringList m_headers;
};

#endif // GLYPHSMODEL_H
