#include "dockglyphselector.h"
#include "appsettings.h"
#include "appcontext.h"
#include "glyphmanager.h"
#include "fontmanager.h"
#include "fontcharactermodel.h"
#include "fontcategorymodel.h"
#include "fontdecompositionmodel.h"
#include "fontscriptmodel.h"
#include "ui_dockglyphselector.h"

DockGlyphSelector::DockGlyphSelector(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyphSelector)
    , m_appContext(appContext)
    , m_glyphManager(nullptr)
    , m_fontManager(nullptr)
    , m_appSettings(nullptr)
    , m_fontCharacterModel(nullptr)
    , m_scriptModel(nullptr)
    , m_categoryModel(nullptr)
    , m_decompositionModel(nullptr)
{
    ui->setupUi(this);
    
    setupModels  ();
    setupValues  ();
    setupSignals ();
}

DockGlyphSelector::~DockGlyphSelector()
{
    delete ui;
}

void DockGlyphSelector::setupModels ()
{
    m_fontCharacterModel = new FontCharacterModel(m_appContext, this);
    m_scriptModel = new FontScriptModel(m_appContext, this);
    m_categoryModel = new FontCategoryModel(m_appContext, this);
    m_decompositionModel = new FontDecompositionModel(m_appContext, this);

    ui->listCategories->setModel(m_categoryModel);
    ui->listScripts->setModel(m_scriptModel);
    ui->listDecompositions->setModel(m_decompositionModel);
    ui->tableCharacters->setModel(m_fontCharacterModel);
}

void DockGlyphSelector::setupValues ()
{
    Q_ASSERT(m_appContext != nullptr && m_appContext->fontManager() != nullptr && m_appContext->appSettings() != nullptr);

    m_fontManager = m_appContext->fontManager();
    m_appSettings = m_appContext->appSettings();
    m_glyphManager = m_appContext->glyphManager();

    ui->fontComboBox->setCurrentFont(m_fontManager->glyphFont());
    ui->spinBoxMSB->setValue(m_fontManager->fontMSBFilter());
    ui->spinBoxFrom->setValue(m_fontManager->fromFilter());
    ui->spinBoxTo->setValue(m_fontManager->toFilter());
    ui->textFilter->setText(m_fontManager->charactersFilter());
}

void DockGlyphSelector::setupSignals ()
{
    QObject::connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, m_appContext->fontManager(), &FontManager::setGlyphFont);
    QObject::connect(ui->tableCharacters, &QTableView::clicked, this, &DockGlyphSelector::selectCharacter);
    QObject::connect(ui->tableCharacters, &QTableView::doubleClicked, this, &DockGlyphSelector::setCharacter);
    QObject::connect(ui->listCategories->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DockGlyphSelector::setFontCategoryFilter);
    QObject::connect(ui->listScripts->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DockGlyphSelector::setFontScriptFilter);
    QObject::connect(ui->listDecompositions->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DockGlyphSelector::setFontDecompositionFilter);
    QObject::connect(ui->spinBoxMSB, &QSpinBox::valueChanged, m_appContext->fontManager(), &FontManager::setFontMSBFilter);
    QObject::connect(ui->textFilter, &QTextEdit::textChanged, this, [=](){
        QString filter = ui->textFilter->toPlainText();
        // qDebug() << filter << filter.contains(QChar('a'));
        m_appContext->fontManager()->setCharactersFilter(filter);
    });
    QObject::connect(ui->spinBoxFrom, &QSpinBox::valueChanged, this, [=](int value){
        Q_UNUSED(value)
        setFromToFilter ();
    });
    QObject::connect(ui->spinBoxTo, &QSpinBox::valueChanged, this, [=](int value){
        Q_UNUSED(value)
        setFromToFilter ();
    });
}

void DockGlyphSelector::setCharacter(const QModelIndex &index)
{
    QChar character = m_appContext->fontManager()->filteredCharacterAt(index.row());
    emit m_glyphManager->changeCharacter(character, false);
}

void DockGlyphSelector::selectCharacter(const QModelIndex &index)
{
    QChar character = m_appContext->fontManager()->filteredCharacterAt(index.row());
    emit m_glyphManager->changeCharacter(character, true);
}

void DockGlyphSelector::setFontCategoryFilter(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QModelIndexList selectedRows = ui->listCategories->selectionModel()->selectedRows();
    QVector<QChar::Category> items;
    for (const QModelIndex &item : std::as_const(selectedRows))
    {
        QChar::Category id = m_appContext->fontManager()->fontCategoryAt(item.row());
        items.append(id);
    }
    m_appContext->fontManager()->setCategoryFilter(items);
    emit m_fontCharacterModel->layoutChanged();
}

void DockGlyphSelector::setFontScriptFilter(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QModelIndexList selectedRows = ui->listScripts->selectionModel()->selectedRows();
    QVector<QChar::Script> items;
    for (const QModelIndex &item : std::as_const(selectedRows))
    {
        QChar::Script id = m_appContext->fontManager()->fontScriptAt(item.row());
        items.append(id);
    }
    m_appContext->fontManager()->setScriptFilter(items);
    emit m_fontCharacterModel->layoutChanged();
}

void DockGlyphSelector::setFontDecompositionFilter(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QModelIndexList selectedRows = ui->listDecompositions->selectionModel()->selectedRows();
    QVector<QChar::Decomposition> items;
    for (const QModelIndex &item : std::as_const(selectedRows))
    {
        QChar::Decomposition id = m_appContext->fontManager()->fontDecompositionAt(item.row());
        items.append(id);
    }
    m_appContext->fontManager()->setDecompositionFilter(items);
    emit m_fontCharacterModel->layoutChanged();
}

void DockGlyphSelector::setFromToFilter ()
{
    int from = ui->spinBoxFrom->value();
    int to = ui->spinBoxTo->value();
    int msb = ui->spinBoxMSB->value();

    if (msb >= 0)
    {
        m_appContext->fontManager()->setFromToFilter(msb, from, to);
    }
}

void DockGlyphSelector::saveDockGlyphSelectorState()
{
    QSettings settings;
    settings.setValue("dockGlyphSelector/geometry", saveGeometry());
    settings.setValue("splitter/state", ui->splitter->saveState());
}

void DockGlyphSelector::restoreDockGlyphSelectorState ()
{
    QSettings settings;
    restoreGeometry(settings.value("dockGlyphSelector/geometry").toByteArray());
    ui->splitter->restoreState(settings.value("splitter/state").toByteArray());
}

void DockGlyphSelector::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    restoreDockGlyphSelectorState();
}

void DockGlyphSelector::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    saveDockGlyphSelectorState ();
}

void DockGlyphSelector::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    saveDockGlyphSelectorState ();
}
