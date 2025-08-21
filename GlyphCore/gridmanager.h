#ifndef GRIDMANAGER_H
#define GRIDMANAGER_H

#include <QObject>
#include <QSqlQuery>

class AppContext;
class AppSettings;

#include "GlyphCore_global.h"
#include "gridcontext.h"

class GLYPHCORE_EXPORT GridManager: public QObject {
    Q_OBJECT
public:
    explicit GridManager (AppContext *AppContext);
    ~GridManager();

    inline const GridContext & grid() const { return m_grid; }
    inline void setGrid(const GridContext &value) { m_grid = value; }

    bool appendOrReplaceGridItem(GridContext &grid);
    bool removeGridItem(int id);
    bool removeGridItem(const GridContext &grid);
    bool findGridItem(GridContext &grid);
    bool findGridItem(int id, GridContext &grid);

    void defaultGridItem(GridContext &grid);
signals:
    void changeGridItem(const GridContext &grid);
    void gridItemChanged(const GridContext &grid);

private:
    void setupSignals ();
    bool createTable ();
    void bindGridContext(QSqlQuery query, const GridContext &grid);
    bool loadGridContext(QSqlQuery query, GridContext &grid);

    void saveGridManagerSettings ();
    void restoreGridManagerSettings ();

    AppContext *m_appContext;
    AppSettings *m_appSettings;

    GridContext m_grid;

    const QString m_tableName = "grids";
};

#endif // GRIDMANAGER_H