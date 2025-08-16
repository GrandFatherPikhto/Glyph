#ifndef __IGLYPHRENDER_H__
#define __IGLYPHRENDER_H__

#include <QObject>
#include <QImage>
#include <QSharedPointer>

class IGlyphRenderer {
public:
    virtual ~IGlyphRenderer() = default;

    virtual bool renderGlyph(const QColor &color, const QColor &bgColor, const QSize &targetSize) = 0;
    virtual QString rendererName() const = 0;

    int left() const { return m_left; }
    int top() const { return m_left; }
    int width() const { return m_left; }
    int height() const { return m_height; }

    void setLeft(int value) { m_left = value; }
    void setTop(int value) { m_left = value; }
    void setWidth(int value) { m_width = value; }
    void setHeight(int value) { m_height = value; }

protected:
    // Конструктор protected - нельзя создать экземпляр абстрактного класса
    explicit IGlyphRenderer() : 
        m_left(0), m_top(0), m_width(0), m_height(0), m_size(0, 0) {}

    int m_left;
    int m_top;
    int m_width;
    int m_height;

    QSize m_size;
};

#endif // __IGLYPHRENDER_H__
