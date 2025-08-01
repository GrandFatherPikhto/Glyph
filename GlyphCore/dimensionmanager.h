#ifndef BITMAPDIMENSIONS_H
#define BITMAPDIMENSIONS_H

#include <QObject>
#include <QHash>
#include <QSharedPointer>
#include <QMargins>

#include "bitmapdimension.h"


class DimensionManager : public QObject
{
    Q_OBJECT
public:
    DimensionManager(QObject *parent = nullptr) : QObject(parent) {}
    ~DimensionManager() {};

    QSharedPointer<BitmapDimension> registerBitmapDimension(int value)
    {
        auto it = m_bitmapDimensions.find(value);

        if (it == m_bitmapDimensions.end())
        {
            // Создаем новое измерение если не найдено
            auto dimension = QSharedPointer<BitmapDimension>::create(value);
            m_bitmapDimensions.insert(value, dimension);
            generateBitmapDimensionValues();

            return dimension;
        }

        // Увеличиваем счетчик существующего измерения
        it.value()->incrementCounter();

        return it.value();
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

    bool setGridMargins(int bitmapDimension, const GridPaddings &paddings)
    {
        auto dimIt = m_bitmapDimensions.find(bitmapDimension);
        if (dimIt == m_bitmapDimensions.end())
        {
            return false;
        }

        dimIt.value()->paddings(paddings);

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

Q_DECLARE_METATYPE(DimensionManager)

#endif // BITMAPDIMENSIONS_H

