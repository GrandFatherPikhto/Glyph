#ifndef GLYPHFILTER_H_
#define GLYPHFILTER_H_

#include <QObject>

#include "glyphcontext.h"
#include "GlyphCore_global.h"

class AppContext;

class GLYPHCORE_EXPORT GlyphFilter : public QObject {
    Q_OBJECT
public:
    explicit GlyphFilter(AppContext *appContext, const std::shared_ptr<QVector<QSharedPointer<GlyphContext>>>& source = nullptr);
    ~GlyphFilter();

    void setupSignals ();

    void setSource(const std::shared_ptr<QVector<QSharedPointer<GlyphContext>>>& source);
    void setCharacterFilter(const QString& filter);
    void setBitmapDimension(const QSharedPointer<BitmapDimension>& dimension);
    const QVector<QSharedPointer<GlyphContext>>& filteredGlyphs() const;
    void setSortComparator(std::function<bool(const GlyphContext&, const GlyphContext&)> comparator);
    int size();
    QSharedPointer<GlyphContext> at(int pos);

signals:
    void glyphsFiltered();
    void sourceChanged ();
    void sourceGlyphChanged(QSharedPointer<GlyphContext> glyphContext);

private:
    void sort ();
    void applyGlyphFilter(QSharedPointer<GlyphContext> glyphContext);
    void applyFilter();

    std::shared_ptr<QVector<QSharedPointer<GlyphContext>>> m_source;
    QVector<QSharedPointer<GlyphContext>> m_filtered;
    QString m_characterFilter;
    QSharedPointer<BitmapDimension> m_bitmapDimension;
    std::function<bool(const GlyphContext&, const GlyphContext&)> m_comparator;

    // DI
    AppContext *m_appContext;
};

#endif // GLYPHFILTER_H_