#ifndef BITMAPDIMENSIONS_H
#define BITMAPDIMENSIONS_H

#include <QObject>
#include <QHash>
#include <QSharedPointer>
#include <QMargins>

#include "bitmapdimension.h"


class BitmapDimensions : public QObject
{
    Q_OBJECT
public:
    BitmapDimensions(QObject *parent = nullptr);
    ~BitmapDimensions();

    bool registerBitmapDimension(int value)
    {
        auto it = m_bitmapDimensions.find(value);

        if (it == m_bitmapDimensions.end())
        {
            // Создаем новое измерение если не найдено
            auto dimension = QSharedPointer<BitmapDimension>::create(value, QMargins(0,0,0,0));
            m_bitmapDimensions.insert(value, dimension);
            generateBitmapDimensionValues();

            return true;
        }

        // Увеличиваем счетчик существующего измерения
        it.value()->incrementCounter();

        return false;
    }

    bool releaseBitmapDimension(int value)
    {
        auto it = m_bitmapDimensions.find(value);
        if (it == m_bitmapDimensions.end())
            return false;

        // Декрементируем счетчик
        int newCount = it.value()->decrementCounter();
        
        // Удаляем если счетчик достиг нуля
        if (newCount <= 0)
        {
            m_bitmapDimensions.erase(it);
            generateBitmapDimensionValues();

            return true;
        }
        
        return false;
    }

    QSharedPointer<BitmapDimension> bitmapDimension(int value)
    {
        auto it = m_bitmapDimensions.find(value);
        if (it == m_bitmapDimensions.end())
        {
            return QSharedPointer<BitmapDimension>();
        }

        return it.value();
    }

    bool setGridMargins(int bitmapDimension, const QMargins &margins)
    {
        auto dimIt = m_bitmapDimensions.find(bitmapDimension);
        if (dimIt == m_bitmapDimensions.end())
        {
            return false;
        }

        dimIt.value()->margins(margins);

        return true;
    }

    QSharedPointer<BitmapDimension> bitmapDimensionAt(int pos)
    {
        if (pos < 0 || pos >= m_dimensions.size())
            return QSharedPointer<BitmapDimension>();
        
        int bitmapDimension = m_dimensions.at(pos);
        auto it = m_bitmapDimensions.find(bitmapDimension);

        if (it == m_bitmapDimensions.end())
        {
            return QSharedPointer<BitmapDimension>();
        }

        return it.value();
    }

    int size()
    {
        return m_dimensions.size();
    }

signals:
    
    void bitmapDimensionsChanged ();

private:

    void generateBitmapDimensionValues()
    {
        m_dimensions = m_bitmapDimensions.keys();
        std::sort(m_dimensions.begin(), m_dimensions.end());
        emit bitmapDimensionsChanged();
    }

    QHash<int /* dimension */, QSharedPointer<BitmapDimension>> m_bitmapDimensions;
    QVector<int> m_dimensions;
};

#endif // BITMAPDIMENSIONS_H

#if 0
void generateBitmapDimensionValues()
{
    m_bitmapDimensionValues = m_bitmapDimensions.keys();
    std::sort(m_bitmapDimensionValues.begin(), m_bitmapDimensionValues.end());
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_bitmapDimensions.size();

    emit bitmapDimensionsChanged();
}

QSharedPointer<BitmapDimension> bitmapDimension(int value)
{
    auto it = m_bitmapDimensions.find(value);
    if (it == m_bitmapDimensions.end())
    {
        return QSharedPointer<BitmapDimension>();
    }

    return it.value();
}

QSharedPointer<BitmapDimension> bitmapDimensionAt(int pos)
{
    if (pos < 0 || pos >= m_bitmapDimensionValues.size())
        return QSharedPointer<BitmapDimension>();
    
    int bitmapDimension = m_bitmapDimensionValues.at(pos);
    auto it = m_bitmapDimensions.find(bitmapDimension);

    if (it == m_bitmapDimensions.end())
    {
        return QSharedPointer<BitmapDimension>();
    }

    return it.value();
}

bool setBitmapDimensionGridMargins(int bitmapDimension, const QMargins &margins)
{
    auto dimIt = m_bitmapDimensions.find(bitmapDimension);
    if (dimIt == m_bitmapDimensions.end())
    {
        return false;
    }

    dimIt.value()->setGridMargins(margins);

    return true;
}

bool appendBitmapDimension(int bitmapDimension)
{
    auto it = m_bitmapDimensions.find(bitmapDimension);
    if (it == m_bitmapDimensions.end())
    {
        // Создаем новое измерение если не найдено
        auto dimension = QSharedPointer<BitmapDimension>::create(bitmapDimension, QMargins(0,0,0,0));
        m_bitmapDimensions.insert(bitmapDimension, dimension);
        generateBitmapDimensionValues();

        return true;
    }

    // Увеличиваем счетчик существующего измерения
    it.value()->incrementCounter();

    return false;
}


bool removeBitmapDimension(int value)
{
    auto it = m_bitmapDimensions.find(value);
    if (it == m_bitmapDimensions.end())
        return false;

    // Декрементируем счетчик
    int newCount = it.value()->decrementCounter();
    
    // Удаляем если счетчик достиг нуля
    if (newCount <= 0)
    {
        m_bitmapDimensions.erase(it);
        generateBitmapDimensionValues();

        return true;
    }
    
    return false;
}

#endif