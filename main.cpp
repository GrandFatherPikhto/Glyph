#include "mainwindow.h"

// #include <QtCore>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>
#include <QMetaType>
#include <QMargins>

#include "profilecontext.h"
#include "gridcontext.h"
#include "glyphcontext.h"
#include "fontcontext.h"
#include "imagecontext.h"

// QMargins -> QVariantMap
QVariant marginsToVariantMap(const QMargins &margins) {
    QVariantMap map;

    map["left"] = margins.left();
    map["top"] = margins.top();
    map["right"] = margins.right();
    map["bottom"] = margins.bottom();

    return map;
}

// QVariantMap -> QMargins
QMargins variantMapToMargins(const QVariant &value) {
    QVariantMap map = value.toMap();
    return QMargins(
        map.value("left", 0).toInt(),
        map.value("top", 0).toInt(),
        map.value("right", 0).toInt(),
        map.value("bottom", 0).toInt()
        );
}

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (msg.contains("Broken filename passed to function")) {
        // Здесь можно поставить точку останова
        qDebug() << "Broken filename warning caught!";
        qDebug() << "Context:" << context.file << "line" << context.line;
    }
    // Стандартная обработка
}

void registerMetaTypes ()
{
    qRegisterMetaType<GridContext>("GridContext");
    QMetaType::registerConverter<GridContext, QVariant>();

    qRegisterMetaType<ProfileContext>("ProfileContext");
    QMetaType::registerConverter<ProfileContext, QVariant>();

    qRegisterMetaType<GlyphContext>("GlyphContext");
    QMetaType::registerConverter<GlyphContext, QVariant>();

    qRegisterMetaType<FontContext>("FontContext");
    QMetaType::registerConverter<FontContext, QVariant>();

    qRegisterMetaType<ImageContext>("ImageContext");
    QMetaType::registerConverter<ImageContext, QVariant>();

    qRegisterMetaType<QMargins>("QMarings");
    QMetaType::registerConverter<QMargins, QVariant>(marginsToVariantMap);
    QMetaType::registerConverter<QVariant, QMargins>(variantMapToMargins);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // qInstallMessageHandler(myMessageHandler);

    QCoreApplication::setOrganizationName("DAE");
    QCoreApplication::setApplicationName("Glyph");

    registerMetaTypes ();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Glyph_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.setWindowIcon(QIcon(":/app/appIcon")); // Из ресурсов
    w.show();
    return a.exec();
}
