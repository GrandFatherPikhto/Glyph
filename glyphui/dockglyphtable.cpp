#include <QSettings>

#include "dockglyphtable.h"
#include "ui_dockglyphtable.h"

#include "glyphmanager.h"
#include "glyphcontext.h"
#include "glyphpreview.h"
#include "glyphmodel.h"
#include "appcontext.h"
#include "appsettings.h"
#include "glyphfilter.h"

DockGlyphTable::DockGlyphTable(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyphTable)
    , m_appContext(appContext)
    , m_glyphManager(nullptr)
    , m_glyphPreview(nullptr)
    , m_glyphModel(nullptr)
{
    ui->setupUi(this);
    setupValues ();

    m_glyphPreview = new GlyphPreview(m_appContext, this);
    m_glyphModel = new GlyphModel(m_appContext, this);
    ui->tableViewGlyphs->setModel(m_glyphModel);

    QObject::connect(m_glyphManager, &GlyphManager::glyphChanged, this, [=](QSharedPointer<GlyphContext> glyphContext){
        Q_UNUSED(glyphContext);
        qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "Glyph Data Changed";
    });

    connectSygnals();
}

DockGlyphTable::~DockGlyphTable()
{
    delete ui;
}

void DockGlyphTable::setupValues ()
{
    Q_ASSERT(m_appContext != nullptr && m_appContext->glyphManager() != nullptr && m_appContext->appSettings() != nullptr);
    m_glyphManager = m_appContext->glyphManager();
    m_appSettings = m_appContext->appSettings ();
    m_glyphFilter = m_appContext->glyphFilter ();
}

void DockGlyphTable::restoreData()
{
    ui->spinBoxBitmapDimension->setValue(m_appSettings->bitmapDimension());
    if (m_glyphManager->character() != QChar())
    ui->glyphSize->setValue(m_appSettings->glyphSize());
    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_appContext->character() << m_appContext->font() << m_appContext->glyphSize() << m_appContext->bitmapDimension();
}

void DockGlyphTable::connectSygnals()
{
    QObject::connect(ui->spinBoxBitmapDimension, &QSpinBox::valueChanged, m_appSettings, &AppSettings::setBitmapDimension);
    QObject::connect(ui->glyphSize, &QSpinBox::valueChanged, m_appSettings, &AppSettings::setGlyphSize);

    QObject::connect(ui->resetOffset, &QPushButton::clicked, m_glyphManager , &GlyphManager::glyphOffsetReset);
    QObject::connect(ui->moveLeft, &QPushButton::clicked, m_glyphManager, &GlyphManager::glyphOffsetLeft);
    QObject::connect(ui->moveTop, &QPushButton::clicked, m_glyphManager, &GlyphManager::glyphOffsetUp);
    QObject::connect(ui->moveRight, &QPushButton::clicked, m_glyphManager, &GlyphManager::glyphOffsetRight);
    QObject::connect(ui->moveDown, &QPushButton::clicked, m_glyphManager, &GlyphManager::glyphOffsetDown);

    QObject::connect(ui->tableViewGlyphs, &QTableView::clicked, this, [=](const QModelIndex &index){
        // QChar character = m_fontCharacterModel->characterAt(index);
        // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_glyphManager->filteredAt(index.row());
        // m_glyphManager->changeCurrentGlyphByFilteredPos(index.row());
    });
}

void DockGlyphTable::saveGeometryAndState() {
    QSettings settings;
    settings.setValue("dockGlyphGeometry", saveGeometry());
    // settings.setValue("dockGlyphSplitter", m_mainSplitter->saveState());
}

void DockGlyphTable::restoreGeometryAndState() {
    QSettings settings;
    restoreGeometry(settings.value("dockGlyphGeometry").toByteArray());
    // m_mainSplitter->restoreState(settings.value("dockGlyphSplitter").toByteArray());
    restoreData();
}

void DockGlyphTable::showEvent(QShowEvent *event)
{
    restoreGeometryAndState();
}

void DockGlyphTable::hideEvent(QHideEvent *event)
{
    saveGeometryAndState();
}


void DockGlyphTable::closeEvent(QCloseEvent *event)
{
    saveGeometryAndState();
}
