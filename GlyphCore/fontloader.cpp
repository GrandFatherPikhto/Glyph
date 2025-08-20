#include <QDir>

#include "fontloader.h"
#include "fontutils.h"

FontLoader::FontLoader(const QString &tableName, const QString &dbFile, QObject *parent)
    : QObject{parent}
    , m_tableName(tableName)
    , m_dbFile(dbFile)
{
    ::getWinSystemFonts(m_sysfonts);
    ::getWinSystemFonts(m_sysfonts, HKEY_CURRENT_USER);
}

FontLoader::~FontLoader()
{

}

void FontLoader::loadFonts(const QStringList &fontDirs)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QString("thread_%1").arg(GetCurrentThreadId()));
    db.setDatabaseName(m_dbFile);
    if (!db.open())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't open DB" << m_dbFile;
        return;
    }

    int totalCount = 0;
    for (int i = 0; i < fontDirs.size(); ++i) {
        int counter = processFontDir(db, fontDirs.at(i));
        totalCount += counter;
    }

    if (db.isOpen())
    {
        db.close();
    }

    emit finished(totalCount);
}

int FontLoader::processFontDir(QSqlDatabase db, const QString &path)
{
    QDir fontsDir(QDir::toNativeSeparators(path));
    QStringList filters;
    filters << "*.ttf" << "*.ttc" << "*.otf" << "*.otc"
            << "*.pfa" << "*.pfb" << "*.woff" << "*.woff2"
            << "*.fon" << "*.fnt" << "*.bdf" << "*.pcf";

    int counter = 0;
    const auto files = fontsDir.entryList(QDir::Files);
    for (const QString &fileName : files) {
        qDebug() << fileName;
        FontContext context(fontsDir.absolutePath(), fileName);

        auto it = m_sysfonts.find(fileName);
        if (it != m_sysfonts.end())
        {
            context.setSystem(it.value());
        }

        if (::isFontSupportedByFreeType(context)) {
            if(::appendOrUpdateFontContext(db, m_tableName, context))
            {
                // qDebug() << context;
            }
        }
    }

    return counter;
}


