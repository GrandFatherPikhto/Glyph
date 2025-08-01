#if 0
void GlyphManager::filterGlyphs ()
{
    m_filtered.clear();
    for (const auto &glyphPtr : m_glyphs)
    {
        if (
               (m_characterFilter.isEmpty() || m_characterFilter.contains(glyphPtr->character()))
            && (m_bitmapDimension.isNull() || glyphPtr->bitmapDimension() == m_bitmapDimension)
        )
        {
            m_filtered.append(glyphPtr);
        }
    }

    std::sort(m_filtered.begin(), m_filtered.end(),
        [](const QSharedPointer<GlyphContext>& a, const QSharedPointer<GlyphContext>& b) {
            return a->glyphEntry()->character().unicode() < b->glyphEntry()->character().unicode();
        });
}

void GlyphManager::setCharacterFilter(const QString &filter)
{
    m_characterFilter = filter;
    filterGlyphs();
}

int GlyphManager::filteredSize ()
{
    return m_filtered.size();
}

bool GlyphManager::removeFilteredGlyphByPos(int pos)
{
    if (pos >= m_filtered.size())
        return false;

    auto glyph = m_filtered.at(pos);
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << *(glyph.data());
    remove(glyph->key());
    // m_filtered.remove(pos);
    emit glyphsHashChanged();

    return true;
}

QSharedPointer<GlyphContext> GlyphManager::filteredAt(int pos)
{
    if (pos >= m_filtered.size())
        return QSharedPointer<GlyphContext>();

    return m_filtered.at(pos);
}

#endif

