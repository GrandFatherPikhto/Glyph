#include "dockglyphedit.h"
#include "ui_dockglyphedit.h"

DockGlyphEdit::DockGlyphEdit(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyphEdit)
    , m_appContext(appContext)
    , m_glyphPreview(nullptr)
    , m_glyphModel(nullptr)
    , m_bitmapDimensionModel(nullptr)
{
    ui->setupUi(this);
    setupDockGlyphEditModels();
    setupDockGlyphEditUI();
    setupSignals();
    restoreDefaultValues();
}

DockGlyphEdit::~DockGlyphEdit()
{
    delete ui;
}

void DockGlyphEdit::restoreDefaultValues ()
{
    ui->spinBoxGlyphSize->setValue(m_appContext->appSettings()->glyphSize());
    ui->spinBoxBitmapDimension->setValue(m_appContext->appSettings()->bitmapDimension());
}

void DockGlyphEdit::setupDockGlyphEditModels ()
{
    m_glyphModel = new GlyphModel(m_appContext, this);
    ui->tableGlyphs->setModel(m_glyphModel);

    m_bitmapDimensionModel = new BitmapDimensionModel(m_appContext, this);
    ui->listViewBitmapDimensions->setModel(m_bitmapDimensionModel);
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
    QObject::connect(m_appContext->glyphManager(), &GlyphManager::glyphDataChanged, this, [=](){
        qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "Glyph Data Changed";
        emit m_glyphModel->layoutChanged();
    });

    QObject::connect(m_appContext->glyphManager()->bitmapDimensions(), &BitmapDimensions::bitmapDimensionsChanged, this, [=](){

    });

    QObject::connect(ui->listViewBitmapDimensions->selectionModel(), QItemSelectionModel::currentChanged, this, &DockGlyphEdit::syncInBitmapDimensions);

    QObject::connect(ui->spinBoxLeftCells, &QSpinBox::valueChanged, this, &DockGlyphEdit::syncOutBitmapDimensions);
    QObject::connect(ui->spinBoxTopCells, &QSpinBox::valueChanged, this, &DockGlyphEdit::syncOutBitmapDimensions);
    QObject::connect(ui->spinBoxRightCells, &QSpinBox::valueChanged, this, &DockGlyphEdit::syncOutBitmapDimensions);
    QObject::connect(ui->spinBoxBottomCells, &QSpinBox::valueChanged, this, &DockGlyphEdit::syncOutBitmapDimensions);

    QObject::connect(ui->spinBoxGlyphSize, &QSpinBox::valueChanged, m_appContext->appSettings(), &AppSettings::setGlyphSize);
    QObject::connect(ui->spinBoxBitmapDimension, &QSpinBox::valueChanged, m_appContext->appSettings(), &AppSettings::setBitmapDimension);

    QObject::connect(ui->pushButtonOffsetReset, &QPushButton::clicked, m_appContext->glyphManager(), &GlyphManager::setGlyphOffsetReset);
    QObject::connect(ui->pushButtonOffsetMoveLeft, &QPushButton::clicked, m_appContext->glyphManager(), &GlyphManager::setGlyphOffsetMoveLeft);
    QObject::connect(ui->pushButtonOffsetMoveTop, &QPushButton::clicked, m_appContext->glyphManager(), &GlyphManager::setGlyphOffsetMoveTop);
    QObject::connect(ui->pushButtonOffsetMoveRight, &QPushButton::clicked, m_appContext->glyphManager(), &GlyphManager::setGlyphOffsetMoveRight);
    QObject::connect(ui->pushButtonOffsetMoveBottom, &QPushButton::clicked, m_appContext->glyphManager(), &GlyphManager::setGlyphOffsetMoveDown);
}

void DockGlyphEdit::syncOutBitmapDimensions (int value)
{
    Q_UNUSED(value)

    if (!ui->listViewBitmapDimensions->selectionModel()->hasSelection())
        return;

    QModelIndex idx = ui->listViewBitmapDimensions->selectionModel()->currentIndex();
    int dimension = idx.data(Qt::UserRole).toInt();
    QSharedPointer<BitmapDimension> dim = m_appContext->glyphManager()->bitmapDimensions()->bitmapDimension(dimension);

    if (dim.isNull())
        return;

    QMargins margins (
            ui->spinBoxLeftCells->value()
        , ui->spinBoxTopCells->value()
        , ui->spinBoxRightCells->value()
        , ui->spinBoxBottomCells->value()
    );
    dim->margins(margins);
}

void DockGlyphEdit::syncInBitmapDimensions (const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)

    if (!ui->listViewBitmapDimensions->selectionModel()->hasSelection())
        return;

    if (!previous.isValid())
        return;

    int dimension = current.data(Qt::UserRole).toInt();
    QSharedPointer<BitmapDimension> dim = m_appContext->glyphManager()->bitmapDimensions()->bitmapDimension(dimension);

    if (dim.isNull())
        return;

    ui->spinBoxLeftCells->setValue(dim->margins().left());
    ui->spinBoxTopCells->setValue(dim->margins().top());
    ui->spinBoxRightCells->setValue(dim->margins().right());
    ui->spinBoxBottomCells->setValue(dim->margins().bottom());
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
