#ifndef GLYPHFILTER_H_
#define GLYPHFILTER_H_

#include <QObject>

#include "glyphcontext.h"

class GlyphFilter : public QObject {
    Q_OBJECT
public:
    explicit GlyphFilter(
        QObject *parent = nullptr
        , const std::shared_ptr<QVector<QSharedPointer<GlyphContext>>>& source = nullptr
    ) 
    : QObject{parent}
    , m_source(source ? source : std::make_shared<QVector<QSharedPointer<GlyphContext>>>()) 
    {
        if (!(m_source->isDetached() && m_source->isEmpty()))
        {
            applyFilter();
        }

        setupSignals();
    };

    ~GlyphFilter() {}

    void setupSignals ()
    {
        QObject::connect(this, &GlyphFilter::sourceChanged, this, &GlyphFilter::applyFilter);
        QObject::connect(this, &GlyphFilter::sourceGlyphChanged, this, &GlyphFilter::applyGlyphFilter);
    }

    void setSource(const std::shared_ptr<QVector<QSharedPointer<GlyphContext>>>& source) {
        m_source = source;
        applyFilter();
    }

    void setCharacterFilter(const QString& filter) {
        m_characterFilter = filter;
        applyFilter();
    }

    void setBitmapDimension(const QSharedPointer<BitmapDimension>& dimension) {
        m_bitmapDimension = dimension;
        applyFilter();
    }

    const QVector<QSharedPointer<GlyphContext>>& filteredGlyphs() const {
        return m_filtered;
    }

    void setSortComparator(std::function<bool(const GlyphContext&, const GlyphContext&)> comparator) {
        m_comparator = comparator;
        applyFilter();
    }

    int size() { return m_filtered.size(); }
    QSharedPointer<GlyphContext> at(int pos) {
        if (pos >= m_filtered.size())
            return QSharedPointer<GlyphContext>();

        return m_filtered.at(pos);
    }

signals:
    void glyphsFiltered();
    void sourceChanged ();
    void sourceGlyphChanged(QSharedPointer<GlyphContext> glyphContext);

private:
    void sort ()
    {
        if (m_comparator) {
            std::sort(m_filtered.begin(), m_filtered.end(),
                [this](const auto& a, const auto& b) { return m_comparator(*a, *b); });
        }
    }

    void applyGlyphFilter(QSharedPointer<GlyphContext> glyphContext)
    {
        int idx = m_filtered.indexOf(glyphContext);
        if (glyphContext->temporary() && idx >= 0 && idx < m_filtered.size())
        {
            m_filtered.remove(idx);
            emit glyphsFiltered ();
        } else if (!m_filtered.contains(glyphContext) && !glyphContext->temporary())
        {
            m_filtered.append(glyphContext);
            emit glyphsFiltered ();
        }
    }

    void applyFilter() {
        m_filtered.clear();
        auto& vec = *m_source;
        for (const auto& glyph : vec) {
            if ((m_characterFilter.isEmpty() || m_characterFilter.contains(glyph->character())) &&
                (m_bitmapDimension.isNull() || glyph->bitmapDimension() == m_bitmapDimension))
            {
                m_filtered.append(glyph);
            }
        }

        sort();

        emit glyphsFiltered ();
    }

    std::shared_ptr<QVector<QSharedPointer<GlyphContext>>> m_source;
    QVector<QSharedPointer<GlyphContext>> m_filtered;
    QString m_characterFilter;
    QSharedPointer<BitmapDimension> m_bitmapDimension;
    std::function<bool(const GlyphContext&, const GlyphContext&)> m_comparator;
};

#endif // GLYPHFILTER_H_
