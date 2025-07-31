#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QMargins>

#include "gridpaddings.h"
#include "bitmapdimension.h"
#include "bitmapdimensions.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("DAE");
    QCoreApplication::setApplicationName("Glyph");

    Q_INIT_RESOURCE(main); //< Нужно, если ресурс добавлен в виде библиотеки (main.qrc)

#if 0
    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        if (msg.startsWith("qt.text.font.db"))
            return; // подавляем
        if (msg.startsWith("OpenType support missing for"))
            return; // подавляем
        if (msg.startsWith("OpenType support missing for"))
            return; // подавляем
        if (msg.startsWith("Adding font:"))
            return;

        fprintf(stdout, "%s\n", msg.toLocal8Bit().constData());
    });
#endif

    qRegisterMetaType<QMargins>("QMargins");
    // qRegisterMetaTypeStreamOperators<QMargins>("QMargins");

    qRegisterMetaType<GridPaddings>("GridPaddings");
    // qRegisterMetaTypeStreamOperators<GridPaddings>("GridPaddings");

    qRegisterMetaType<BitmapDimensions>("BitmapDimensions");
    // qRegisterMetaTypeStreamOperators<BitmapDimensions>("BitmapDimensions");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages) {
        const QString baseName = "Glyph_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow window;
    window.setWindowIcon(QIcon(":/app/icons/app_icon")); // Из ресурсов
    window.show();
    return a.exec();
}
