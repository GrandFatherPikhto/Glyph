#include <QSettings>

#include "dockglyph.h"
#include "ui_dockglyph.h"
#include "fontmanager.h"

DockGlyph::DockGlyph(GlyphManager *glyphManager, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyph)
    , m_glyphManager(glyphManager)
    , m_glyphPreview(nullptr)
    , m_glyphTable(nullptr)
    , m_gridDimension(8)
    , m_bitmapDimension(8)
    , m_glyphSize(11)
    , m_font(QFont("Arial"))
    , m_character(QChar('A'))
    , m_mainSplitter(nullptr)
{
    ui->setupUi(this);
    m_mainSplitter = new QSplitter(Qt::Vertical, this);
    m_glyphPreview = new GlyphPreview(glyphManager, this);
    m_glyphTable = new QTableView(this);
    m_mainSplitter->addWidget(m_glyphPreview);
    m_mainSplitter->addWidget(m_glyphTable);
    ui->dockWidgetContents->layout()->addWidget(m_mainSplitter);

    connectSygnals();
    ui->bitmapDimension->setValue(m_gridDimension);
    ui->fontComboBox->setCurrentFont(m_font);
    ui->character->setText(QString("%1").arg(m_character));
    ui->glyphSize->setValue(m_glyphSize);
    auto &fontManager = FontManager::instance();
    m_fontPath = fontManager.findFontPath(m_font.family());
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
    QObject::connect(ui->gridDimension, &QSpinBox::valueChanged, this, &DockGlyph::slotGridDimensionChanged);
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
    m_font = font;
    auto& fontManager = FontManager::instance();
    m_fontPath = fontManager.findFontPath(m_font.family());
    ui->fontPath->setText(m_fontPath);

    updateGlyph ();
}

void DockGlyph::slotCharacterChanged()
{
    m_character = ui->character->text().at(0);
    updateGlyph ();
}

void DockGlyph::slotBitmapDimensionChanged(int newSize)
{
    m_bitmapDimension = newSize;
    updateGlyph ();
}

void DockGlyph::slotGridDimensionChanged(int newSize)
{
    if (newSize < m_bitmapDimension)
    {
        ui->gridDimension->setValue(m_bitmapDimension);
        return;
    }
    m_gridDimension = newSize;
    emit gridDimensionChanged(m_gridDimension);
    updateGlyph ();
}

void DockGlyph::slotGlyphSizeChanged(int newGlyphSize)
{
    m_glyphSize = newGlyphSize;
    m_glyphMeta->setGlyphSize(newGlyphSize);
    updateGlyph ();
}


void DockGlyph::slotMoveCenterClicked()
{
    m_glyphMeta->moveCenter();
    updateGlyph ();
}


void DockGlyph::slotMoveLeftClicked()
{
    m_glyphMeta->moveLeft();
    updateGlyph ();
}


void DockGlyph::slotMoveTopClicked()
{
    m_glyphMeta->moveTop();
    updateGlyph ();
}


void DockGlyph::slotMoveDownClicked()
{
    m_glyphMeta->moveDown();
    updateGlyph ();
}


void DockGlyph::slotMoveRightClicked()
{
    m_glyphMeta->moveRight();
    updateGlyph();
}

void DockGlyph::updateGlyph ()
{
    m_glyphMeta = m_glyphManager->findOrCreate(m_character, m_bitmapDimension);
    // qDebug() << __FILE__ << __LINE__ << m_character << m_bitmapDimension << m_glyphMeta;

    if (m_glyphMeta->glyphSize() <= 0)
    {
        m_glyphMeta->setGlyphSize(m_glyphSize);
    }

    if (m_glyphMeta->font() == QFont())
    {
        m_glyphMeta->setFontPath(m_fontPath);
        m_glyphMeta->setFont(m_font);
    }

    m_glyphMeta->setGridDimension(m_gridDimension);
    
    m_glyphMeta->setDirty();

    emit glyphChanged(m_glyphMeta);
}


void DockGlyph::saveGeometryAndState() {
    QSettings settings("DAE", "Glyph");
    settings.setValue("dockGlyphGeometry", saveGeometry());
    settings.setValue("dockGlyphSplitter", m_mainSplitter->saveState());
    settings.setValue("dockGlyphBitmapSize", ui->bitmapDimension->value());
    settings.setValue("dockGlyphGridDimension", ui->gridDimension->value());
    settings.setValue("dockGlyphGlyphSize", ui->glyphSize->value());
    settings.setValue("dockGlyphFont", ui->fontComboBox->currentFont());
}

void DockGlyph::restoreGeometryAndState() {
    QSettings settings("DAE", "Glyph");
    restoreGeometry(settings.value("dockGlyphGeometry").toByteArray());
    m_mainSplitter->restoreState(settings.value("dockGlyphSplitter").toByteArray());
    ui->bitmapDimension->setValue(settings.value("dockGlyphBitmapSize").toInt());
    ui->gridDimension->setValue(settings.value("dockGlyphGridDimension").toInt());
    ui->glyphSize->setValue(settings.value("dockGlyphGlyphSize").toInt());
    // QFont font(settings.value("dockGlyphFont"));
    // qDebug() << __FILE__ << __LINE__ << settings.value("dockGlyphFont");
    QFont font(settings.value("dockGlyphFont").value<QFont>());
    ui->fontComboBox->setCurrentFont(font);
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
