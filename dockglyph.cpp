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


void DockGlyph::on_fontComboBox_currentFontChanged(const QFont &f)
{
    m_font = f;
    auto& fontManager = FontManager::instance();
    m_fontPath = fontManager.findFontPath(m_font.family());
    updateGlyph ();
}

void DockGlyph::on_character_returnPressed()
{
    m_character = ui->character->text().at(0);
    updateGlyph ();
}


void DockGlyph::on_character_editingFinished()
{
    m_character = ui->character->text().at(0);
    qDebug() << m_font << m_character << m_gridSize;
    updateGlyph ();
}



void DockGlyph::showEvent(QShowEvent *event)
{
    updateGlyph ();
}


void DockGlyph::on_gridSize_valueChanged(int newSize)
{
    m_gridSize = newSize;
    updateGlyph ();
}


void DockGlyph::on_glyphSize_valueChanged(int newGlyphSize)
{
    m_glyphSize = newGlyphSize;
    m_glyph->setGlyphSize(newGlyphSize);
    updateGlyph ();
}


void DockGlyph::on_moveCenter_clicked()
{
    m_glyph->moveCenter();
    updateGlyph ();
}


void DockGlyph::on_moveLeft_clicked()
{
    m_glyph->moveLeft();
    updateGlyph ();
}


void DockGlyph::on_moveTop_clicked()
{
    m_glyph->moveTop();
    updateGlyph ();
}


void DockGlyph::on_moveDown_clicked()
{
    m_glyph->moveDown();
    updateGlyph ();
}


void DockGlyph::on_moveRight_clicked()
{
    m_glyph->moveRight();
    updateGlyph();
}

void DockGlyph::updateGlyph ()
{
    m_glyph = m_glyphManager->findOrCreate(m_character, m_gridSize);
    if (m_glyph->glyphSize() <= 0)
    {
        m_glyph->setGlyphSize(m_glyphSize);
    } else
    {

    }

    if (m_glyph->font() == QFont())
    {
        m_glyph->setFontPath(m_fontPath);
        m_glyph->setFont(m_font);
    }
    
    if (m_glyph->glyphSize() < 0)
    {
        m_glyph->setGlyphSize(m_glyphSize);
    }

    qDebug() << m_glyph->toString();

    emit glyphChanged(m_glyph);
}
