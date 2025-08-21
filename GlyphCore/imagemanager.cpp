#include "imagemanager.h"
#include "appsettings.h"
#include "appcontext.h"
#include "glyphmanager.h"
#include "profilemanager.h"
#include "iglyphrendererfactory.h"

ImageManager::ImageManager(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
    , m_glyphManager(appContext->glyphManager())
    , m_profileManager(appContext->profileManager())
    , m_glyphRenderer(IGlyphRendererFactory::createFtRenderer())
    , m_tableName("images")
{
    initValues ();
    createTable();
    setupSignals ();
}

ImageManager::~ImageManager()
{

}

void ImageManager::setupSignals()
{

}

void ImageManager::initValues()
{

}

bool ImageManager::createTable()
{
    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen())
    {
        qWarning()  << __FILE__ << __LINE__ << "Database is not open";
        return false;
    }

    QSqlQuery query(db);

    QString createTableQuery =
        QString(
            "CREATE TABLE IF NOT EXISTS %1 ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "glyph_id INTEGER NOT NULL, "
            "width INTEGER NOT NULL, "
            "height INTEGER NOT NULL, "
            "image BLOB NOT NULL, "
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP "
            ");"
            ).arg(m_tableName);

    if (!query.exec(createTableQuery)) {
        qWarning()  << __FILE__ << __LINE__ << QString("Failed to create table %1: %2").arg(m_tableName, query.lastError().text());
        return false;
    }

    // Создаем индекс для ускорения поиска по glyph_id
    if (!query.exec(QString("CREATE INDEX IF NOT EXISTS idx_glyph_id ON %1(glyph_id)").arg(m_tableName))) {
        qWarning() << __FILE__ << __LINE__ << QString("Failed to create index %1: %2").arg(m_tableName, query.lastError().text());
    }

    return true;
}

bool ImageManager::getGlyphImage(const FontContext &font, QSharedPointer<ImageContext> &image)
{
    if (image.isNull())
    {
        return false;
    }

    if (image->type() == ImageContext::NoneType)
        return false;

    if (image->type() == ImageContext::ImagePreview || image->type() == ImageContext::ImageTemplate)
    {
        return m_glyphRenderer->renderGlyph(font, image);
    }

    return false;
}

bool ImageManager::saveDrawImage(const QSharedPointer<DrawContext> &draw)
{
    if (draw.isNull())
        return false;

    if (!draw->isValid())
        return false;

    if (draw->image().isNull())
    {
        draw->setImage(QSharedPointer<QImage>::create(draw->size(), QImage::Format_ARGB32));
    }

    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QString(
                      "INSERT INTO %1 (glyph_id, width, height, image) "
                      "VALUES (:glyph_id, :width, :height, :image)"
                      ).arg(m_tableName));

    query.bindValue(":glyph_id", draw->glyphId());
    query.bindValue(":width", draw->size().width());
    query.bindValue(":height", draw->size().height());

    // Сериализуем QImage в QByteArray
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    draw->image()->save(&buffer, "PNG"); // Используем PNG для сохранения качества
    query.bindValue(":image", imageData);

    if (!query.exec()) {
        qWarning() << __FILE__ << __LINE__
                   << "Failed to save image:" << query.lastError().text();
        return false;
    }

    return true;
}

bool ImageManager::findDrawImage(int glyphId, QSharedPointer<DrawContext> &draw)
{
    if (glyphId < 0)
        return false;

    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen()) {
        qWarning() << __FILE__ << __LINE__ << "Database is not open";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(QString(
                      "SELECT id, glyph_id, width, height, image, created_at FROM %1 WHERE glyph_id = :glyph_id "
                      "ORDER BY created_at DESC LIMIT 1"
                      ).arg(m_tableName));

    query.bindValue(":glyph_id", glyphId);

    if (!query.exec()) {
        qWarning() << __FILE__ << __LINE__
                   << "Failed to load image:" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        qDebug() << "No image found for glyph_id:" << glyphId;
        return false;
    }

    return loadFromQuery(std::move(query), draw);
}

bool ImageManager::loadOrCreateDrawImage(int glyphId, QSharedPointer<DrawContext> &draw)
{
    if (findDrawImage(glyphId, draw))
    {
        return true;
    }

    draw->setGlyphId(glyphId);
    return saveDrawImage(draw);
}

bool ImageManager::loadFromQuery(QSqlQuery query, QSharedPointer<DrawContext> &draw)
{
    QByteArray imageData = query.value("image").toByteArray();
    QSharedPointer<QImage> image = QSharedPointer<QImage>::create();
    if (!image->loadFromData(imageData)) {
        qWarning() << __FILE__ << __LINE__ << "Failed to load image from blob data";
        return false;
    }

    try {
        draw->setId(query.value("id").toInt());
        draw->setGlyphId(query.value("glyph_id").toInt());
        int width = (query.value("width").toInt());
        int height = (query.value("height").toInt());
        draw->setSize(QSize(width, height));
        QByteArray imageData = query.value("image").toByteArray();
        QSharedPointer<QImage> image = QSharedPointer<QImage>::create();
        if (!image->loadFromData(imageData)) {
            qWarning() << __FILE__ << __LINE__ << "Failed to load image from blob data";
            // return false;
        }
        qDebug() << __FILE__ << __LINE__ << *(image.data());
    } catch (const std::exception &e) {
        qCritical() << "Error parsing profile data:" << e.what();
        return false;
    }

    return true;
}
