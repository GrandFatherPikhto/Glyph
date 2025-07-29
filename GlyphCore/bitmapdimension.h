#ifndef BITMAPDIMENSION_H
#define BITMAPDIMENSION_H

#include <QMargins>

class BitmapDimension
{
public:
    BitmapDimension(int bitmapDimension, const QMargins &margins);

    void margins(const QMargins &value) { m_margins = value;  }
    const QMargins & margins() { return m_margins; }

    int bitmapDimension() {return m_bitmapDimension; }

    int incrementCounter() { return ++m_counter; }
    int decrementCounter () { return --m_counter; }
    int counter() { return m_counter; }

private:
    QMargins m_margins;
    int m_bitmapDimension;
    int m_counter;
};

#endif // BITMAPDIMENSION_H
