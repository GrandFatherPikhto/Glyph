#ifndef BITMAPDIMENSION_H
#define BITMAPDIMENSION_H

#include "gridpaddings.h"

class BitmapDimension
{
public:
    BitmapDimension(int bitmapDimension = -1, const GridPaddings &paddings = GridPaddings())
        : m_paddings(paddings)
        , m_bitmapDimension(bitmapDimension)
        , m_counter(0)
    {}

    BitmapDimension(const BitmapDimension &bitmapDimension)
        : m_paddings(bitmapDimension.m_paddings)
        , m_bitmapDimension(bitmapDimension.m_bitmapDimension)
        , m_counter(bitmapDimension.m_counter)
    {}

    BitmapDimension(BitmapDimension *bitmapDimension)
        : m_paddings(bitmapDimension->m_paddings)
        , m_bitmapDimension(bitmapDimension->m_bitmapDimension)
        , m_counter(bitmapDimension->m_counter)
    {}

    BitmapDimension & operator = (const BitmapDimension &bitmapDimension)
    {
        m_bitmapDimension = bitmapDimension.m_bitmapDimension;
        m_paddings = bitmapDimension.m_paddings;
        m_counter = bitmapDimension.m_counter;
        
        return *this;
    }

    bool operator == (const BitmapDimension &bitmapDimension)
    {
        return m_bitmapDimension == bitmapDimension.m_bitmapDimension;
    }

    void paddings(const GridPaddings &value) { m_paddings = value; }
    GridPaddings & paddings() { return m_paddings; }
    const GridPaddings & paddings() const { return m_paddings; }

    int bitmapDimension() const { return m_bitmapDimension; }
    // void setBitmapDimension(int value) { m_bitmapDimension = value; }

    int incrementCounter() { return ++m_counter; }
    int decrementCounter () { return --m_counter; }
    int counter() { return m_counter; }

#ifndef QT_NO_DATASTREAM
    friend QDataStream & operator << (QDataStream &out, const BitmapDimension &dimension);
    friend QDataStream & operator >> (QDataStream &in, BitmapDimension &dimension);
#endif

private:
    GridPaddings m_paddings;
    int m_bitmapDimension;
    int m_counter;
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator << (QDataStream &out, const BitmapDimension &dimension)
{
    out << dimension.m_bitmapDimension
        << dimension.m_paddings;
    return out;
}

inline QDataStream & operator >> (QDataStream &in, BitmapDimension &dimension)
{
    in  >> dimension.m_bitmapDimension
        >> dimension.m_paddings;

    return in;
}
#endif

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator<<(QDebug debug, const BitmapDimension &dimension)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "BitmapDimension("
        << dimension.bitmapDimension()
        << ", "
        << dimension.paddings()
        << ")";
    return debug;
}
#endif

Q_DECLARE_METATYPE(BitmapDimension)

#endif // BITMAPDIMENSION_H
