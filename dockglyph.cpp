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
    , m_gridDimension(-1)
    , m_bitmapDimension(-1)
    , m_glyphSize(-1)
    , m_font(QFont())
    , m_character(QChar())
    , m_mainSplitter(nullptr)
    , m_glyphsModel(nullptr)
{
    ui->setupUi(this);
    m_mainSplitter = new QSplitter(Qt::Vertical, this);
    m_glyphPreview = new GlyphPreview(glyphManager, this);
    m_glyphTable = new QTableView(this);
    m_glyphsModel = new GlyphsModel(m_glyphManager, this);
    m_glyphTable->setModel(m_glyphsModel);
    m_glyphTable->update();

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

void DockGlyph::slotGlyphSizeChanged(int newGlyphSize)
{
    m_glyphSize = newGlyphSize;
    if (m_glyphMeta){
        m_glyphMeta->setGlyphSize(newGlyphSize);
    }
    updateGlyph ();
}


void DockGlyph::slotMoveCenterClicked()
{
    if (m_glyphMeta)
    {
        m_glyphMeta->moveCenter();
    }
    updateGlyph ();
}


void DockGlyph::slotMoveLeftClicked()
{
    if (m_glyphMeta)
    {
        m_glyphMeta->moveLeft();
    }
    updateGlyph ();
}


void DockGlyph::slotMoveTopClicked()
{
    if (m_glyphMeta)
    {
        m_glyphMeta->moveTop();
    }
    updateGlyph ();
}


void DockGlyph::slotMoveDownClicked()
{
    if (m_glyphMeta)
    {
        m_glyphMeta->moveDown();
    }
    updateGlyph ();
}


void DockGlyph::slotMoveRightClicked()
{
    m_glyphMeta->moveRight();
    updateGlyph();
}

void DockGlyph::updateGlyph ()
{
    if (m_character == QChar() || m_bitmapDimension < 6)
        return;
        
    m_glyphMeta = m_glyphManager->findOrCreate(m_character, m_bitmapDimension, m_glyphSize, m_font, m_fontPath);

    if (!m_glyphMeta)
        return;

    m_glyphMeta->setGlyphSize(m_glyphSize);

    if (m_glyphMeta->font() != m_font)
    {
        m_glyphMeta->setFontPath(m_fontPath);
        m_glyphMeta->setFont(m_font);
    }
    
    m_glyphMeta->setDirty();

    // qDebug() << __FILE__ << __LINE__ << m_character << m_bitmapDimension << m_glyphMeta->toString();
    qDebug() << "Row Count: " << m_glyphsModel->rowCount();
    // QModelIndex topLeft = m_glyphsModel->index(0, 0);
    // QModelIndex bottomRight = m_glyphsModel->index(m_glyphsModel->columnCount() - 1, m_glyphsModel->rowCount() -1, QModelIndex());
    // emit m_glyphsModel->dataChanged(topLeft, bottomRight, QList<int>(Qt::DisplayRole));
    emit m_glyphsModel->layoutChanged();
    emit glyphChanged(m_glyphMeta);
}


void DockGlyph::saveGeometryAndState() {
    QSettings settings("DAE", "Glyph");
    settings.setValue("dockGlyphGeometry", saveGeometry());
    settings.setValue("dockGlyphSplitter", m_mainSplitter->saveState());
    settings.setValue("dockGlyphBitmapSize", ui->bitmapDimension->value());
    settings.setValue("dockGlyphGlyphSize", ui->glyphSize->value());
    settings.setValue("dockGlyphFont", ui->fontComboBox->currentFont());
}

void DockGlyph::restoreGeometryAndState() {
    QSettings settings("DAE", "Glyph");
    restoreGeometry(settings.value("dockGlyphGeometry").toByteArray());
    m_mainSplitter->restoreState(settings.value("dockGlyphSplitter").toByteArray());
    ui->bitmapDimension->setValue(settings.value("dockGlyphBitmapSize").toInt());
    int glyphSize = settings.value("dockGlyphGlyphSize").toInt();
    qDebug() << glyphSize;
    if (glyphSize > 0)
    {
        ui->glyphSize->setValue(glyphSize);
    }
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
