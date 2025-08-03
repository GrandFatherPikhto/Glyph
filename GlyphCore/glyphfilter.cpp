#include "glyphfilter.h"
#include "appcontext.h"

GlyphFilter::GlyphFilter(AppContext *appContext, const std::shared_ptr<QVector<QSharedPointer<GlyphContext>>>& source)
    : QObject{appContext}

    , m_source(source ? source : std::make_shared<QVector<QSharedPointer<GlyphContext>>>())    
{
    if (!(m_source->isDetached() && m_source->isEmpty()))
    {
        applyFilter();
    }

    setupSignals();
}

GlyphFilter::~GlyphFilter()
{

}

void GlyphFilter::setupSignals()
{
    QObject::connect(this, &GlyphFilter::sourceChanged, this, &GlyphFilter::applyFilter);
    QObject::connect(this, &GlyphFilter::sourceGlyphChanged, this, &GlyphFilter::applyGlyphFilter);
}

void GlyphFilter::setSource(const std::shared_ptr<QVector<QSharedPointer<GlyphContext>>>& source)
{
    m_source = source;
    applyFilter();
}

void GlyphFilter::setCharacterFilter(const QString& filter) {
    m_characterFilter = filter;
    applyFilter();
}

void GlyphFilter::setBitmapDimension(const QSharedPointer<BitmapDimension>& dimension) {
    m_bitmapDimension = dimension;
    applyFilter();
}

const QVector<QSharedPointer<GlyphContext>>& GlyphFilter::filteredGlyphs() const {
    return m_filtered;
}

void GlyphFilter::setSortComparator(std::function<bool(const GlyphContext&, const GlyphContext&)> comparator) {
    m_comparator = comparator;
    applyFilter();
}

int GlyphFilter::size() { return m_filtered.size(); }

QSharedPointer<GlyphContext> GlyphFilter::at(int pos) {
    if (pos >= m_filtered.size())
        return QSharedPointer<GlyphContext>();

    return m_filtered.at(pos);
}

void GlyphFilter::sort ()
{
    if (m_comparator) {
        std::sort(m_filtered.begin(), m_filtered.end(),
            [this](const auto& a, const auto& b) { return m_comparator(*a, *b); });
    }
}

void GlyphFilter::applyGlyphFilter(QSharedPointer<GlyphContext> glyphContext)
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

void GlyphFilter::applyFilter() {
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
