#ifndef GLYPHDOCK_H
#define GLYPHDOCK_H

#include <QDockWidget>
#include <QObject>
#include <QWidget>
#include <QString>

class GlyphDock : public QDockWidget
{
    Q_OBJECT
public:
    GlyphDock(const QString &title, QWidget *parent = nullptr);
    ~GlyphDock();

};

#endif // GLYPHDOCK_H
