#ifndef FONTCHARACTERSMODEL_H
#define FONTCHARACTERSMODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QItemSelection>
#include <QFont>
#include <QFontMetrics>
#include <QChar>
#include <QString>
#include <QPair>
#include <QLoggingCategory>

#include "appcontext.h"

class DimensionManager;
class FontManager;
class UnicodeMetadata;

class FontCharacterModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FontCharacterModel(AppContext *appContext, QObject *parent = nullptr);
    ~FontCharacterModel();

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
    Qt::ItemFlags flags(const QModelIndex &index) const override;

public slots:

signals:

private:

    void setupSignals();

    QFont m_font;

    AppContext *m_appContext;
    FontManager * m_fontManager;
    UnicodeMetadata * m_unicodeMetadata;
};

#endif // FONTCHARACTERSMODEL_H
