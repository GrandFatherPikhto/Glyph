#include "maintoolbar.h"

MainToolbar::MainToolbar(AppContext *appContext, QWidget *parent)
    : QToolBar(parent)
    , m_appContext(appContext)
    , m_gridEnable(nullptr)
    , m_templateLayerEnable(nullptr)
    , m_userLayerEnable(nullptr)
    , m_previewLayerEnable(nullptr)
    , m_glyphRectLayerEnable(nullptr)
    , m_baselineLayerEnable(nullptr)
    , m_bitmapRectLayerEnable(nullptr)
    , m_addBottomCells(nullptr)
    , m_addLeftCells(nullptr)
    , m_pasteGlyphToUserLayer(nullptr)
    , m_clearUserLayer(nullptr)
{
    setObjectName("GlyphToolbar");

    setupUI();
    setupSignals();
    initState();
}

MainToolbar::~MainToolbar ()
{

}

void MainToolbar::setupUI()
{
    m_templateLayerEnable = new QAction(QIcon(":/button/icons/template"), "Template Layer", this);
    m_templateLayerEnable->setCheckable(true);
    addAction(m_templateLayerEnable);

    m_gridEnable = new QAction(QIcon(":/button/icons/grid"), "Grid Enable", this);
    m_gridEnable->setCheckable(true);
    addAction(m_gridEnable);

    m_userLayerEnable = new QAction(QIcon(":/button/icons/user"), "User Layer Enable", this);
    m_userLayerEnable->setCheckable(true);
    addAction(m_userLayerEnable);


    m_previewLayerEnable = new QAction(QIcon(":/button/icons/preview"), "Preview Layer Enable", this);
    m_previewLayerEnable->setCheckable(true);
    addAction(m_previewLayerEnable);

    m_glyphRectLayerEnable = new QAction(QIcon(":/button/icons/glyphrect"), "Glyph Rect Layer Enable", this);
    m_glyphRectLayerEnable->setCheckable(true);
    addAction(m_glyphRectLayerEnable);


    m_bitmapRectLayerEnable = new QAction(QIcon(":/button/icons/bitmaprect"), "BaseLine Layer Enable", this);
    m_bitmapRectLayerEnable->setCheckable(true);
    addAction(m_bitmapRectLayerEnable);

    m_baselineLayerEnable = new QAction(QIcon(":/button/icons/baseline"), "Bitmap Rect Layer Enable", this);
    m_baselineLayerEnable->setCheckable(true);
    addAction(m_baselineLayerEnable);

    m_pasteGlyphToUserLayer = new QAction(QIcon(":/button/icons/paste"), "Paste Glyph to User Layer", this);
    addAction(m_pasteGlyphToUserLayer);

    m_clearUserLayer = new QAction(QIcon(":/button/icons/clear"), "Clear User Layer");
    addAction(m_clearUserLayer);

    addSeparator();

    m_leftCellsLabel = new QLabel("Left Cells: ", this);
    addWidget(m_leftCellsLabel);

    m_addLeftCells = new QSpinBox(this);
    m_addLeftCells->setMinimum(0);
    m_addLeftCells->setMaximum(1240);
    addWidget(m_addLeftCells);

    m_bottomCellsLabel = new QLabel("Bottom Cells: ", this);
    addWidget(m_bottomCellsLabel);
    
    m_addBottomCells = new QSpinBox(this);
    m_addBottomCells->setMinimum(0);
    m_addBottomCells->setMaximum(1240);
    addWidget(m_addBottomCells);

    addSeparator();

}

void MainToolbar::initState ()
{
    Q_ASSERT(m_appContext != nullptr);

    m_gridEnable->setChecked(m_appContext->gridLayerEnable());
    m_templateLayerEnable->setChecked(m_appContext->templateLayerEnable());
    m_userLayerEnable->setChecked(m_appContext->drawLayerEnable());
    m_previewLayerEnable->setChecked(m_appContext->previewLayerEnable());
    m_glyphRectLayerEnable->setChecked(m_appContext->glyphRectLayerEnable());
    m_baselineLayerEnable->setChecked(m_appContext->baselineLayerEnable());
    m_bitmapRectLayerEnable->setChecked(m_appContext->bitmapRectLayerEnable());
    m_addBottomCells->setValue(m_appContext->bottomGridCells());
    m_addLeftCells->setValue(m_appContext->leftGridCells());
}

void MainToolbar::setupSignals()
{
    QObject::connect(m_templateLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appContext->setTemplateLayerEnable(checked);
        emit templateLayerEnable(checked);
    });

    QObject::connect(m_gridEnable, &QAction::toggled, this, [=](bool checked) {
        qDebug() << checked;
        m_appContext->setGridLayerEnable(checked);
        emit gridEnable(checked);
    });

    QObject::connect(m_userLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appContext->setDrawLayerEnable(checked);
        emit userLayerEnable(checked);
    });

    QObject::connect(m_previewLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appContext->setPreviewLayerEnable(checked);
        emit previewLayerEnable(checked);
    });

    QObject::connect(m_glyphRectLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appContext->setGlyphRectLayerEnable(checked);
        emit glyphRectLayerEnable(checked);
    });

    QObject::connect(m_bitmapRectLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appContext->setBitmapRectLayerEnable(checked);
        emit bitmapRectLayerEnable(checked);
    });

    QObject::connect(m_baselineLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appContext->setBaselineLayerEnable(checked);
        emit baselineLayerEnable(checked);
    });

    QObject::connect(m_pasteGlyphToUserLayer, &QAction::triggered, m_appContext, [=](){
        emit m_appContext->pasteTemplateToDrawLayer();
        emit pasteGlyphToUserLayer();
    });

    QObject::connect(m_clearUserLayer, &QAction::triggered, this, [=](){
        emit m_appContext->clearDrawLayer();
        emit clearUserLayer();
    });

    QObject::connect(m_addLeftCells, &QSpinBox::valueChanged, this, [=](int value){
        m_appContext->setLeftGridCells(value);
        emit leftGridCellsChanged(value);
    });

    QObject::connect(m_addBottomCells, &QSpinBox::valueChanged, this, [=](int value){
        m_appContext->setBottomGridCells(value);
        emit bottomGridCellsChanged(value);
    });
}

void MainToolbar::saveToolbarState()
{
    QSettings settings("DAE", "Glyph");

    settings.setValue("mainToolbarGeometry", saveGeometry());
}

void MainToolbar::restoreToolbarState()
{
    QSettings settings("DAE", "Glyph");
    restoreGeometry(settings.value("mainToolbarGeometry").toByteArray());
}

void MainToolbar::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    restoreToolbarState ();
}

void MainToolbar::hideEvent(QHideEvent *event)
{
    saveToolbarState ();
    Q_UNUSED(event)
}

void MainToolbar::closeEvent(QCloseEvent *event)
{
    saveToolbarState ();
    Q_UNUSED(event)
}
