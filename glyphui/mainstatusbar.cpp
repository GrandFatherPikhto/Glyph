#include "mainstatusbar.h"

MainStatusbar::MainStatusbar(AppContext *appContext, QWidget* parent)
    : QStatusBar(parent)
    , m_appContext(appContext)
    , m_fontLabel(nullptr)
    , m_glyphSizeLabel(nullptr)
    , m_gridSizeLabel(nullptr)
    , m_glyphRectLabel(nullptr)
    , m_charLabel(nullptr)
    , m_glyphMeta(QSharedPointer<GlyphMeta>())
{
    setupUI();
}

MainStatusbar::~MainStatusbar()
{

}

void MainStatusbar::setupUI()
{
    m_fontLabel = new QLabel(this);
    m_charLabel = new QLabel(this);
    m_glyphSizeLabel = new QLabel(this);
    m_gridSizeLabel = new QLabel(this);
    addPermanentWidget(m_fontLabel);
    addPermanentWidget(m_charLabel);
    addPermanentWidget(m_glyphSizeLabel);
    addPermanentWidget(m_gridSizeLabel);
}

void MainStatusbar::saveToolbarState()
{
    QSettings settings("DAE", "Glyph");

}

void MainStatusbar::restoreToolbarState()
{
    QSettings settings("DAE", "Glyph");

}

void MainStatusbar::setGlyph(QSharedPointer<GlyphMeta> glyphMeta)
{
    m_glyphMeta = glyphMeta;

    if (!m_glyphMeta.isNull())
    {
        m_fontLabel->setText(QString("Font family: %1").arg(m_glyphMeta->font().family()));
        m_charLabel->setText(QString("Character: '%1'").arg(m_glyphMeta->character()));
        m_glyphSizeLabel->setText(QString("Glyph Size: %1 px").arg(m_glyphMeta->glyphSize()));
        m_gridSizeLabel->setText(QString("Bitmap Size: %1x%1 px").arg(m_glyphMeta->bitmapDimension()));
    }
}
