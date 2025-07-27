#include <QSettings>

#include "dockglyphtable.h"
#include "ui_dockglyphtable.h"

DockGlyphTable::DockGlyphTable(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyphTable)
    , m_appContext(appContext)
    , m_glyphPreview(nullptr)
    // , m_glyphTable(nullptr)
    // , m_mainSplitter(nullptr)
    , m_glyphModel(nullptr)
{
    ui->setupUi(this);

    // m_mainSplitter = new QSplitter(Qt::Vertical, this);
    m_glyphPreview = new GlyphPreview(m_appContext, this);
    // m_glyphTable = new QTableView(this);
    m_glyphModel = new GlyphModel(m_appContext, this);
    // m_glyphTable->setModel(m_glyphModel);
    // m_glyphTable->update();
    ui->tableViewGlyphs->setModel(m_glyphModel);

    // m_mainSplitter->addWidget(m_glyphPreview);
    // m_mainSplitter->addWidget(m_glyphTable);

    // ui->dockWidgetContents->layout()->addWidget(m_mainSplitter);

    QObject::connect(m_appContext->glyphManager(), &GlyphManager::glyphDataChanged, this, [=](){
        qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "Glyph Data Changed";
        emit m_glyphModel->layoutChanged();
    });

    connectSygnals();
}

DockGlyphTable::~DockGlyphTable()
{
    delete ui;
}

void DockGlyphTable::restoreData()
{
    ui->spinBoxBitmapDimension->setValue(m_appContext->bitmapDimension());
    if (m_appContext->character() != QChar())
    ui->glyphSize->setValue(m_appContext->glyphSize());
    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_appContext->character() << m_appContext->font() << m_appContext->glyphSize() << m_appContext->bitmapDimension();
}

void DockGlyphTable::connectSygnals()
{
    QObject::connect(ui->spinBoxBitmapDimension, &QSpinBox::valueChanged, m_appContext, &AppContext::setBitmapDimension);
    QObject::connect(ui->glyphSize, &QSpinBox::valueChanged, m_appContext, &AppContext::setGlyphSize);

    QObject::connect(ui->resetOffset, &QPushButton::clicked, m_appContext, &AppContext::resetGlyphOffset);
    QObject::connect(ui->moveLeft, &QPushButton::clicked, m_appContext, &AppContext::setGlyphMoveLeft);
    QObject::connect(ui->moveTop, &QPushButton::clicked, m_appContext, &AppContext::setGlyphMoveTop);
    QObject::connect(ui->moveRight, &QPushButton::clicked, m_appContext, &AppContext::setGlyphMoveRight);
    QObject::connect(ui->moveDown, &QPushButton::clicked, m_appContext, &AppContext::setGlyphMoveDown);

    QObject::connect(ui->tableViewGlyphs, &QTableView::clicked, this, [=](const QModelIndex &index){
        // QChar character = m_fontCharacterModel->characterAt(index);
        qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_appContext->glyphAt(index.row())->toString();
        // m_appContext->setCharacter(character, !m_fontCharacterModel->isSelected(index));
        QSharedPointer<GlyphMeta> glyphMeta = m_appContext->glyphAt(index.row());
        if (!glyphMeta.isNull())
        {
            emit m_appContext->glyphChanged(glyphMeta);
        }
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
