#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "gridmanager.h"
#include "appcontext.h"
#include "appsettings.h"

GridManager::GridManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
    {
        restoreGridManagerSettings();
        createTable();
        setupSignals ();
    }

GridManager::~GridManager()
{
    saveGridManagerSettings();
}

void GridManager::setupSignals()
{
    connect(this, &GridManager::changeGridItem, this, [=](const GridContext &grid) { 
        if (m_grid != grid)
        {
            m_grid = grid;
            emit gridItemChanged(grid);
        }
    });
}

bool GridManager::createTable()
{
    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    QString createTableQuery =
        QString(
            "CREATE TABLE IF NOT EXISTS %1 ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "width INTEGER, "
            "height INTEGER, "
            "left INTEGER, "
            "top INTEGER, "
            "right INTEGER, "
            "bottom INTEGER, "
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
            "UNIQUE(width, height) ON CONFLICT REPLACE"
            ");"
        ).arg(m_tableName);
    
    if (!query.exec(createTableQuery)) {
        qWarning() << __FILE__ << __LINE__ << QString("Failed to create table %1: %2").arg(m_tableName, query.lastError().text());
        return false;
    }
    
    return true;
}

bool GridManager::appendGridItem(GridContext &grid)
{
    if (!grid.isValid())
        return false;

    if (findGridItem(grid))
    {
        return true;
    }

    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
    {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    db.transaction();

    QSqlQuery query(db);

    if (!query.prepare(QString(
        "INSERT INTO %1 "
        "(width, height, left, top, right, bottom) "
        "VALUES (:width, :height, :left, :top, :right, :bottom)"
    ).arg(m_tableName)))
    {
        qWarning() << __FILE__ << __LINE__ << "Can't prepare query" << query.lastQuery() << " With Error:" << query.lastError();
        db.rollback();
        return false;
    }
    
    bindGridContext(query, grid);

    if (!query.exec()) {
        qWarning() << __FILE__ << __LINE__ << "Failed to insert grid item" <<  query.lastError().text() << query.lastQuery();
        db.rollback();

        return false;
    }

    if (!db.commit()) {
        qWarning() << __FILE__ << __LINE__ << "Commit failed:" << db.lastError().text();
        return false;
    }

    // qDebug() << __FILE__ << __LINE__ << query.lastInsertId();
    grid.setId(query.lastInsertId().toInt());

    return true;
}

void GridManager::bindGridContext(QSqlQuery &query, const GridContext &grid)
{
    query.bindValue(":width", grid.width());
    query.bindValue(":height", grid.height());
    query.bindValue(":left", grid.left());
    query.bindValue(":top", grid.top());
    query.bindValue(":right", grid.right());
    query.bindValue(":bottom", grid.bottom());
}

bool GridManager::loadGridContext(QSqlQuery &query, GridContext &grid)
{
    try {
        grid.setId(query.value("id").toInt());
        grid.setWidth(query.value("width").toInt());
        grid.setHeight(query.value("height").toInt());
        grid.setLeft(query.value("left").toInt());
        grid.setTop(query.value("top").toInt());
        grid.setRight(query.value("right").toInt());
        grid.setBottom(query.value("bottom").toInt());
    }  catch (const std::exception &e) {
        qCritical() << "Error parsing profile data:" << e.what();
        return false;
    }

    return true;
}

bool GridManager::removeGridItem(int id)
{
    GridContext grid;
    if (!findGridItem(id, grid))
    {
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    if(!query.prepare(QString(
        "DELETE FROM %1 WHERE id = :id"
    ).arg(m_tableName)))
    {
        qWarning() << __FILE__ << __LINE__ << "Can't prepare grid SQL" << query.lastQuery() << " width Id:" << id << query.lastError();
        return false;
    }

    query.bindValue(":id", id);

    if(!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't exec grid SQL" << query.lastQuery() << " width Id:" << id << query.lastError();
        return false;
    }

    return true;
}

bool GridManager::removeGridItem(const GridContext &grid)
{
    GridContext gridItem(grid);
    
    if (!findGridItem(gridItem))
    {
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    if(!query.prepare(QString(
        "DELETE FROM %1 WHERE width = :width AND height = :height"
    ).arg(m_tableName)))
    {
        qWarning() << __FILE__ << __LINE__ << "Can't prepare grid SQL" << query.lastQuery() << " width width:" << grid.width() << ", height:" << grid.height() << query.lastError();
        return false;
    }

    query.bindValue(":width", gridItem.width());
    query.bindValue(":height", gridItem.height());

    if(!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't exec grid SQL" << query.lastQuery() << " width width:" << grid.width() << ", height:" << grid.height() << query.lastError();
        return false;
    }

    return true;
}

bool GridManager::findGridItem(GridContext &grid)
{
    grid.setId(-1);

    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    if(!query.prepare(QString(
        "SELECT width, height, left, top, right, bottom, created_at FROM %1 WHERE width = :width AND height = :height"
    ).arg(m_tableName)))
    {
        qWarning() << __FILE__ << __LINE__ << "Can't prepare grid SQL" << query.lastQuery() << " width width:" << grid.width() << ", height:" << grid.height() << query.lastError();
        return false;
    }

    query.bindValue(":width", grid.width());
    query.bindValue(":height", grid.height());

    if(!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't exec grid SQL" << query.lastQuery() << " width width:" << grid.width() << ", height:" << grid.height() << ", Error:" << query.lastError();
        return false;
    }

    if (!query.next())
    {
        // qWarning() << __FILE__ << __LINE__ << "Can't find grid SQL" << query.lastQuery() << " width width:" << grid.width() << ", height:" << grid.height() << ", Error:" << query.lastError();
        grid.setId(-1);
        return false;
    }

    return loadGridContext(query, grid);
}

bool GridManager::findGridItem(int id, GridContext &grid)
{
    QSqlDatabase db = QSqlDatabase::database("main");
    grid.setId();

    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    if(!query.prepare(QString(
        "SELECT width, height, left, top, right, bottom, created_at FROM %1 WHERE id = :id"
    ).arg(m_tableName)))
    {
        qWarning() << __FILE__ << __LINE__ << "Can't prepare grid SQL" << query.lastQuery() << " width Id:" << id << query.lastError();
        return false;
    }

    query.bindValue(":id", id);

    if(!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't exec grid SQL" << query.lastQuery() << " width Id:" << id << query.lastError();
        return false;
    }

    if (!query.next())
    {
        // qWarning() << __FILE__ << __LINE__ << "Can't find grid SQL" << query.lastQuery() << " width Id:" << id << query.lastError();
        return false;
    }

    return loadGridContext(query, grid);
}

void GridManager::defaultGridItem(GridContext &grid)
{
    if (grid.isValid())
        return;

    if (grid.width() < 0)
        grid.setWidth(m_appSettings->value("defaultGridWidth").toInt());

    if (grid.height() < 0)
        grid.setHeight(m_appSettings->value("defaultGridHeight").toInt());

    if (grid.left() < 0)
        grid.setLeft(m_appSettings->value("defaultGridLeft").toInt());

    if (grid.top() < 0)
        grid.setTop(m_appSettings->value("defaultGridTop").toInt());

    if (grid.right() < 0)
        grid.setRight(m_appSettings->value("defaultGridRight").toInt());

    if (grid.bottom() < 0)
        grid.setBottom(m_appSettings->value("defaultGridBottom").toInt());
}

void GridManager::saveGridManagerSettings()
{
    QSettings settings(this);
    settings.beginGroup("GridManager");
    settings.setValue("grid", m_grid);
    settings.endGroup();
}

void GridManager::restoreGridManagerSettings()
{
    QSettings settings(this);
    settings.beginGroup("GridManager");
    m_grid = settings.value("grid", GridContext()).value<GridContext>();
    settings.endGroup();

    defaultGridItem(m_grid);
}
