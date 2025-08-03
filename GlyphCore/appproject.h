#ifndef APPPROJECT_H
#define APPPROJECT_H

#include <QObject>

#include "GlyphCore_global.h"

class AppContext;
class AppSettings;
class DbCore;

class GLYPHCORE_EXPORT AppProject : public QObject
{
    Q_OBJECT
public:
    explicit AppProject(AppContext *appContext);
    ~AppProject();

    bool createProject(const QString &projectName);

signals:

private:
    AppContext *m_appContext;
    AppSettings *m_appSettings;

    QString m_projectName;
    QString m_projectPath;
    DbCore *m_dbCore;
};

#endif // APPPROJECT_H
