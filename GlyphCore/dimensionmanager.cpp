#include "dimensionmanager.h"
#include "appcontext.h"

DimensionManager::DimensionManager(AppContext *appContext) 
    : QObject{appContext}, 
    m_appContext(appContext) 
{

}

DimensionManager::~DimensionManager() 
{

};

QSharedPointer<BitmapDimension> DimensionManager::registerBitmapDimension(int value)
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

bool DimensionManager::releaseBitmapDimension(int value)
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

QSharedPointer<BitmapDimension> DimensionManager::bitmapDimension(int value)
{
    auto it = m_bitmapDimensions.find(value);
    if (it == m_bitmapDimensions.end())
    {
        return QSharedPointer<BitmapDimension>();
    }

    return it.value();
}

bool DimensionManager::setGridMargins(int bitmapDimension, const GridPaddings &paddings)
{
    auto dimIt = m_bitmapDimensions.find(bitmapDimension);
    if (dimIt == m_bitmapDimensions.end())
    {
        return false;
    }

    dimIt.value()->paddings(paddings);

    return true;
}

QSharedPointer<BitmapDimension> DimensionManager::bitmapDimensionAt(int pos)
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

int DimensionManager::size()
{
    return m_dimensions.size();
}

void DimensionManager::generateBitmapDimensionValues()
{
    m_dimensions = m_bitmapDimensions.keys();
    std::sort(m_dimensions.begin(), m_dimensions.end());
    emit bitmapDimensionsChanged();
}
