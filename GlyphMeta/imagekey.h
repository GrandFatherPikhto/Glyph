#ifndef IMAGEKEY_H_
#define IMAGEKEY_H_

#include <QChar>
#include <QHash>

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT ImageKey {

public:
    enum ImageType {
        ImageTemplate = 0x01,
        ImagePreview  = 0x02,
        ImageDraw = 0x03
    };

    ImageKey(const QChar &character, int bitmapDimension, ImageType type) 
        : m_unicode(character.unicode())
        , m_bitmapDimension(bitmapDimension)
        , m_type(type)    
    {}

    ImageKey(quint32 unicode, int bitmapDimension, ImageType type)
        : m_unicode(unicode)
        , m_bitmapDimension(bitmapDimension)
        , m_type(type)
    {}

    ImageKey(const ImageKey &imageKey)
        : m_unicode(imageKey.m_unicode)
        , m_bitmapDimension(imageKey.m_bitmapDimension)
        , m_type(imageKey.m_type)
    {}

    ImageKey & operator = (const ImageKey &key)
    {
        m_unicode = key.m_unicode;
        m_bitmapDimension = key.m_bitmapDimension;
        m_type = key.m_type;

        return *this;
    }

    bool inline operator == (const ImageKey &key) const
    {
        return (
               m_unicode == key.m_unicode
            && m_bitmapDimension == key.m_bitmapDimension
            && m_type == key.m_type
        );
    }

    bool operator > (const ImageKey &key)
    {
        if (m_unicode != key.m_unicode)
            return key.m_unicode > m_unicode;

        if (key.m_bitmapDimension != m_bitmapDimension)
            return key.m_bitmapDimension > m_bitmapDimension;
        
        
        return key.m_type > m_type;
    }

    bool operator < (const ImageKey &key)
    {
        if (m_unicode != key.m_unicode)
            return key.m_unicode < m_unicode;

        if (key.m_bitmapDimension != m_bitmapDimension)
            return key.m_bitmapDimension < m_bitmapDimension;
        
        return key.m_type < m_type;
    }

    quint32 unicode () const { return m_unicode; }
    int bitmapDimension () const { return m_bitmapDimension; }
    ImageType type() const { return m_type; }

private:
    quint32 m_unicode;
    int m_bitmapDimension;
    ImageType m_type;    
};

// inline uint qHash(const GlyphKey &key, uint seed = 0);
inline uint qHash(const ImageKey &key, uint seed) {
    
    QtPrivate::QHashCombine hash;
    
    seed = hash(seed, key.unicode());
    seed = hash(seed, key.bitmapDimension());
    seed = hash(seed, key.type());

    return seed;
}

#endif // IMAGEKEY_H_
