#ifndef GLYPHIMAGES_H_
#define GLYPHIMAGES_H_

#include <QObject>
#include <QSharedPointer>
#include <QHash>

#include "glyphkey.h"
#include "glyphimage.h"
#include "appcontext.h"

class GlyphImages : public QObject {
    Q_OBJECT
public:
    GlyphImages(AppContext *appContext);
    ~GlyphImages();

private:
    GlyphKey m_key;
    
    QHash<GlyphKey, QSharedPointer<GlyphImage>> m_templates;
    QHash<GlyphKey, QSharedPointer<GlyphImage>> m_previews;
    QHash<GlyphKey, QSharedPointer<GlyphImage>> m_draws;

    AppContext *m_appContext;
};

#endif // GLYPHIMAGES_H_