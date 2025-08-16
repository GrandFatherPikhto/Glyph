#include "fontutils.h"

QString winDefaultFontPath()
{
    /**
     * https://learn.microsoft.com/en-us/windows/win32/shell/knownfolderid
     * https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath
     */
    PWSTR pszPath = nullptr;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Fonts, 0, NULL, &pszPath);

    if (SUCCEEDED(hr)) {
        std::wcout << L"Default Font Directory: " << pszPath << std::endl;
        CoTaskMemFree(pszPath); // Free the allocated memory
        return QString(pszPath);
    } else {
        std::wcerr << L"Failed to get font directory. HRESULT: " << hr << std::endl;
    }

    return QString();
}

QFont fontByPath(const QString &fontPath)
{
    // Сначала загружаем шрифт (если он еще не загружен)
    int fontId = QFontDatabase::addApplicationFont(fontPath);

    if (fontId == -1) {
        qWarning() << "Не удалось загрузить шрифт из" << fontPath;
        return QFont();
    }

    // Получаем список семейств шрифтов для загруженного шрифта
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

    if (fontFamilies.isEmpty()) {
        qWarning() << "Не удалось определить семейство шрифтов";
        return QFont();
    }

    // Создаем QFont
    QFont customFont(fontFamilies.at(0));
    customFont.setPointSize(12);

    return customFont;
}

bool getWinSystemFonts(QMap<QString, QString> &fonts, const HKEY &rootKey)
{
    QString regPath("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
    HKEY hKey;
    uint32_t pathSize = 0;
    DWORD index = 0;
    bool found = false;

    // qDebug() << __FILE__ << __LINE__ << registerPath;
    if (RegOpenKeyEx(rootKey, regPath.toStdWString().c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return true;
    }

    QString name;
    QString fileName;
    while (getRegisterKeyValue(hKey, index, name, fileName)) {
        if (!(name.isEmpty() || fileName.isEmpty()))
        {
            fonts.insert(fileName, name);
        }
        index++;
    }

    RegCloseKey(hKey);

    return true;
}

bool getRegisterKeyValue(const HKEY &hKey, DWORD index, QString &name, QString &data)
{
    TCHAR valueName[256];
    TCHAR valueData[MAX_PATH];
    DWORD valueNameSize, valueDataSize, type;

    name = QString();
    data = QString ();

    valueNameSize = sizeof(valueName) / sizeof(TCHAR);
    valueDataSize = sizeof(valueData);

    if (RegEnumValue(hKey, index, valueName, &valueNameSize, NULL,
                     &type, (LPBYTE)valueData, &valueDataSize) != ERROR_SUCCESS) {
        return false;
    }

    TCHAR tchFontName[MAX_PATH] = {0};
    TCHAR tchFontPath[MAX_PATH] = {0};

    if (_tcsstr(valueName, tchFontName) != NULL)
    {
        name = QString::fromWCharArray(valueName);
    }

    if (_tcsstr(valueData, tchFontPath) != NULL)
    {
        data = QString::fromWCharArray(valueData);
    }

    return true;
}

bool appendOrUpdateFontContext(QSqlDatabase db, const QString & tableName, FontContext &context)
{
    QSqlQuery query(db);

    if(!query.prepare(QString(
                           "INSERT INTO %1 (name, path, family, style, system) "
                           "VALUES (:name, :path, :family, :style, :system) "
                           "ON CONFLICT(name, path) DO UPDATE SET "
                           "family = excluded.family, style = excluded.style, system = excluded.system "
                           "RETURNING id"
                           ).arg(tableName)))
    {
        qWarning() << __FILE__ << __LINE__ << "Can't prepare " << query.lastQuery() << query.lastError();
        return false;
    }

    query.bindValue(":name", context.name());
    query.bindValue(":path", context.path());
    query.bindValue(":family", context.family());
    query.bindValue(":style", context.style());
    query.bindValue(":system", context.system());

    if (!query.exec())
    {
        qWarning() << __FILE__ << __LINE__ << "Can't append font" << context << query.lastError();
        return false;
    }


    context.setId(query.lastInsertId().toInt());

    return true;
}

bool isFontSupportedByFreeType(FontContext &context)
{
    FT_Library library;
    FT_Face face;

    if (FT_Init_FreeType(&library)) return false;

    QString filePath = context.path() + "/" + context.name();
    FT_Error ftError = FT_New_Face(
        library,
        filePath.toLocal8Bit().constData(),
        0,
        &face);

    if (!ftError)
    {
        context.setFamily(QString(face->family_name));
        context.setStyle(QString(face->style_name));
    }

    FT_Done_FreeType(library);

    return (ftError == 0);
}
