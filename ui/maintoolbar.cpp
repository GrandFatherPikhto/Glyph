#include "maintoolbar.h"

MainToolbar::MainToolbar(GlyphManager *glyphManager, QWidget *parent)
    : QToolBar(parent)
    , m_glyphManager(glyphManager)
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
    setupUI();
}

MainToolbar::~MainToolbar ()
{

}

void MainToolbar::setupUI()
{
    setObjectName("GlyphToolbar");
    m_templateLayerEnable = new QAction(QIcon(":/button/icons/template"), "Template Layer", this);
    m_templateLayerEnable->setCheckable(true);
    QObject::connect(m_templateLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit templateLayerEnable(checked);
    });
    addAction(m_templateLayerEnable);

    m_gridEnable = new QAction(QIcon(":/button/icons/grid"), "Grid Enable", this);
    m_gridEnable->setCheckable(true);
    QObject::connect(m_gridEnable, &QAction::toggled, this, [=](bool checked) {
        emit gridEnable(checked);
    });
    addAction(m_gridEnable);

    m_userLayerEnable = new QAction(QIcon(":/button/icons/user"), "User Layer Enable", this);
    m_userLayerEnable->setCheckable(true);
    QObject::connect(m_userLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit userLayerEnable(checked);
    });
    addAction(m_userLayerEnable);


    m_previewLayerEnable = new QAction(QIcon(":/button/icons/preview"), "Preview Layer Enable", this);
    m_previewLayerEnable->setCheckable(true);
    QObject::connect(m_previewLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit previewLayerEnable(checked);
    });
    addAction(m_previewLayerEnable);

    m_glyphRectLayerEnable = new QAction(QIcon(":/button/icons/glyphrect"), "Glyph Rect Layer Enable", this);
    m_glyphRectLayerEnable->setCheckable(true);
    QObject::connect(m_glyphRectLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit glyphRectLayerEnable(checked);
    });
    addAction(m_glyphRectLayerEnable);


    m_bitmapRectLayerEnable = new QAction(QIcon(":/button/icons/bitmaprect"), "BaseLine Layer Enable", this);
    m_bitmapRectLayerEnable->setCheckable(true);
    QObject::connect(m_bitmapRectLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit bitmapRectLayerEnable(checked);
    });
    addAction(m_bitmapRectLayerEnable);

    m_baselineLayerEnable = new QAction(QIcon(":/button/icons/baseline"), "Bitmap Rect Layer Enable", this);
    m_baselineLayerEnable->setCheckable(true);
    QObject::connect(m_baselineLayerEnable, &QAction::toggled, this, [=](bool checked) {
        emit baselineLayerEnable(checked);
    });
    addAction(m_baselineLayerEnable);

    m_pasteGlyphToUserLayer = new QAction(QIcon(":/button/icons/paste"), "Paste Glyph to User Layer", this);
    QObject::connect(m_pasteGlyphToUserLayer, &QAction::triggered, this, [=](){
        emit pasteGlyphToUserLayer();
    });
    addAction(m_pasteGlyphToUserLayer);

    m_clearUserLayer = new QAction(QIcon(":/button/icons/clear"), "Clear User Layer");
    QObject::connect(m_clearUserLayer, &QAction::triggered, this, [=](){
        emit clearUserLayer();
    });
    addAction(m_clearUserLayer);


    m_addBottomCells = new QSpinBox(this);
    m_addBottomCells->setMinimum(0);
    m_addBottomCells->setMaximum(1240);
    QObject::connect(m_addBottomCells, &QSpinBox::valueChanged, this, [=](int value){
        emit leftGridCells(value);
    });
    addWidget(m_addBottomCells);

    m_addLeftCells = new QSpinBox(this);
    m_addLeftCells->setMinimum(0);
    m_addLeftCells->setMaximum(1240);
    QObject::connect(m_addLeftCells, &QSpinBox::valueChanged, this, [=](int value){
        emit bottomGridCells(value);
    });
    addWidget(m_addLeftCells);
}

void MainToolbar::saveToolbarState()
{
    QSettings settings("DAE", "Glyph");

    settings.setValue("mainToolbarGeometry", saveGeometry());

    settings.setValue("MainToolbar/templateLayerEnable", m_templateLayerEnable->isChecked());
    settings.setValue("MainToolbar/gridEnable", m_gridEnable->isChecked());
    settings.setValue("MainToolbar/previewLayerEnable", m_previewLayerEnable->isChecked());
    settings.setValue("MainToolbar/userLayerEnable", m_userLayerEnable->isChecked());
    settings.setValue("MainToolbar/glyphRectLayerEnable", m_glyphRectLayerEnable->isChecked());
    settings.setValue("MainToolbar/baseLineLayerEnable", m_baselineLayerEnable->isChecked());
    settings.setValue("MainToolbar/bitmapRectLayerEnable", m_bitmapRectLayerEnable->isChecked());
    settings.setValue("MainToolbar/addLeftCells", m_addLeftCells->value());
    settings.setValue("MainToolbar/addBottomCells", m_addBottomCells->value());
}

void MainToolbar::restoreToolbarState()
{
    QSettings settings("DAE", "Glyph");

    bool setTemplateGlyphEnable = settings.value("templateLayerEnable").toBool();
    if (m_templateLayerEnable) {
        m_templateLayerEnable->setChecked(setTemplateGlyphEnable);
    }
    bool gridEnable = settings.value("MainToolbar/gridEnable").toBool();
    if (m_gridEnable) {
        m_gridEnable->setChecked(gridEnable);
    }
    bool userLayerEnable = settings.value("MainToolbar/userLayerEnable").toBool();
    if (m_userLayerEnable) {
        m_userLayerEnable->setChecked(userLayerEnable);
    }
    bool previewLayerEnable = settings.value("MainToolbar/previewLayerEnable").toBool();
    if (m_previewLayerEnable) {
        m_previewLayerEnable->setChecked(previewLayerEnable);
    }
    bool glyphRectLayerEnable = settings.value("MainToolbar/glyphRectLayerEnable").toBool();
    if (m_glyphRectLayerEnable) {
        m_glyphRectLayerEnable->setChecked(glyphRectLayerEnable);
    }
    bool baselineLayerEnable = settings.value("MainToolbar/baselineLayerEnable").toBool();
    if (m_baselineLayerEnable) {
        m_baselineLayerEnable->setChecked(baselineLayerEnable);
    }
    bool bitmapRectLayerEnable = settings.value("MainToolbar/bitmapRectLayerEnable").toBool();
    if (m_bitmapRectLayerEnable) {
        m_bitmapRectLayerEnable->setChecked(bitmapRectLayerEnable);
    }
    int addLeftCells = settings.value("MainToolbar/addLeftCells").toInt();
    m_addLeftCells->setValue(addLeftCells);
    int addBottomCells = settings.value("MainToolbar/addBottomCells").toInt();
    m_addBottomCells->setValue(addBottomCells);
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