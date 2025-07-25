#include "fontscriptmodel.h"

FontScriptModel::FontScriptModel(AppContext *appContext, QObject *parent)
    : QAbstractListModel{parent}
    , m_appContext(appContext)
{
    initNames();
    QObject::connect(m_appContext->fontManager(), &FontManager::glyphFontChanged, this, [=](const QFont &font){
        Q_UNUSED(font);
        emit layoutChanged();
    });
}

int FontScriptModel::rowCount(const QModelIndex &parent) const
{
    return m_appContext->fontManager()->fontScriptSize();
}

QVariant FontScriptModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_appContext->fontManager()->fontScriptSize())
        return QVariant();

    QString name = QString("Unknown Script");
    QChar::Script item = m_appContext->fontManager()->fontScriptAt(index.row());
    auto it = m_names.find(static_cast<QChar::Script>(item));
    if (it != m_names.end())
    {
        name = it.value();
    }

    switch(role) {
        case Qt::DisplayRole:
            return name;   // Название
        case Qt::EditRole: // Добавьте эту строку
            return name;   // Название
        case Qt::UserRole:
            return item;  // ID
        default:
            return QVariant();
    }
}

void FontScriptModel::initNames ()
{
    m_names.clear ();
    m_names = {
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

