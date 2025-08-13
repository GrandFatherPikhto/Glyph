#include "mainwindow.h"

#include <QtCore>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>
#include <QMetaType>

#include "profilecontext.h"
#include "gridpaddings.h"
#include "glyphcontext.h"

void registerMetaTypes ()
{
    qRegisterMetaType<GridPaddings>("GridPaddings");
    QMetaType::registerConverter<GridPaddings, QVariant>();

    qRegisterMetaType<ProfileContext>("ProfileContext");
    // qRegisterMetaTypeStreamOperators<ProfileContext>("ProfileContext");    
    QMetaType::registerConverter<ProfileContext, QVariant>();

    qRegisterMetaType<GlyphContext>("GlyphContext");
    // qRegisterMetaTypeStreamOperators<ProfileContext>("ProfileContext");    
    QMetaType::registerConverter<GlyphContext, QVariant>();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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
