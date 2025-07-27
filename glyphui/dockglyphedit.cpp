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
}

DockGlyphEdit::~DockGlyphEdit()
{
    delete ui;
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

    QObject::connect(m_appContext->glyphManager(), &GlyphManager::bitmapDimensionsChanged, this, [=](){

    });

    QObject::connect(ui->spinBoxBottomCells, &QSpinBox::valueChanged, this, [=](int value){
        // QMargins margins = m_appContext->
        if (ui->listViewBitmapDimensions->selectionModel()->hasSelection())
        {
            QModelIndex idx = ui->listViewBitmapDimensions->selectionModel()->currentIndex();
            int dimension = idx.data(Qt::UserRole).toInt();
            QSharedPointer<BitmapDimension> dim = m_appContext->glyphManager()->bitmapDimension(dimension);
            if (!dim.isNull())
            {
                QMargins margins = dim->gridMargins();
                margins.setBottom(value);
                dim->setGridMargins(margins);
            }
        }
    });
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
