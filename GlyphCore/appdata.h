#ifndef APPDATA_H
#define APPDATA_H

#include <QObject>

#include "GlyphCore_global.h"

class AppContext;

class GLYPHCORE_EXPORT AppData : public QObject
{
    Q_OBJECT
public:
    explicit AppData(AppContext *appContext);
    ~AppData();

    const QString & appDataPath() { return m_appDataPath; }

private:
    void initAppDataCatalog();

    AppContext *m_appContext;
    QString m_appDataPath;
};

#endif // APPDATA_H
