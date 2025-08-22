#ifndef APPSETTINGS_H_
#define APPSETTINGS_H_

#include <QObject>
#include <QFont>
#include <QString>
#include <QColor>
#include <QPoint>
#include <QMargins>
#include <QRect>
#include <QSharedPointer>
#include <QVariant>

#include "GlyphCore_global.h"

// #define NOT_RESTORE_SETTINGS

class AppContext;

class GLYPHCORE_EXPORT AppSettings : public QObject {
    Q_OBJECT
public:
    explicit AppSettings(AppContext *appContext);
    ~AppSettings ();

    void clearAllSettings();

    const QString & appDataPath () const;

    bool setValue(const QString &name, const QVariant &value);
    QVariant value(const QString &name);

signals:
    void valueChanged(const QString &name, const QVariant &value);

    void glyphPreviewMarginsChanged(const QMargins &value);
    void glyphWidgetMarginsChanged(const QMargins &value);

private:
    void setupValues();
    void setupSignals();

    void saveAppSettings();
    void restoreAppSettings();
    
    void initSettings ();

    const QString & initAppDataCatalog();


    AppContext *m_appContext;

    QMap<QString, QVariant> m_values;
    QString m_appDataPath;
};

#endif // APPSETTINGS_H_
