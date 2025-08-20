#ifndef DRAWCONTEXT_H
#define DRAWCONTEXT_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QVariant>
#include <QDebug>
#include <QDataStream>
#include <QImage>

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT DrawContext {
    public:
    explicit DrawContext(int id = -1, int glyph_id = -1, const QSize &size = QSize(), bool dirty = true)
        : m_id(id)
        , m_glyphId(glyph_id)
        , m_size(size)
        , m_dirty(dirty)
    {}

    ~DrawContext() {}

    int id() const { return m_id; }
    int glyphId() const { return m_glyphId; }
    const QSize & size() const { return m_size; }
    bool dirty() const { return m_dirty; }
    QSharedPointer<QImage> image() const { return m_image; }

    void setId(int value = -1) { m_id = value; }
    void setGlyphId(int value = -1) { m_glyphId = value; }
    void setSize(const QSize &value = QSize()) { m_size = value; }
    void setDirty(bool value = true) { m_dirty = value; }
    void setImage(const QSharedPointer<QImage> &value) { m_image = value; }

    bool isValid() const { return (
        m_glyphId >= 0
        && m_size != QSize()
    );}

    inline const bool operator== (const DrawContext &context) const
    {
        return (
            m_id == context.m_id
            && m_glyphId == context.m_glyphId
            && m_size == context.m_size
            && m_dirty == context.m_dirty
            && m_image == context.m_image
        );
    }

    inline const bool operator!= (const DrawContext &context) const
    {
        return (
            m_id != context.m_id
            || m_glyphId != context.m_glyphId
            || m_size != context.m_size
            || m_dirty != context.m_dirty
            || m_image != context.m_image
        );
    }

    inline const DrawContext & operator= (const DrawContext &context)
    {
        m_id = context.m_id;
        m_glyphId = context.m_glyphId;
        m_size = context.m_size;
        m_dirty = context.m_dirty;
        m_image = context.m_image;

        return *this;
    }

#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator << (QDataStream &out, const DrawContext &context);
    friend QDataStream & operator >> (QDataStream &in, DrawContext &context);
#endif


    private:
        int m_id;
        int m_glyphId;
        QSize m_size;
        bool m_dirty;
        QSharedPointer<QImage> m_image;
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator << (QDataStream &out, const DrawContext &context)
{
    out << context.m_id
        << context.m_glyphId
        << context.m_size
        << context.m_dirty;
    
        // Сохраняем изображение, если указатель не нулевой
    bool hasImage = !context.m_image.isNull();
    out << hasImage;
    if (hasImage) {
        out << *context.m_image.data();
    }
    return out;
}

inline QDataStream & operator >> (QDataStream &in, DrawContext &context)
{
    in  >> context.m_id
        >> context.m_glyphId
        >> context.m_size
        >> context.m_dirty;

        bool hasImage;
    in >> hasImage;
    if (hasImage) {
        context.m_image = QSharedPointer<QImage>::create();
        in >> *context.m_image.data();
    } else {
        context.m_image.clear();
    }

    return in;
}
#endif // QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator <<(QDebug debug, const DrawContext &context)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "DrawContext( ID: "
                    << context.id()
                    << ", Glyph ID: "
                    << context.glyphId()
                    << ", Dimension: "
                    << context.size()
                    << ", Dirty: "
                    << context.dirty()
                    << ", Valid: "
                    << context.isValid()
                    << ")";

    return debug;
}
#endif // QT_NO_DEBUG_OUTPUT

Q_DECLARE_METATYPE(DrawContext)

#endif // DRAWCONTEXT_H
