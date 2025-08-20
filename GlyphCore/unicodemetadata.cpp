#include "appcontext.h"
#include "unicodemetadata.h"
#include "appsettings.h"
#include "dbmanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>

UnicodeMetadata::UnicodeMetadata(AppContext *appContext)
    : QObject{appContext}
    , m_appContext(appContext)
    , m_dbManager(appContext->dbManager())
    , m_scriptTable("script_data")
    , m_categoryTable("category_data")
    , m_decompositionTable("decomposition_data")
{
    // QObject::connect(m_appContext, &AppContext::valuesInited, this, &UnicodeMetadata::setupValues);
    setupValues ();
}

void UnicodeMetadata::setupValues ()
{
    loadScripts();
    loadCategories();
    loadDecompositions();

    initTable(m_scriptTable);
    initTable(m_categoryTable);
    initTable(m_decompositionTable);

    fillScriptTable();
    fillCategoryTable();
    fillDecompositionTable();

    setupSignals ();
}

void UnicodeMetadata::setupSignals ()
{

}

bool UnicodeMetadata::fillScriptTable()
{
    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
    {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    if (!query.exec(QString("DELETE FROM %1").arg(m_scriptTable)))
    {
        qWarning() << __FILE__ << __LINE__ << "Delete failed:" << query.lastError().text();
        return false;
    }

    db.transaction();
    // Явно указываем имена столбцов и параметры
    query.prepare(QString("INSERT OR REPLACE INTO %1 "
                          "(id, name) "
                          "VALUES (:id, :name)").arg(m_scriptTable));
    for (auto it = m_scripts.constBegin(); it != m_scripts.constEnd(); ++it) {
        query.bindValue(":id", it.key());
        query.bindValue(":name", it.value());

        if (!query.exec()) {
            qWarning() << __FILE__ << __LINE__ << "Failed to insert script" <<  query.lastError().text() << query.lastQuery();
            db.rollback();

            return false;
        }
    }


    if (!db.commit()) {
        qWarning()  << __FILE__ << __LINE__ << "Commit failed:" << db.lastError().text();
        return false;
    }

    return true;
}

bool UnicodeMetadata::fillCategoryTable()
{
    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
    {
        qWarning() << __FILE__ << __LINE__ << "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    if (!query.exec(QString("DELETE FROM %1").arg(m_categoryTable)))
    {
        qWarning() << __FILE__ << __LINE__ << "Delete failed:" << query.lastError().text();
        return false;
    }

    db.transaction();
    // Явно указываем имена столбцов и параметры
    query.prepare(QString("INSERT OR REPLACE INTO %1 "
                          "(id, name) "
                          "VALUES (:id, :name)").arg(m_categoryTable));
    for (auto it = m_categories.constBegin(); it != m_categories.constEnd(); ++it) {
        // qDebug() << "Script:" << script << "Name:" << scriptName;
        query.bindValue(":id", it.key());
        query.bindValue(":name", it.value());

        if (!query.exec()) {
            qWarning() << __FILE__ << __LINE__ << "Failed to insert script" <<  query.lastError().text() << query.lastQuery();
            db.rollback();

            return false;
        }
    }


    if (!db.commit()) {
        qWarning()  << __FILE__ << __LINE__ << "Commit failed:" << db.lastError().text();
        return false;
    }

    return true;
}

bool UnicodeMetadata::fillDecompositionTable()
{
    QSqlDatabase db = QSqlDatabase::database("main");
    if (!db.isOpen())
    {
        qWarning() << __FILE__ << __LINE__ <<  "Database is not open!";
        return false;
    }

    QSqlQuery query(db);

    if (!query.exec(QString("DELETE FROM %1").arg(m_decompositionTable)))
    {
        qWarning()  << __FILE__ << __LINE__ << "Delete failed:" << query.lastError().text();
        return false;
    }

    db.transaction();
    // Явно указываем имена столбцов и параметры
    query.prepare(QString("INSERT OR REPLACE INTO %1 "
                          "(id, name) "
                          "VALUES (:id, :name)").arg(m_decompositionTable));
    for (auto it = m_decompositions.constBegin(); it != m_decompositions.constEnd(); ++it) {
        // qDebug() << "Script:" << script << "Name:" << scriptName;
        query.bindValue(":id", it.key());
        query.bindValue(":name", it.value());

        if (!query.exec()) {
            qWarning() << __FILE__ << __LINE__ << "Failed to insert script" <<  query.lastError().text() << query.lastQuery();
            db.rollback();

            return false;
        }
    }


    if (!db.commit()) {
        qWarning() << __FILE__ << __LINE__ << "Commit failed:" << db.lastError().text();
        return false;
    }

    return true;
}


bool UnicodeMetadata::initTable(const QString &tableName)
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
        "id INTEGER NOT NULL, "   // uint32
        "name TEXT, "
        "PRIMARY KEY (id)"        // Добавляем первичный ключ
        ");").arg(tableName);
    
    if (!query.exec(createTableQuery)) {
        qWarning()  << __FILE__ << __LINE__ << QString("Failed to create table %1: %2").arg(tableName, query.lastError().text());
        return false;
    }
    
    // Создаем индекс для ускорения поиска по категории (опционально)
    if (!query.exec(QString("CREATE INDEX IF NOT EXISTS idx_category ON %1(category)").arg(tableName))) {
        qWarning()  << __FILE__ << __LINE__ << QString("Failed to create index %1: %2").arg(tableName, query.lastError().text());
        // Не считаем это критической ошибкой
    }
    
    // qDebug() << QString("Table %1 created successfully").arg(tableName);

    return true;
}


void UnicodeMetadata::loadScripts() {
    m_scripts = {
                 {QChar::Script_Unknown, "Unknown"},
                 {QChar::Script_Inherited, "Inherited"},
                 {QChar::Script_Common, "Common"},
                 {QChar::Script_Adlam, "Adlam"},
                 {QChar::Script_Ahom, "Ahom"},
                 {QChar::Script_AnatolianHieroglyphs, "AnatolianHieroglyphs"},
                 {QChar::Script_Arabic, "Arabic"},
                 {QChar::Script_Armenian, "Armenian"},
                 {QChar::Script_Avestan, "Avestan"},
                 {QChar::Script_Balinese, "Balinese"},
                 {QChar::Script_Bamum, "Bamum"},
                 {QChar::Script_BassaVah, "BassaVah"},
                 {QChar::Script_Batak, "Batak"},
                 {QChar::Script_Bengali, "Bengali"},
                 {QChar::Script_Bhaiksuki, "Bhaiksuki"},
                 {QChar::Script_Bopomofo, "Bopomofo"},
                 {QChar::Script_Brahmi, "Brahmi"},
                 {QChar::Script_Braille, "Braille"},
                 {QChar::Script_Buginese, "Buginese"},
                 {QChar::Script_Buhid, "Buhid"},
                 {QChar::Script_CanadianAboriginal, "CanadianAboriginal"},
                 {QChar::Script_Carian, "Carian"},
                 {QChar::Script_CaucasianAlbanian, "CaucasianAlbanian"},
                 {QChar::Script_Chakma, "Chakma"},
                 {QChar::Script_Cham, "Cham"},
                 {QChar::Script_Cherokee, "Cherokee"},
                 {QChar::Script_Chorasmian, "Chorasmian"},
                 {QChar::Script_Coptic, "Coptic"},
                 {QChar::Script_Cuneiform, "Cuneiform"},
                 {QChar::Script_Cypriot, "Cypriot"},
                 {QChar::Script_CyproMinoan, "CyproMinoan"},
                 {QChar::Script_Cyrillic, "Cyrillic"},
                 {QChar::Script_Deseret, "Deseret"},
                 {QChar::Script_Devanagari, "Devanagari"},
                 {QChar::Script_DivesAkuru, "DivesAkuru"},
                 {QChar::Script_Dogra, "Dogra"},
                 {QChar::Script_Duployan, "Duployan"},
                 {QChar::Script_EgyptianHieroglyphs, "EgyptianHieroglyphs"},
                 {QChar::Script_Elbasan, "Elbasan"},
                 {QChar::Script_Elymaic, "Elymaic"},
                 {QChar::Script_Ethiopic, "Ethiopic"},
                 {QChar::Script_Garay, "Garay"},
                 {QChar::Script_Georgian, "Georgian"},
                 {QChar::Script_Glagolitic, "Glagolitic"},
                 {QChar::Script_Gothic, "Gothic"},
                 {QChar::Script_Grantha, "Grantha"},
                 {QChar::Script_Greek, "Greek"},
                 {QChar::Script_Gujarati, "Gujarati"},
                 {QChar::Script_GunjalaGondi, "GunjalaGondi"},
                 {QChar::Script_Gurmukhi, "Gurmukhi"},
                 {QChar::Script_GurungKhema, "GurungKhema"},
                 {QChar::Script_Han, "Han"},
                 {QChar::Script_Hangul, "Hangul"},
                 {QChar::Script_HanifiRohingya, "HanifiRohingya"},
                 {QChar::Script_Hanunoo, "Hanunoo"},
                 {QChar::Script_Hatran, "Hatran"},
                 {QChar::Script_Hebrew, "Hebrew"},
                 {QChar::Script_Hiragana, "Hiragana"},
                 {QChar::Script_ImperialAramaic, "ImperialAramaic"},
                 {QChar::Script_InscriptionalPahlavi, "InscriptionalPahlavi"},
                 {QChar::Script_InscriptionalParthian, "InscriptionalParthian"},
                 {QChar::Script_Javanese, "Javanese"},
                 {QChar::Script_Kaithi, "Kaithi"},
                 {QChar::Script_Kannada, "Kannada"},
                 {QChar::Script_Katakana, "Katakana"},
                 {QChar::Script_Kawi, "Kawi"},
                 {QChar::Script_KayahLi, "KayahLi"},
                 {QChar::Script_Kharoshthi, "Kharoshthi"},
                 {QChar::Script_KhitanSmallScript, "KhitanSmallScript"},
                 {QChar::Script_Khmer, "Khmer"},
                 {QChar::Script_Khojki, "Khojki"},
                 {QChar::Script_Khudawadi, "Khudawadi"},
                 {QChar::Script_KiratRai, "KiratRai"},
                 {QChar::Script_Lao, "Lao"},
                 {QChar::Script_Latin, "Latin"},
                 {QChar::Script_Lepcha, "Lepcha"},
                 {QChar::Script_Limbu, "Limbu"},
                 {QChar::Script_LinearA, "LinearA"},
                 {QChar::Script_LinearB, "LinearB"},
                 {QChar::Script_Lisu, "Lisu"},
                 {QChar::Script_Lycian, "Lycian"},
                 {QChar::Script_Lydian, "Lydian"},
                 {QChar::Script_Mahajani, "Mahajani"},
                 {QChar::Script_Makasar, "Makasar"},
                 {QChar::Script_Malayalam, "Malayalam"},
                 {QChar::Script_Mandaic, "Mandaic"},
                 {QChar::Script_Manichaean, "Manichaean"},
                 {QChar::Script_Marchen, "Marchen"},
                 {QChar::Script_MasaramGondi, "MasaramGondi"},
                 {QChar::Script_Medefaidrin, "Medefaidrin"},
                 {QChar::Script_MeeteiMayek, "MeeteiMayek"},
                 {QChar::Script_MendeKikakui, "MendeKikakui"},
                 {QChar::Script_MeroiticCursive, "MeroiticCursive"},
                 {QChar::Script_MeroiticHieroglyphs, "MeroiticHieroglyphs"},
                 {QChar::Script_Miao, "Miao"},
                 {QChar::Script_Modi, "Modi"},
                 {QChar::Script_Mongolian, "Mongolian"},
                 {QChar::Script_Mro, "Mro"},
                 {QChar::Script_Multani, "Multani"},
                 {QChar::Script_Myanmar, "Myanmar"},
                 {QChar::Script_Nabataean, "Nabataean"},
                 {QChar::Script_NagMundari, "NagMundari"},
                 {QChar::Script_Nandinagari, "Nandinagari"},
                 {QChar::Script_Newa, "Newa"},
                 {QChar::Script_NewTaiLue, "NewTaiLue"},
                 {QChar::Script_Nko, "Nko"},
                 {QChar::Script_Nushu, "Nushu"},
                 {QChar::Script_NyiakengPuachueHmong, "NyiakengPuachueHmong"},
                 {QChar::Script_Ogham, "Ogham"},
                 {QChar::Script_OlChiki, "OlChiki"},
                 {QChar::Script_OlOnal, "OlOnal"},
                 {QChar::Script_OldHungarian, "OldHungarian"},
                 {QChar::Script_OldItalic, "OldItalic"},
                 {QChar::Script_OldNorthArabian, "OldNorthArabian"},
                 {QChar::Script_OldPermic, "OldPermic"},
                 {QChar::Script_OldPersian, "OldPersian"},
                 {QChar::Script_OldSogdian, "OldSogdian"},
                 {QChar::Script_OldSouthArabian, "OldSouthArabian"},
                 {QChar::Script_OldTurkic, "OldTurkic"},
                 {QChar::Script_OldUyghur, "OldUyghur"},
                 {QChar::Script_Oriya, "Oriya"},
                 {QChar::Script_Osage, "Osage"},
                 {QChar::Script_Osmanya, "Osmanya"},
                 {QChar::Script_PahawhHmong, "PahawhHmong"},
                 {QChar::Script_Palmyrene, "Palmyrene"},
                 {QChar::Script_PauCinHau, "PauCinHau"},
                 {QChar::Script_PhagsPa, "PhagsPa"},
                 {QChar::Script_Phoenician, "Phoenician"},
                 {QChar::Script_PsalterPahlavi, "PsalterPahlavi"},
                 {QChar::Script_Rejang, "Rejang"},
                 {QChar::Script_Runic, "Runic"},
                 {QChar::Script_Samaritan, "Samaritan"},
                 {QChar::Script_Saurashtra, "Saurashtra"},
                 {QChar::Script_Sharada, "Sharada"},
                 {QChar::Script_Shavian, "Shavian"},
                 {QChar::Script_Siddham, "Siddham"},
                 {QChar::Script_SignWriting, "SignWriting"},
                 {QChar::Script_Sinhala, "Sinhala"},
                 {QChar::Script_Sogdian, "Sogdian"},
                 {QChar::Script_SoraSompeng, "SoraSompeng"},
                 {QChar::Script_Soyombo, "Soyombo"},
                 {QChar::Script_Sundanese, "Sundanese"},
                 {QChar::Script_Sunuwar, "Sunuwar"},
                 {QChar::Script_SylotiNagri, "SylotiNagri"},
                 {QChar::Script_Syriac, "Syriac"},
                 {QChar::Script_Tagalog, "Tagalog"},
                 {QChar::Script_Tagbanwa, "Tagbanwa"},
                 {QChar::Script_TaiLe, "TaiLe"},
                 {QChar::Script_TaiTham, "TaiTham"},
                 {QChar::Script_TaiViet, "TaiViet"},
                 {QChar::Script_Takri, "Takri"},
                 {QChar::Script_Tamil, "Tamil"},
                 {QChar::Script_Tangut, "Tangut"},
                 {QChar::Script_Tangsa, "Tangsa"},
                 {QChar::Script_Telugu, "Telugu"},
                 {QChar::Script_Thaana, "Thaana"},
                 {QChar::Script_Thai, "Thai"},
                 {QChar::Script_Tibetan, "Tibetan"},
                 {QChar::Script_Tifinagh, "Tifinagh"},
                 {QChar::Script_Tirhuta, "Tirhuta"},
                 {QChar::Script_Todhri, "Todhri"},
                 {QChar::Script_Toto, "Toto"},
                 {QChar::Script_TuluTigalari, "TuluTigalari"},
                 {QChar::Script_Ugaritic, "Ugaritic"},
                 {QChar::Script_Vai, "Vai"},
                 {QChar::Script_Vithkuqi, "Vithkuqi"},
                 {QChar::Script_Wancho, "Wancho"},
                 {QChar::Script_WarangCiti, "WarangCiti"},
                 {QChar::Script_Yezidi, "Yezidi"},
                 {QChar::Script_Yi, "Yi"},
                 {QChar::Script_ZanabazarSquare, "ZanabazarSquare"},
                 };
}

void UnicodeMetadata::loadCategories() {
    m_categories = {
                    {QChar::Mark_NonSpacing, "NonSpacing (Mn)"},
                    {QChar::Mark_SpacingCombining, "SpacingCombining (Mc)"},
                    {QChar::Mark_Enclosing, "Enclosing (Me)"},
                    {QChar::Number_DecimalDigit, "DecimalDigit (Nd)"},
                    {QChar::Number_Letter, "Letter (Nl)"},
                    {QChar::Number_Other, "Other (No)"},
                    {QChar::Separator_Space, "Space (Zs)"},
                    {QChar::Separator_Line, "Line (Zl)"},
                    {QChar::Separator_Paragraph, "Paragraph (Zp)"},
                    {QChar::Other_Control, "Control (Cc)"},
                    {QChar::Other_Format, "Format (Cf)"},
                    {QChar::Other_Surrogate, "Surrogate (Cs)"},
                    {QChar::Other_PrivateUse, "PrivateUse (Co)"},
                    {QChar::Other_NotAssigned, "NotAssigned (Cn)"},
                    {QChar::Letter_Uppercase, "Uppercase (Lu)"},
                    {QChar::Letter_Lowercase, "Lowercase (Ll)"},
                    {QChar::Letter_Titlecase, "Titlecase (Lt)"},
                    {QChar::Letter_Modifier, "Modifier (Lm)"},
                    {QChar::Letter_Other, "Other (Lo)"},
                    {QChar::Punctuation_Connector, "Connector (Pc)"},
                    {QChar::Punctuation_Dash, "Dash (Pd)"},
                    {QChar::Punctuation_Open, "Open (Ps)"},
                    {QChar::Punctuation_Close, "Close (Pe)"},
                    {QChar::Punctuation_InitialQuote, "InitialQuote (Pi)"},
                    {QChar::Punctuation_FinalQuote, "FinalQuote (Pf)"},
                    {QChar::Punctuation_Other, "Other (Po)"},
                    {QChar::Symbol_Math, "Math (Sm)"},
                    {QChar::Symbol_Currency, "Currency (Sc)"},
                    {QChar::Symbol_Modifier, "Modifier (Sk)"},
                    {QChar::Symbol_Other, "Other (So)"},
                    };
}

void UnicodeMetadata::loadDecompositions() {
    m_decompositions = {
                        {QChar::NoDecomposition, "NoDecomposition"},
                        {QChar::Canonical, "Canonical"},
                        {QChar::Circle, "Circle"},
                        {QChar::Compat, "Compat"},
                        {QChar::Final, "Final"},
                        {QChar::Font, "Font"},
                        {QChar::Fraction, "Fraction"},
                        {QChar::Initial, "Initial"},
                        {QChar::Isolated, "Isolated"},
                        {QChar::Medial, "Medial"},
                        {QChar::Narrow, "Narrow"},
                        {QChar::NoBreak, "NoBreak"},
                        {QChar::Small, "Small"},
                        {QChar::Square, "Square"},
                        {QChar::Sub, "Sub"},
                        {QChar::Super, "Super"},
                        {QChar::Vertical, "Vertical"},
                        {QChar::Wide, "Wide"},
                        };
}

// Геттеры
QString UnicodeMetadata::scriptName(QChar::Script script) const {
    return m_scripts.value(script, "Unknown Script");
}

QString UnicodeMetadata::categoryName(QChar::Category category) const {
    return m_categories.value(category, "Unknown Category");
}

QString UnicodeMetadata::decompositionName(QChar::Decomposition decomposition) const {
    return m_decompositions.value(decomposition, "Unknown Decomposition");
}

const QMap<QChar::Script, QString>& UnicodeMetadata::allScripts() const {
    return m_scripts;
}

const QMap<QChar::Category, QString>& UnicodeMetadata::allCategories() const {
    return m_categories;
}

const QMap<QChar::Decomposition, QString>& UnicodeMetadata::allDecompositions() const {
    return m_decompositions;
}
