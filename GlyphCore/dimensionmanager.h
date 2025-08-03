#ifndef BITMAPDIMENSIONS_H
#define BITMAPDIMENSIONS_H

#include <QObject>
#include <QHash>
#include <QSharedPointer>
#include <QMargins>

#include "bitmapdimension.h"
#include "GlyphCore_global.h"

class AppContext;

class GLYPHCORE_EXPORT DimensionManager : public QObject
{
    Q_OBJECT
public:
    DimensionManager(AppContext *appContext);
    ~DimensionManager();

    QSharedPointer<BitmapDimension> registerBitmapDimension(int value);
    bool releaseBitmapDimension(int value);
    QSharedPointer<BitmapDimension> bitmapDimension(int value);
    bool setGridMargins(int bitmapDimension, const GridPaddings &paddings);
    QSharedPointer<BitmapDimension> bitmapDimensionAt(int pos);
    int size();

signals:
    
    void bitmapDimensionsChanged ();

private:

    void generateBitmapDimensionValues();

    QHash<int /* dimension */, QSharedPointer<BitmapDimension>> m_bitmapDimensions;
    QVector<int> m_dimensions;
    AppContext *m_appContext;
};

Q_DECLARE_METATYPE(DimensionManager)

#endif // BITMAPDIMENSIONS_H

