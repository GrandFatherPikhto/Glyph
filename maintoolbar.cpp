#include "maintoolbar.h"
#include "appcontext.h"
#include "appsettings.h"

MainToolbar::MainToolbar(AppContext *appContext, QWidget *parent)
    : QToolBar{parent}
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
{
    setObjectName("MainToolBar");
    setupUi();
    setupSignals();
    setupStates();
}

MainToolbar::~MainToolbar()
{

}

void MainToolbar::setupUi()
{
    m_templateLayerEnable = new QAction(QIcon(":/toolbar/template"), "Template Layer", this);
    m_templateLayerEnable->setCheckable(true);
    addAction(m_templateLayerEnable);

    m_gridEnable = new QAction(QIcon(":/toolbar/grid"), "Grid Enable", this);
    m_gridEnable->setCheckable(true);
    addAction(m_gridEnable);

    m_drawLayerEnable = new QAction(QIcon(":/toolbar/draw"), "Draw Layer Enable", this);
    m_drawLayerEnable->setCheckable(true);
    addAction(m_drawLayerEnable);


    m_previewLayerEnable = new QAction(QIcon(":/toolbar/preview"), "Preview Layer Enable", this);
    m_previewLayerEnable->setCheckable(true);
    addAction(m_previewLayerEnable);

    m_glyphRectLayerEnable = new QAction(QIcon(":/toolbar/glyph_rect"), "Glyph Rect Layer Enable", this);
    m_glyphRectLayerEnable->setCheckable(true);
    addAction(m_glyphRectLayerEnable);

    m_bitmapRectLayerEnable = new QAction(QIcon(":/toolbar/bitmap_rect"), "BaseLine Layer Enable", this);
    m_bitmapRectLayerEnable->setCheckable(true);
    addAction(m_bitmapRectLayerEnable);

    m_baselineLayerEnable = new QAction(QIcon(":/toolbar/baseline"), "Baseline Layer Enable", this);
    m_baselineLayerEnable->setCheckable(true);
    addAction(m_baselineLayerEnable);

    m_leftLineLayerEnable = new QAction(QIcon(":/toolbar/leftline"), "LeftLine Layer Enable", this);
    m_leftLineLayerEnable->setCheckable(true);
    addAction(m_leftLineLayerEnable);

    m_pasteGlyphToDrawLayer = new QAction(QIcon(":/toolbar/paste"), "Paste Glyph to User Layer", this);
    addAction(m_pasteGlyphToDrawLayer);

    m_clearDrawLayer = new QAction(QIcon(":/toolbar/clear"), "Clear User Layer");
    addAction(m_clearDrawLayer);
}

void MainToolbar::setupSignals()
{
    QObject::connect(m_templateLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appSettings->setValue("templateLayer", QVariant(checked));
        emit templateLayerEnable(checked);
    });

    QObject::connect(m_gridEnable, &QAction::toggled, this, [=](bool checked) {
        m_appSettings->setValue("gridLayer", QVariant(checked));
        emit gridEnable(checked);
    });

    QObject::connect(m_drawLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appSettings->setValue("drawLayer", QVariant(checked));
        emit userLayerEnable(checked);
    });

    QObject::connect(m_previewLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appSettings->setValue("previewLayer", QVariant(checked));
        emit previewLayerEnable(checked);
    });

    QObject::connect(m_glyphRectLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appSettings->setValue("glyphRectLayer", QVariant(checked));
        emit glyphRectLayerEnable(checked);
    });

    QObject::connect(m_bitmapRectLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appSettings->setValue("bitmapRectLayer", QVariant(checked));
        emit bitmapRectLayerEnable(checked);
    });

    QObject::connect(m_baselineLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appSettings->setValue("baselineLayer", QVariant(checked));
        emit baselineLayerEnable(checked);
    });

    QObject::connect(m_leftLineLayerEnable, &QAction::toggled, this, [=](bool checked) {
        m_appSettings->setValue("leftlineLayer", QVariant(checked));
        emit leftlineLayerEnable(checked);
    });

    QObject::connect(m_pasteGlyphToDrawLayer, &QAction::triggered, m_appContext, [=](){
        emit pasteGlyphToDrawLayer();
    });

    QObject::connect(m_clearDrawLayer, &QAction::triggered, this, [=](){
        emit clearDrawLayer();
    });
}

void MainToolbar::setupStates ()
{
    m_templateLayerEnable->setChecked(m_appSettings->value("templateLayer").toBool());
    m_gridEnable->setChecked(m_appSettings->value("gridLayer").toBool());
    m_glyphRectLayerEnable->setChecked(m_appSettings->value("glyphRectLayer").toBool());
    m_bitmapRectLayerEnable->setChecked(m_appSettings->value("bitmapRectLayer").toBool());
    m_templateLayerEnable->setChecked(m_appSettings->value("templateLayer").toBool());
    m_previewLayerEnable->setChecked(m_appSettings->value("previewLayer").toBool());
    m_drawLayerEnable->setChecked(m_appSettings->value("drawLayer").toBool());
    m_baselineLayerEnable->setChecked(m_appSettings->value("baselineLayer").toBool());
    m_leftLineLayerEnable->setChecked(m_appSettings->value("leftlineLayer").toBool());
}
