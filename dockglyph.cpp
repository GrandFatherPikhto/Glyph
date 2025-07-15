#include "dockglyph.h"
#include "ui_dockglyph.h"
#include "fontmanager.h"

DockGlyph::DockGlyph(GlyphManager *glyphManager, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyph)
    , m_glyphManager(glyphManager)
    , m_gridSize(8)
    , m_glyphSize(11)
    , m_font(QFont("Arial"))
    , m_character(QChar('A'))
{
    ui->setupUi(this);
    connectSygnals();
    ui->gridSize->setValue(m_gridSize);
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
    QObject::connect(ui->gridSize, &QSpinBox::valueChanged, this, &DockGlyph::slotGridSizeChanged);
    QObject::connect(ui->glyphSize, &QSpinBox::valueChanged, this, &DockGlyph::slotGlyphSizeChanged);
    QObject::connect(ui->moveCenter, &QPushButton::clicked, this, &DockGlyph::slotMoveCenterClicked);
    QObject::connect(ui->moveLeft, &QPushButton::clicked, this, &DockGlyph::slotMoveLeftClicked);
    QObject::connect(ui->moveTop, &QPushButton::clicked, this, &DockGlyph::slotMoveTopClicked);
    QObject::connect(ui->moveRight, &QPushButton::clicked, this, &DockGlyph::slotMoveRightClicked);
    QObject::connect(ui->moveDown, &QPushButton::clicked, this, &DockGlyph::slotMoveDownClicked);
}

void DockGlyph::slotFontChanged(const QFont &font)
{
    m_font = font;
    auto& fontManager = FontManager::instance();
    m_fontPath = fontManager.findFontPath(m_font.family());

    updateGlyph ();
}

void DockGlyph::slotCharacterChanged()
{
    m_character = ui->character->text().at(0);
    updateGlyph ();
}


void DockGlyph::showEvent(QShowEvent *event)
{
    updateGlyph ();
}


void DockGlyph::slotGridSizeChanged(int newSize)
{
    m_gridSize = newSize;
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
    m_glyphMeta = m_glyphManager->findOrCreate(m_character, m_gridSize);

    if (m_glyphMeta->glyphSize() <= 0)
    {
        m_glyphMeta->setGlyphSize(m_glyphSize);
    }

    if (m_glyphMeta->font() == QFont())
    {
        m_glyphMeta->setFontPath(m_fontPath);
        m_glyphMeta->setFont(m_font);
    }
    
    if (m_glyphMeta->glyphSize() < 0)
    {
        m_glyphMeta->setGlyphSize(m_glyphSize);
    }

    m_glyphMeta->setDirty();
    // qDebug() << m_glyph->toString();

    emit glyphChanged(m_glyphMeta);
}
