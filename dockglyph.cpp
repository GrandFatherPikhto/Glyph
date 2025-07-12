#include "dockglyph.h"
#include "ui_dockglyph.h"
#include "fontmanager.h"

DockGlyph::DockGlyph(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyph)
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
    emit fontChanged(m_font, m_fontPath);
}

void DockGlyph::on_character_returnPressed()
{
    m_character = ui->character->text().at(0);
    emit characterChanged(m_character);
}


void DockGlyph::on_character_editingFinished()
{
    m_character = ui->character->text().at(0);
    emit characterChanged(m_character);
}



void DockGlyph::showEvent(QShowEvent *event)
{
    emit glyphParamsChanged(m_font, m_fontPath, m_glyphSize, m_character, m_gridSize);
}


void DockGlyph::on_gridSize_valueChanged(int newSize)
{
    m_gridSize = newSize;
    emit gridSizeChanged(m_gridSize);
}


void DockGlyph::on_glyphSize_valueChanged(int newGlyphSize)
{
    m_glyphSize = newGlyphSize;
    m_font.setPixelSize(m_glyphSize);
    emit glyphSizeChanged(m_glyphSize);
}


void DockGlyph::on_moveCenter_clicked()
{
    emit moveGlyphCenter();
}


void DockGlyph::on_moveLeft_clicked()
{
    emit moveGlyphLeft();
}


void DockGlyph::on_moveTop_clicked()
{
    emit moveGlyphTop();
}


void DockGlyph::on_moveDown_clicked()
{
    emit moveGlyphDown();
}


void DockGlyph::on_moveRight_clicked()
{
    emit moveGlyphRight ();
}

