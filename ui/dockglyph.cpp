#include <QSettings>

#include "dockglyph.h"
#include "ui_dockglyph.h"

DockGlyph::DockGlyph(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyph)
    , m_appContext(appContext)
    , m_glyphPreview(nullptr)
    , m_glyphTable(nullptr)
    , m_mainSplitter(nullptr)
    , m_glyphModel(nullptr)
{
    ui->setupUi(this);

    m_mainSplitter = new QSplitter(Qt::Vertical, this);
    m_glyphPreview = new GlyphPreview(m_appContext, this);
    m_glyphTable = new QTableView(this);
    m_glyphModel = new GlyphModel(m_appContext, this);
    m_glyphTable->setModel(m_glyphModel);
    m_glyphTable->update();

    m_mainSplitter->addWidget(m_glyphPreview);
    m_mainSplitter->addWidget(m_glyphTable);

    ui->dockWidgetContents->layout()->addWidget(m_mainSplitter);

    connectSygnals();
    ui->bitmapDimension->setValue(m_appContext->bitmapDimension());
    ui->fontComboBox->setCurrentFont(m_appContext->font());
    if (m_appContext->character() != QChar())
        ui->character->setText(QString("%1").arg(m_appContext->character()));
    ui->glyphSize->setValue(m_appContext->glyphSize());
}

DockGlyph::~DockGlyph()
{
    delete ui;
}

void DockGlyph::connectSygnals()
{
    QObject::connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this, &DockGlyph::slotFontChanged);
    QObject::connect(ui->character, &QLineEdit::returnPressed, this, &DockGlyph::slotCharacterChanged);
    QObject::connect(ui->character, &QLineEdit::editingFinished, this, &DockGlyph::slotCharacterChanged);
    QObject::connect(ui->bitmapDimension, &QSpinBox::valueChanged, this, &DockGlyph::slotBitmapDimensionChanged);
    QObject::connect(ui->glyphSize, &QSpinBox::valueChanged, this, &DockGlyph::slotGlyphSizeChanged);
    QObject::connect(ui->moveCenter, &QPushButton::clicked, this, &DockGlyph::slotMoveCenterClicked);
    QObject::connect(ui->moveLeft, &QPushButton::clicked, this, &DockGlyph::slotMoveLeftClicked);
    QObject::connect(ui->moveTop, &QPushButton::clicked, this, &DockGlyph::slotMoveTopClicked);
    QObject::connect(ui->moveRight, &QPushButton::clicked, this, &DockGlyph::slotMoveRightClicked);
    QObject::connect(ui->moveDown, &QPushButton::clicked, this, &DockGlyph::slotMoveDownClicked);
    QObject::connect(this, &DockGlyph::glyphChanged, m_glyphPreview, &GlyphPreview::slotSetGlyphMeta);
}

void DockGlyph::slotFontChanged(const QFont &font)
{
    emit m_appContext->fontChanged(font);
    ui->fontPath->setText(m_appContext->fontPath());
    updateGlyph ();
}

void DockGlyph::slotCharacterChanged()
{
    emit m_appContext->characterChanged(ui->character->text().at(0));
    updateGlyph ();
}

void DockGlyph::slotBitmapDimensionChanged(int newSize)
{
    emit m_appContext->bitmapDimensionChanged(newSize);
    updateGlyph ();
}

void DockGlyph::slotGlyphSizeChanged(int newGlyphSize)
{
    emit m_appContext->glyphSizeChanged(newGlyphSize);
    updateGlyph ();
}


void DockGlyph::slotMoveCenterClicked()
{
    if(m_glyphMeta.isNull())
        return;
    m_glyphMeta->moveCenter();
    updateGlyph ();
}


void DockGlyph::slotMoveLeftClicked()
{
    if(m_glyphMeta.isNull())
        return;
    m_glyphMeta->moveLeft();
    updateGlyph ();
}


void DockGlyph::slotMoveTopClicked()
{
    if(m_glyphMeta.isNull())
        return;
    m_glyphMeta->moveTop();
    updateGlyph ();
}


void DockGlyph::slotMoveDownClicked()
{
    if(m_glyphMeta.isNull())
        return;
    m_glyphMeta->moveDown();
    updateGlyph ();
}


void DockGlyph::slotMoveRightClicked()
{
    if(m_glyphMeta.isNull())
        return;
    m_glyphMeta->moveRight();
    updateGlyph();
}

void DockGlyph::updateGlyph ()
{
    qDebug() << QString("%1:%2 %3").arg(__FILE__, QString::number(__LINE__), __FUNCTION__);
    m_glyphMeta = m_appContext->findOrCreateCurrentGlyph();
    if (m_glyphMeta.isNull())
        return;
    qDebug() << QString("%1:%2 %3 %4").arg(__FILE__, QString::number(__LINE__), __FUNCTION__, m_glyphMeta->toString());
    emit m_glyphModel->layoutChanged();
    emit glyphChanged(m_glyphMeta);
}


void DockGlyph::saveGeometryAndState() {
    QSettings settings;
    settings.setValue("dockGlyphGeometry", saveGeometry());
    settings.setValue("dockGlyphSplitter", m_mainSplitter->saveState());
}

void DockGlyph::restoreGeometryAndState() {
    QSettings settings;
    restoreGeometry(settings.value("dockGlyphGeometry").toByteArray());
    m_mainSplitter->restoreState(settings.value("dockGlyphSplitter").toByteArray());
}

void DockGlyph::showEvent(QShowEvent *event)
{
    restoreGeometryAndState();
    updateGlyph ();
}

void DockGlyph::hideEvent(QHideEvent *event)
{
    saveGeometryAndState();
}


void DockGlyph::closeEvent(QCloseEvent *event)
{
    saveGeometryAndState();
}
