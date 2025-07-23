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
}

DockGlyph::~DockGlyph()
{
    delete ui;
}

void DockGlyph::restoreData()
{
    ui->bitmapDimension->setValue(m_appContext->bitmapDimension());
    ui->fontComboBox->setCurrentFont(m_appContext->font());
    if (m_appContext->character() != QChar())
        ui->character->setText(QString("%1").arg(m_appContext->character()));
    ui->glyphSize->setValue(m_appContext->glyphSize());
    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_appContext->character() << m_appContext->font() << m_appContext->glyphSize() << m_appContext->bitmapDimension();
}

void DockGlyph::connectSygnals()
{
    QObject::connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, m_appContext, &AppContext::setFont);
    QObject::connect(ui->bitmapDimension, &QSpinBox::valueChanged, m_appContext, &AppContext::setBitmapDimension);
    QObject::connect(ui->glyphSize, &QSpinBox::valueChanged, m_appContext, &AppContext::setGlyphSize);

    QObject::connect(ui->resetOffset, &QPushButton::clicked, m_appContext, &AppContext::resetGlyphOffset);
    QObject::connect(ui->moveLeft, &QPushButton::clicked, m_appContext, &AppContext::setGlyphMoveLeft);
    QObject::connect(ui->moveTop, &QPushButton::clicked, m_appContext, &AppContext::setGlyphMoveTop);
    QObject::connect(ui->moveRight, &QPushButton::clicked, m_appContext, &AppContext::setGlyphMoveRight);
    QObject::connect(ui->moveDown, &QPushButton::clicked, m_appContext, &AppContext::setGlyphMoveDown);

    QObject::connect(ui->character, &QLineEdit::returnPressed, this, &DockGlyph::slotCharacterChanged);
    QObject::connect(ui->character, &QLineEdit::editingFinished, this, &DockGlyph::slotCharacterChanged);
}

void DockGlyph::slotFontChanged(const QFont &font)
{
    m_appContext->setFont(font);
    ui->fontPath->setText(m_appContext->fontPath());
}

void DockGlyph::slotCharacterChanged()
{
    if (ui->character->text().size())
    {
        m_appContext->setCharacter(ui->character->text().at(0));
    }
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
    restoreData();
}

void DockGlyph::showEvent(QShowEvent *event)
{
    restoreGeometryAndState();
}

void DockGlyph::hideEvent(QHideEvent *event)
{
    saveGeometryAndState();
}


void DockGlyph::closeEvent(QCloseEvent *event)
{
    saveGeometryAndState();
}
