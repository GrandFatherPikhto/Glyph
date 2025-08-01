#include "mainstatusbar.h"
#include "appcontext.h"
#include "glyphcontext.h"
#include "glyphmanager.h"

MainStatusbar::MainStatusbar(AppContext *appContext, QWidget* parent)
    : QStatusBar(parent)
    , m_appContext(appContext)
    , m_glyphManager(nullptr)
    , m_fontLabel(nullptr)
    , m_glyphSizeLabel(nullptr)
    , m_gridSizeLabel(nullptr)
    , m_glyphRectLabel(nullptr)
    , m_charLabel(nullptr)
{
    initValues ();
    setupUI ();
    setupSignals ();
}

MainStatusbar::~MainStatusbar()
{

}

void MainStatusbar::setupSignals ()
{
    // QObject::connect(m_glyphManager, &GlyphManager::glyphChanged, this, MainStatusbar::setGlyph);
}

void MainStatusbar::initValues()
{
    Q_ASSERT(m_appContext != nullptr && m_appContext->glyphManager() != nullptr);
    m_glyphManager = m_appContext->glyphManager();
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

void MainStatusbar::setGlyph(QSharedPointer<GlyphContext> glyphContext)
{
    if (glyphContext.isNull())
        return;

    QString fontFamily = glyphContext->glyphFont() != QFont() ? glyphContext->glyphFont().family() : "Unknown Font";
    m_fontLabel->setText(QString("Font family: %1").arg(fontFamily));
    m_charLabel->setText(QString("Character: '%1'").arg(glyphContext->character()));
    m_glyphSizeLabel->setText(QString("Glyph Size: %1 px").arg(QString::number(glyphContext->glyphSize())));
    m_gridSizeLabel->setText(QString("Bitmap Size: %1x%1 px").arg(QString::number(glyphContext->dimension())));
}
