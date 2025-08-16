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

#define NOT_RESTORE_SETTINGS

class AppContext;
class FontManager;
class ProfileManager;

class GLYPHCORE_EXPORT AppSettings : public QObject {
    Q_OBJECT
public:
    explicit AppSettings(AppContext *appContext);
    ~AppSettings ();

    const QString & appDataPath () const;

    inline bool setValue(const QString &name, const QVariant &value)
    {
        bool contains = m_values.keys().contains(name);
        m_values.insert(name, value);
        emit valueChanged(name, value);
        return contains;
    }

    inline QVariant value(const QString &name)
    {
        auto it = m_values.find(name);
        if (it == m_values.end())
            return QVariant();
        
        return it.value();
    }

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
    FontManager *m_fontManager;
    ProfileManager *m_profileManager;

    QMap<QString, QVariant> m_values;
    QString m_appDataPath;
};

#endif // APPSETTINGS_H_
