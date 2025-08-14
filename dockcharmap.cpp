#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>

#include "dockcharmap.h"
#include "ui_dockcharmap.h"

#include "appcontext.h"
#include "appsettings.h"
#include "profilecontext.h"
#include "glyphcontext.h"
#include "glyphmanager.h"
#include "sqlfilter.h"
#include "charmapmanager.h"
#include "profilemanager.h"
#include "charmapmodel.h"
#include "unicodemetadataselectionmodel.h"

DockCharmap::DockCharmap(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockCharmap)
    , m_appContext(appContext)
    , m_glyphManager(nullptr)
    , m_profileManager(nullptr)
    , m_charmapModel(nullptr)
    , m_categoryModel(nullptr)
    , m_scriptModel(nullptr)
    , m_filter(nullptr)
    , m_categorySelectionModel(nullptr)
    , m_decompostionModel(nullptr)
    , m_unicodeMSB(-1)
{
    ui->setupUi(this);
    setupValues ();
    setupSignals();
    refreshCharmapTable();
}

DockCharmap::~DockCharmap()
{
    delete ui;
}

void DockCharmap::setupSignals()
{
    QObject::connect(ui->tableViewCharmap, &QTableView::clicked, this, &DockCharmap::glyphClicked);

    QObject::connect(ui->tableViewCharmap, &QTableView::doubleClicked, this, &DockCharmap::glyphDoubleClicked);

    QObject::connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this, [=](const QFont &font){
        QFont newFont(font);
        newFont.setPointSize(14);
        m_charmapManager->loadFont(newFont);
        refreshCharmapTable();
        m_profile.setFont(newFont);
        m_profile.setFontPath(m_charmapManager->fontPath());
        emit m_profileManager->changeProfile(m_profile);
    });

    QObject::connect(ui->listViewCategories->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=](const QItemSelection &selected, const QItemSelection &deselected){
        refreshCharmapTable();
    });

    QObject::connect(ui->listViewScripts->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=](const QItemSelection &selected, const QItemSelection &deselected){
        Q_UNUSED(selected);
        Q_UNUSED(deselected);
        refreshCharmapTable();
    });

    QObject::connect(ui->listViewDecompositions->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=](const QItemSelection &selected, const QItemSelection &deselected){
        Q_UNUSED(selected);
        Q_UNUSED(deselected);
        refreshCharmapTable();
    });

    QObject::connect(ui->spinBoxMSB, &QSpinBox::valueChanged, this, [=](int value){
        m_unicodeMSB = value;
        QSharedPointer<Condition> condition = m_filter->getCondition("msb");
        Q_ASSERT(!condition.isNull());

        if (m_unicodeMSB >= 0)
        {
            condition->setValue(m_unicodeMSB);
        } else
        {
            condition->resetValue();
        }
        refreshCharmapTable();
    });

    QObject::connect(ui->plainTextEditFilter, &QPlainTextEdit::textChanged, this, [=](){
        QString text = ui->plainTextEditFilter->toPlainText();
        QSharedPointer<Condition> condition = m_filter->getCondition("character");
        Q_ASSERT(!condition.isNull());
        if (!text.isEmpty())
        {
            QVariantList characters;
            std::transform(
                text.begin(), text.end(),
                std::back_inserter(characters),
                [](const QChar &ch) { return QVariant(static_cast<quint32>(ch.unicode())); } // Лямбда: QChar → QVariant
                );
            condition->appendValue(characters);
        } else
        {
            condition->resetValue();
        }

        refreshCharmapTable();
    });

    QObject::connect(m_profileManager, &ProfileManager::profileChanged, this, [=](const ProfileContext &profile){
        if (m_profile != profile && !profile.temporary() && profile.font() != QFont())
        {
            ui->fontComboBox->setCurrentFont(profile.font());
            refreshCharmapTable();
        }
    });

    QObject::connect(ui->pushButtonClearFilters, &QPushButton::clicked, this, [=](){
        ui->listViewDecompositions->selectionModel()->clearSelection();
        ui->listViewCategories->selectionModel()->clearSelection();
        ui->listViewScripts->selectionModel()->clearSelection();
        ui->spinBoxMSB->setValue(-1);
        ui->spinBoxUnicodeFrom->setValue(-1);
        ui->spinBoxUnicodeTo->setValue(-1);
        ui->plainTextEditFilter->setPlainText("");
        m_filter->reset();
        refreshCharmapTable();
    });

    QObject::connect(ui->spinBoxUnicodeFrom, &QSpinBox::valueChanged, this, [=](int value){
        QSharedPointer<Condition> condition = m_filter->getCondition("from");
        if (value > 0)
        {
            condition->setValue(QVariant(value));
            refreshCharmapTable();
        } else
        {
            condition->resetValue();
            refreshCharmapTable();
        }
    });

    QObject::connect(ui->spinBoxUnicodeTo, &QSpinBox::valueChanged, this, [=](int value){
        QSharedPointer<Condition> condition = m_filter->getCondition("to");
        if (value > 0)
        {
            condition->setValue(QVariant(value));
            refreshCharmapTable();
        } else
        {
            condition->resetValue();
            refreshCharmapTable();
        }
    });

    QObject::connect(ui->tableViewCharmap->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=](const QItemSelection &selected, const QItemSelection &deselected){
        if (selected.indexes().size())
        {
            const QModelIndexList &selectedIdxs = selected.indexes();
            const QModelIndex &selectedIdx = selected.indexes().at(0);
            if (selectedIdx.isValid())
            {
                GlyphContext context = m_charmapModel->glyphContext(selectedIdx.row());
                emit m_glyphManager->changeGlyph(context);
            }
        }
    });
}

void DockCharmap::setupValues ()
{
    Q_ASSERT(m_appContext->charmapManager() != nullptr && m_appContext->appSettings() != nullptr && m_appContext->glyphManager() != nullptr);

    m_charmapManager = m_appContext->charmapManager();
    m_appSettings = m_appContext->appSettings();
    m_glyphManager = m_appContext->glyphManager();
    m_profileManager = m_appContext->profileManager();
    m_filter = m_charmapManager->filter();

    m_profile = m_profileManager->profile();

    initFontComboBox();
    initCategoriesList();
    initScriptsList();
    initDecompositionsList();
    initCharmapTable ();
}

void DockCharmap::initFontComboBox ()
{
    if (m_profile.font() != QFont())
    {
        ui->fontComboBox->setCurrentFont(m_profile.font());
    }
}

void DockCharmap::initCharmapTable()
{
    m_charmapModel = new CharmapModel(m_appContext, this);
    refreshCharmapTable();

    ui->tableViewCharmap->setModel(m_charmapModel);
    ui->tableViewCharmap->resizeColumnsToContents();

    ui->tableViewCharmap->setStyleSheet(
        "QTableView {"
        "    selection-background-color: #a0a0ff;"
        "    selection-color: black;"
        "}"
        "QTableView::item:selected {"
        "    border: 1px solid #6060ff;"
        "}"
        );
    ui->tableViewCharmap->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
}

void DockCharmap::refreshCharmapTable ()
{
    // qDebug() << __FILE__ << __LINE__ << m_filter->prepare() << m_filter->binds();
    QSqlQuery query;
    m_charmapManager->execQuery(query);
    m_charmapModel->setQuery(std::move(query));
}

void DockCharmap::initCategoriesList ()
{
    m_categoryModel = new QSqlQueryModel(this);
    QSharedPointer<Condition> condition = m_filter->getCondition("category");
    Q_ASSERT(!condition.isNull());
    m_categorySelectionModel = new UnicodeMetadataSelectionModel(m_categoryModel, condition, this);

    refreshCategoriesList();
    ui->listViewCategories->setModel(m_categoryModel);
    ui->listViewCategories->setSelectionModel(m_categorySelectionModel);
    ui->listViewCategories->setModelColumn(1);
}

void DockCharmap::initScriptsList ()
{
    m_scriptModel = new QSqlQueryModel(this);
    QSharedPointer<Condition> condition = m_filter->getCondition("script");
    Q_ASSERT(!condition.isNull());
    m_scriptSelectionModel = new UnicodeMetadataSelectionModel(m_scriptModel, condition, this);
    refreshScriptsList();
    ui->listViewScripts->setModel(m_scriptModel);
    ui->listViewScripts->setSelectionModel(m_scriptSelectionModel);
    ui->listViewScripts->setModelColumn(1);
}

void DockCharmap::initDecompositionsList ()
{
    m_decompostionModel = new QSqlQueryModel(this);
    QSharedPointer<Condition> condition = m_filter->getCondition("decomposition");
    Q_ASSERT(!condition.isNull());
    m_decompositionSelectionModel = new UnicodeMetadataSelectionModel(m_decompostionModel, condition, this);

    refreshDecompositionsList();
    ui->listViewDecompositions->setModel(m_decompostionModel);
    ui->listViewDecompositions->setSelectionModel(m_decompositionSelectionModel);
    ui->listViewDecompositions->setModelColumn(1);
}

void DockCharmap::refreshCategoriesList ()
{
    QString queryStr = "SELECT id, name FROM category_data";
    m_categoryModel->setQuery(queryStr);;
    if (m_categoryModel->lastError().isValid()) {
        qWarning() << __FILE__ << __LINE__ << m_charmapModel->lastError().text();
    }
}

void DockCharmap::refreshScriptsList ()
{
    QString queryStr = "SELECT id, name FROM script_data";
    m_scriptModel->setQuery(queryStr);;
    if (m_scriptModel->lastError().isValid()) {
        qWarning() << __FILE__ << __LINE__ << m_charmapModel->lastError().text();
    }

}

void DockCharmap::refreshDecompositionsList ()
{
    QString queryStr = "SELECT id, name FROM decomposition_data";
    m_decompostionModel->setQuery(queryStr);;
    if (m_decompostionModel->lastError().isValid()) {
        qWarning() << __FILE__ << __LINE__ << m_charmapModel->lastError().text();
    }
}

void DockCharmap::glyphClicked(const QModelIndex &index)
{
    if (index.isValid() && index.column() == 0)
    {
        setResetGlyph(index.row());
    }
}

void DockCharmap::glyphDoubleClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        setResetGlyph(index.row());
    }
}

void DockCharmap::setResetGlyph(int row)
{
    GlyphContext context = m_charmapModel->glyphContext(row);
    if(context.isValid())
    {
        if (context.id() < 0)
        {
            if (m_glyphManager->appendGlyphIfNotExists(context))
            {
                refreshCharmapTable();
            }
        } else
        {
            if (m_glyphManager->removeGlyphById(context.id()))
            {
                refreshCharmapTable();
            }
        }
    }
}

void DockCharmap::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    saveDockCharmapState ();
}

void DockCharmap::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    saveDockCharmapState ();
}

void DockCharmap::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    restoreDockCharmapState ();
}

void DockCharmap::saveDockCharmapState()
{
    QSettings settings(this);

    settings.beginGroup("DockCharmap");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitterState", ui->splitter->saveState());
    settings.setValue("splitterGeometry", ui->splitter->saveGeometry());
    settings.setValue("filtersGeometry", ui->toolBoxFilters->saveGeometry());
    settings.setValue("filtersPage", ui->toolBoxFilters->currentIndex());
    settings.setValue("MSBFilter", ui->spinBoxMSB->value());

    settings.endGroup();
}

void DockCharmap::restoreDockCharmapState ()
{
    QSettings settings(this);

    settings.beginGroup("DockCharmap");

    restoreGeometry(settings.value("geometry").toByteArray());
    ui->splitter->restoreState(settings.value("splitterState").toByteArray());
    ui->splitter->restoreGeometry(settings.value("splitterGeometry").toByteArray());
    ui->toolBoxFilters->restoreGeometry(settings.value("filtersGeometry").toByteArray());
    ui->toolBoxFilters->setCurrentIndex(settings.value("filtersPage", 0).toInt());
    ui->spinBoxMSB->setValue(settings.value("MSBFilter", -1).toInt());

    settings.endGroup();
}

