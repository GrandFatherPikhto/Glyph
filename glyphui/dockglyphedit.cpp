#include "dockglyphedit.h"
#include "ui_dockglyphedit.h"

#include "appcontext.h"
#include "appsettings.h"
#include "glyphpreview.h"
#include "glyphmodel.h"
#include "glyphmanager.h"
#include "bitmapdimensionmodel.h"
#include "bitmapdimension.h"
#include "dimensionmanager.h"
#include "glyphfilter.h"

DockGlyphEdit::DockGlyphEdit(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyphEdit)
    , m_appContext(appContext)
    , m_appSettings(nullptr)
    , m_glyphManager(nullptr)
    , m_glyphPreview(nullptr)
    , m_glyphModel(nullptr)
    , m_bitmapDimensionModel(nullptr)
{
    ui->setupUi(this);

    setupVariables ();
    setupDockGlyphEditModels();
    setupDockGlyphEditUI();
    setupSignals();
    restoreDefaultValues();
}

DockGlyphEdit::~DockGlyphEdit()
{
    delete ui;
}

void DockGlyphEdit::setupVariables ()
{
    Q_ASSERT(m_appContext != nullptr && m_appContext->appSettings() != nullptr && m_appContext->glyphManager());

    m_appSettings = m_appContext->appSettings();
    m_glyphManager = m_appContext->glyphManager ();
    m_dimensionManager = m_appContext->dimensionManager ();
    m_appSettings = m_appContext->appSettings();
    m_glyphFilter = m_appContext->glyphFilter();
}

void DockGlyphEdit::restoreDefaultValues ()
{
    ui->spinBoxGlyphSize->setValue(m_appSettings->glyphSize());
    ui->spinBoxBitmapDimension->setValue(m_appSettings->bitmapDimension());
}

void DockGlyphEdit::setupDockGlyphEditModels ()
{
    m_glyphModel = new GlyphModel(m_appContext, this);
    ui->tableGlyphs->setModel(m_glyphModel);

    m_bitmapDimensionModel = new BitmapDimensionModel(m_appContext, this);
    ui->listViewDimensionManager->setModel(m_bitmapDimensionModel);
}

void DockGlyphEdit::setupDockGlyphEditUI ()
{
    m_glyphPreview = new GlyphPreview(m_appContext, this);

    if (ui->groupBoxGlyphPreview->layout() != nullptr)
    {
        ui->groupBoxGlyphPreview->layout()->replaceWidget(ui->widgetGlyphPreview, m_glyphPreview);
    }
}

void DockGlyphEdit::setupSignals ()
{
    QObject::connect(m_glyphManager, &GlyphManager::glyphChanged, this, [=](){
    });

    QObject::connect(m_dimensionManager, &DimensionManager::bitmapDimensionsChanged, this, [=](){

    });

    QObject::connect(ui->tableGlyphs, &QTableView::clicked, this, &DockGlyphEdit::clickGlyph);
    QObject::connect(ui->tableGlyphs, &QTableView::doubleClicked, this, &DockGlyphEdit::doubleClickGlyph);

    QObject::connect(ui->listViewDimensionManager->selectionModel(), &QItemSelectionModel::currentChanged, this, &DockGlyphEdit::syncInDimensionManager);

    QObject::connect(ui->spinBoxLeftCells, &QSpinBox::valueChanged, this, &DockGlyphEdit::syncOutDimensionManager);
    QObject::connect(ui->spinBoxTopCells, &QSpinBox::valueChanged, this, &DockGlyphEdit::syncOutDimensionManager);
    QObject::connect(ui->spinBoxRightCells, &QSpinBox::valueChanged, this, &DockGlyphEdit::syncOutDimensionManager);
    QObject::connect(ui->spinBoxBottomCells, &QSpinBox::valueChanged, this, &DockGlyphEdit::syncOutDimensionManager);

    QObject::connect(ui->spinBoxGlyphSize, &QSpinBox::valueChanged, m_appSettings, &AppSettings::setGlyphSize);
    QObject::connect(ui->spinBoxBitmapDimension, &QSpinBox::valueChanged, m_appSettings, &AppSettings::setBitmapDimension);

    QObject::connect(ui->pushButtonOffsetReset, &QPushButton::clicked, m_glyphManager, &GlyphManager::glyphOffsetReset);
    QObject::connect(ui->pushButtonOffsetMoveLeft, &QPushButton::clicked, m_glyphManager, &GlyphManager::glyphOffsetLeft);
    QObject::connect(ui->pushButtonOffsetMoveTop, &QPushButton::clicked, m_glyphManager, &GlyphManager::glyphOffsetUp);
    QObject::connect(ui->pushButtonOffsetMoveRight, &QPushButton::clicked, m_glyphManager, &GlyphManager::glyphOffsetRight);
    QObject::connect(ui->pushButtonOffsetMoveBottom, &QPushButton::clicked, m_glyphManager, &GlyphManager::glyphOffsetDown);
}

void DockGlyphEdit::syncOutDimensionManager (int value)
{
    Q_UNUSED(value)

    if (!ui->listViewDimensionManager->selectionModel()->hasSelection())
        return;

    QModelIndex idx = ui->listViewDimensionManager->selectionModel()->currentIndex();
    int bitmapDimensionValue = idx.data(Qt::UserRole).toInt();
    QSharedPointer<BitmapDimension> dimensions = m_dimensionManager->bitmapDimension(bitmapDimensionValue);

    if (dimensions.isNull())
        return;

    GridPaddings paddings (
        ui->spinBoxLeftCells->value()
        , ui->spinBoxTopCells->value()
        , ui->spinBoxRightCells->value()
        , ui->spinBoxBottomCells->value()
    );
    dimensions->paddings(paddings);
}

void DockGlyphEdit::syncInDimensionManager (const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if (!ui->listViewDimensionManager->selectionModel()->hasSelection())
        return;

    if (!previous.isValid())
        return;

    int dimension = current.data(Qt::UserRole).toInt();
    QSharedPointer<BitmapDimension> bitmapDimension = m_dimensionManager->bitmapDimension(dimension);

    if (bitmapDimension.isNull())
        return;

    ui->spinBoxLeftCells->setValue(bitmapDimension->paddings().left());
    ui->spinBoxTopCells->setValue(bitmapDimension->paddings().top());
    ui->spinBoxRightCells->setValue(bitmapDimension->paddings().right());
    ui->spinBoxBottomCells->setValue(bitmapDimension->paddings().bottom());
}

void DockGlyphEdit::saveGeometryAndState()
{
    QSettings settings;
    settings.beginGroup("DockGlyphEdit");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitter", ui->splitter->saveState());
    settings.endGroup ();

}

void DockGlyphEdit::restoreGeometryAndState() 
{
    QSettings settings;
    settings.beginGroup("DockGlyphEdit");
    restoreGeometry(settings.value("geometry").toByteArray());
    ui->splitter->restoreState(settings.value("splitter").toByteArray());
    settings.endGroup ();
}

void DockGlyphEdit::doubleClickGlyph(const QModelIndex &index)
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << index << m_glyphFilter->at(index.row());
    // emit m_glyphManager->removeFilteredGlyphAt(index.row());
}

void DockGlyphEdit::clickGlyph(const QModelIndex &index)
{

}

void DockGlyphEdit::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    restoreGeometryAndState ();
}

void DockGlyphEdit::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    saveGeometryAndState ();
}

void DockGlyphEdit::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    saveGeometryAndState ();
}
