#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QString>

#include "GlyphCore_global.h"

class AppContext;
class GlyphProfile;

class GLYPHCORE_EXPORT ProfileManager : public QObject
{
    Q_OBJECT
public:
    ProfileManager(AppContext *appContext);
    ~ProfileManager();

    bool insertOrReplaceProfile(const GlyphProfile &profile);
    bool clearTable();
    const QString & tableName() const;

    bool getProfileById(int id, GlyphProfile &profile);

signals:
    void profilesChanged();

private:
    void setupValues ();
    void setupSignals ();
    bool createTable();

    AppContext *m_appContext;

    QString m_tableName;
};

#endif // PROFILEMANAGER_H
