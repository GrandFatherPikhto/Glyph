#ifndef IMAGECONTEXT_H
#define IMAGECONTEXT_H

#include <QVariant>
#include <QImage>
#include <QChar>
#include <QSize>
#include <QDebug>
#include <QColor>
#include <QBuffer>
#include <QIODevice>

#include "GlyphMeta_global.h"

#define IMAGECONTEXT_SHARED_POINTER

class GLYPHMETA_EXPORT ImageContext
{
public:
    typedef enum ImageType_t {
        NoneType = 0x0,
        ImageTemplate = 0x01,
        ImagePreview = 0x02,
        ImageDraw = 0x03
    } ImageType;

    explicit ImageContext(int id = -1, int glyph_id = -1, const QChar &ch = QChar(), const QSize &sz = QSize(), ImageType type = NoneType, const QColor &color = QColor(), const QColor &bgColor = QColor(), bool dirty = true, int l = -1, int t = -1, int w = -1, int h = -1)
        : m_id(id)
        , m_glyphId(glyph_id)
        , m_character(ch)
        , m_size(sz)
#ifdef IMAGECONTEXT_SHARED_POINTER
        , m_image(QSharedPointer<QImage>())
#endif
        , m_left(l)
        , m_top(t)
        , m_width(w)
        , m_height(h)
        , m_dirty(dirty)
        , m_color(color)
        , m_bgColor(color)
    {}

    ImageContext(const ImageContext &context)
        : m_id(context.m_id)
        , m_glyphId(context.m_glyphId)
        , m_character(context.m_character)
        , m_size(context.size())
        , m_image(context.m_image)
        , m_left(context.m_left)
        , m_top(context.m_top)
        , m_width(context.m_width)
        , m_height(context.m_height)
        , m_dirty(context.m_dirty)
        , m_color(context.m_color)
        , m_bgColor(context.m_bgColor)
    {}

    ~ImageContext() {}

    int id() const { return m_id; }
    int glyphId() const { return m_glyphId; }
    ImageType type() { return m_type; }
    int left() const { return m_left; }
    int top() const { return m_top; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    bool dirty() const { return m_dirty; }
    const QColor & color() const { return m_color; }
    const QColor & bgColor() const { return m_bgColor; }
    const QChar & character() const { return m_character; }
    const QSize & size() const { return m_size; }

#ifdef IMAGECONTEXT_SHARED_POINTER
    QSharedPointer<QImage> image() const { return m_image; }
#else
    const QImage & image() { return m_image; }
#endif

    bool isValid() const { return m_character == QChar() || m_size != QSize(); }

    QString typeName() const {
        switch (m_type)
        {
        case ImageTemplate: return "Template";
        case ImagePreview: return "Preview";
        case ImageDraw: return "Draw";
        default: return "Unknown";
        }

        return QString ();
    }

    void setId(int value) { m_id = value; }
    void setGlyphId(int value) { m_glyphId = value; }
    void setType(ImageType value) { m_type = value; }
    void setLeft(int value) { m_left = value; }
    void setTop(int value) { m_top = value; }
    void setWidth(int value) { m_width = value; }
    void setHeight(int value) { m_height = value; }
    void setCharacter(const QChar &ch) { m_character = ch; }
    void setSize (const QSize &value) { m_size = value; }

#ifdef IMAGECONTEXT_SHARED_POINTER
    void setImage(QSharedPointer<QImage> img) { m_image = img; }
#else
    void setImage(QSharedPointer<QImage> img) { m_image = img; }
#endif
    void setDirty(bool value = true) { m_dirty = value; }
    void setColor(const QColor &value) { m_color = value; }
    void setBgColor(const QColor &value) { m_bgColor = value; }

    inline bool operator== (const ImageContext &context) const {
        return (
            m_id == context.m_id
            && m_glyphId == context.m_glyphId
            && m_type == context.m_type
            && m_character == context.m_character
            && m_size == context.m_size
            && m_left == context.m_left
            && m_top == context.m_top
            && m_height == context.m_height
            && m_width == context.m_width
            && m_dirty == context.m_dirty
            && m_color == context.m_color
            && m_bgColor == context.m_bgColor
            && m_image == context.m_image
            );
    }

    inline bool operator!= (const ImageContext &context) const {
        return (
            m_id != context.m_id
            || m_glyphId != context.m_glyphId
            || m_type != context.m_type
            || m_character != context.m_character
            || m_size != context.m_size
            || m_left != context.m_left
            || m_top != context.m_top
            || m_height != context.m_height
            || m_width != context.m_width
            || m_dirty != context.m_dirty
            || m_color != context.m_color
            || m_bgColor != context.m_bgColor
            || m_image != context.m_image
            );
    }

    inline bool operator< (const ImageContext &context) const {
        if (m_character != context.m_character)
            return m_character < context.m_character;
        if (m_type != context.m_type)
            return m_type < context.m_type;
        return m_size.width() < context.m_size.width();
    }

    inline const ImageContext & operator= (const ImageContext &context)
    {
        m_id = context.m_id;
        m_glyphId = context.m_glyphId;
        m_type = context.m_type;
        m_character = context.m_character;
        m_size = context.m_size;
        m_left = context.m_left;
        m_top = context.m_top;
        m_width = context.m_width;
        m_height = context.m_height;
        m_dirty = context.m_dirty;
        m_image = context.m_image;
        m_color = context.m_color;
        m_bgColor = context.m_bgColor;

        return *this;
    }

    operator QVariant() const {
        if(QMetaType::fromName("ProfileContext").isValid()) {
            return QVariant::fromValue(*this);
        } else {
            // Fallback для случая, когда GridPaddings не зарегистрирован
            QVariantMap map;

            map["id"] = m_id;
            map["glyph_id"] = m_glyphId;
            map["character"] = m_character;
            map["type"] = m_type;
            map["size"] = m_size;
            map["left"] = m_left;
            map["top"] = m_top;
            map["width"] = m_width;
            map["height"] = m_height;
            map["dirty"] = m_dirty;
            map["color"] = m_color;
            map["background_color"] = m_bgColor;

#ifdef IMAGECONTEXT_SHARED_POINTER
            // Сохраняем изображение в байтовом массиве
            if (m_image && !m_image->isNull()) {
                QBuffer buffer;
                buffer.open(QIODevice::WriteOnly);
                m_image->save(&buffer, "PNG");
                map["image_data"] = buffer.data();
                map["image_format"] = "PNG";
            } else {
                map["image_data"] = QByteArray();
                map["image_format"] = QString();
            }
#else
            map["image"] = m_image;
#endif
            return map;
        }
    }

#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator << (QDataStream &out, const ImageContext &context);
    friend QDataStream & operator >> (QDataStream &in, ImageContext &context);
    friend QDataStream & operator << (QDataStream &out, const QSharedPointer<ImageContext> &context);
    friend QDataStream & operator >> (QDataStream &in, QSharedPointer<ImageContext> &context);
#endif // QT_NO_DATASTREAM

private:
    QChar m_character;
    QSize m_size; //< Target size
    ImageType m_type;

    int m_id;
    int m_glyphId;

    int m_left;
    int m_top;
    int m_width;
    int m_height;

    bool m_dirty;

    QColor m_color;
    QColor m_bgColor;

#ifdef IMAGECONTEXT_SHARED_POINTER
    QSharedPointer<QImage> m_image;
#else
    QImage m_image;
#endif
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator << (QDataStream &out, const ImageContext &context)
{
    out << context.m_id
        << context.m_glyphId
        << context.m_character
        << context.m_size
        << context.m_dirty
        << context.m_left
        << context.m_top
        << context.m_width
        << context.m_height
        << context.m_color
        << context.m_bgColor
        << static_cast<quint32>(context.m_type);



#ifdef IMAGECONTEXT_SHARED_POINTER
    // Сохраняем изображение, если указатель не нулевой
    bool hasImage = !context.m_image.isNull();
    out << hasImage;
    if (hasImage) {
        out << *context.m_image.data();
    }
#else
    out << context.m_image;
#endif

    return out;
}

inline QDataStream & operator >> (QDataStream &in, ImageContext &context)
{
    qint32 type;
    in >> context.m_id
        >> context.m_glyphId
        >> context.m_character
        >> context.m_size
        >> context.m_dirty
        >> context.m_left
        >> context.m_top
        >> context.m_width
        >> context.m_height
        >> context.m_color
        >> context.m_bgColor
        >> type;

    context.m_type = static_cast<ImageContext::ImageType>(type);

#ifdef IMAGECONTEXT_SHARED_POINTER
    bool hasImage;
    in >> hasImage;
    if (hasImage) {
        QImage image;
        in >> image;
        context.m_image = QSharedPointer<QImage>::create(image);
    } else {
        context.m_image.clear();
    }
#else
    in >> context.m_image;
#endif
    return in;
}


inline QDataStream & operator << (QDataStream &out, const QSharedPointer<ImageContext> &context)
{
    if (context.isNull()) {
        // Сохраняем флаг, что указатель пустой
        out << false;
        return out;
    }

    // Сохраняем флаг, что указатель не пустой
    out << true;

    // Сериализуем данные объекта
    out << context->m_id
        << context->m_glyphId
        << context->m_character
        << context->m_size
        << context->m_dirty
        << context->m_left
        << context->m_top
        << context->m_width
        << context->m_height
        << context->m_color
        << context->m_bgColor
        << static_cast<quint32>(context->m_type);

#ifdef IMAGECONTEXT_SHARED_POINTER
    // Сохраняем изображение, если указатель не нулевой
    bool hasImage = !context->m_image.isNull();
    out << hasImage;
    if (hasImage) {
        out << *context->m_image.data();
    }
#else
    out << context->m_image;
#endif

    return out;
}

inline QDataStream & operator >> (QDataStream &in, QSharedPointer<ImageContext> &context)
{
    bool hasData;
    in >> hasData;

    if (!hasData) {
        context.clear();
        return in;
    }

    // Создаем новый объект
    context = QSharedPointer<ImageContext>::create();

    // Десериализуем данные
    in >> context->m_id
        >> context->m_glyphId
        >> context->m_character
        >> context->m_size
        >> context->m_dirty
        >> context->m_left
        >> context->m_top
        >> context->m_width
        >> context->m_height
        >> context->m_color
        >> context->m_bgColor;

    quint32 type;
    in >> type;
    context->m_type = static_cast<ImageContext::ImageType>(type);

#ifdef IMAGECONTEXT_SHARED_POINTER
    bool hasImage;
    in >> hasImage;
    if (hasImage) {
        context->m_image = QSharedPointer<QImage>::create();
        in >> *context->m_image.data();
    } else {
        context->m_image.clear();
    }
#else
    in >> context->m_image;
#endif

    return in;
}

#endif // QT_NO_DATASTREAM


#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator <<(QDebug debug, const ImageContext &context)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "ImageContext(Id: "
                    << context.id()
                    << ", Glyph ID: "
                    << context.glyphId()
                    << ", Character: "
                    << context.character()
                    << ", Target Size: "
                    << context.size()
                    << ", Type: "
                    << context.typeName()
                    << ", Dirty: "
                    << context.dirty()
                    << ", Color: "
                    << context.color()
                    << ", Bg Color: "
                    << context.bgColor()
                    << ", Valid: "
                    << context.isValid()
                    << ", Image Dimension: (l: "
                    << context.left()
                    << ", t: "
                    << context.top()
                    << ", w: "
                    << context.width()
                    << ", h: "
                    << context.height()
                    << "))";

    return debug;
}

inline QDebug operator <<(QDebug debug, const QSharedPointer<ImageContext> context)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "ImageContext(Id: "
                    << context->id()
                    << ", Glyph ID: "
                    << context->glyphId()
                    << ", Character: "
                    << context->character()
                    << ", Target Size: "
                    << context->size()
                    << ", Type: "
                    << context->typeName()
                    << ", Dirty: "
                    << context->dirty()
                    << ", Color: "
                    << context->color()
                    << ", Bg Color: "
                    << context->bgColor()
                    << ", Valid: "
                    << context->isValid()
                    << ", Image Dimension: (l: "
                    << context->left()
                    << ", t: "
                    << context->top()
                    << ", w: "
                    << context->width()
                    << ", h: "
                    << context->height()
                    << "))";

    return debug;
}

#endif

Q_DECLARE_METATYPE(ImageContext)

#endif // IMAGECONTEXT_H
