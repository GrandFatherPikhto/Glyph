#include <QGroupBox>

#include "dockglyphselector.h"

DockGlyphSelector::DockGlyphSelector(AppContext *appContext, QWidget *parent) :
    QDockWidget("Glyph Selector", parent)
    , m_appContext(appContext)
    , m_mainWidget(nullptr)
    , m_mainLayout(nullptr)
    , m_mainSplitter(nullptr)
    , m_fontCharacterModel(nullptr)
    , m_decompositionModel(nullptr)
    , m_categoryModel(nullptr)
    , m_scriptModel(nullptr)
    , m_glyphSplitter(nullptr)
    , m_fontCategoryList(nullptr)
    , m_fontScriptList(nullptr)
    , m_fontComboBox(nullptr)
    , m_fontMSB(nullptr)
    , m_fontCharsTable(nullptr)
{
    setObjectName("glyphSelectorDock");
    setupUI();
    setupSignals();
}

DockGlyphSelector::~DockGlyphSelector()
{
    
}

void DockGlyphSelector::setupSignals ()
{
    QObject::connect(m_fontCharsTable, &QTableView::clicked, this, &DockGlyphSelector::selectCharacter);
    QObject::connect(m_fontCharsTable, &QTableView::doubleClicked, this, &DockGlyphSelector::setCharacter);
    QObject::connect(m_fontComboBox, &QFontComboBox::currentFontChanged, m_appContext, &AppContext::setGlyphFont);
    QObject::connect(m_appContext->fontManager(), &FontManager::glyphFontChanged, this, &DockGlyphSelector::setGlyphFont);
    QObject::connect(m_fontCategoryList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DockGlyphSelector::setFontCategoryFilter);
    QObject::connect(m_fontScriptList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DockGlyphSelector::setFontScriptFilter);
    QObject::connect(m_fontDecompositionList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DockGlyphSelector::setFontDecompositionFilter);
}

void DockGlyphSelector::setupUI() {
    // qDebug() << __FILE__ << font().family() << font().pointSize();

    m_fontCharacterModel = new FontCharacterModel(m_appContext, this);
    m_decompositionModel = new FontDecompositionModel(this);
    m_categoryModel = new FontCategoryModel(this);
    m_scriptModel = new FontScriptModel(this);

    m_mainWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout(m_mainWidget);
    m_mainSplitter = new QSplitter(Qt::Orientation::Vertical, m_mainWidget);

    setupFontComboBox          ();
    setupFontCategoryList      ();
    setupFontScriptList        ();
    setupFontDecompositionList ();
    setupFontMSBSelector       ();
    setupGlyphTable            ();

    m_mainWidget->setLayout(m_mainLayout);
    m_mainLayout->addWidget(m_mainSplitter);
    setWidget(m_mainWidget);

    setGlyphFont(m_appContext->glyphFont());
}

void DockGlyphSelector::setupFontCategoryList () {
    QGroupBox *categoryGroup = new QGroupBox("Категории", m_mainWidget);
    QVBoxLayout *categoryBoxLayout = new QVBoxLayout(categoryGroup);
    m_fontCategoryList = new QListView(categoryGroup);
    m_fontCategoryList->setModel(m_categoryModel);
    categoryBoxLayout->addWidget(m_fontCategoryList);
    // categoryGroup->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_fontCategoryList->setSelectionMode(QAbstractItemView::MultiSelection);
    m_mainSplitter->addWidget(categoryGroup);
}

void DockGlyphSelector::setupFontDecompositionList()
{
    QGroupBox *group = new QGroupBox("Декомпозиция", m_mainWidget);
    QVBoxLayout *boxLayout = new QVBoxLayout(group);
    m_fontDecompositionList = new QListView(group);
    m_fontDecompositionList->setSelectionMode(QAbstractItemView::MultiSelection);
    m_fontDecompositionList->setModel(m_decompositionModel);
    boxLayout->addWidget(m_fontDecompositionList);
    m_mainSplitter->addWidget(group);
}

void DockGlyphSelector::setupFontScriptList () {
    QGroupBox *scriptGroup = new QGroupBox("Языки", m_mainWidget);
    QVBoxLayout *scriptBoxLayout = new QVBoxLayout(scriptGroup);
    m_fontScriptList = new QListView(scriptGroup);
    m_fontScriptList->setSelectionMode(QAbstractItemView::MultiSelection);
    m_fontScriptList->setModel(m_scriptModel);
    scriptBoxLayout->addWidget(m_fontScriptList);
    m_mainSplitter->addWidget(scriptGroup);
}

void DockGlyphSelector::setupFontMSBSelector () {
    QGroupBox *msbGroup = new QGroupBox("MSB", m_mainWidget);
    QVBoxLayout *msbBoxLayout = new QVBoxLayout(msbGroup);
    // m_fontMSB = new QSpinBox(msbGroup);
    // m_fontMSB->setRange(0, 255);
    m_fontMSB = new QLineEdit(msbGroup);
    QRegularExpressionValidator *v = new QRegularExpressionValidator(QRegularExpression("[\\da-fA-F]{1,2}"), m_fontMSB);
    m_fontMSB->setValidator(v);
    msbBoxLayout->addWidget(m_fontMSB);
    int fontMSB = m_appContext->fontManager()->fontMSBFilter();

    if (fontMSB < 0)
    {
        m_fontMSB->setText(QString(""));
    } else 
    {
        m_fontMSB->setText(QString::number(fontMSB, 16));
    }
    
    m_mainSplitter->addWidget(msbGroup);
}

void DockGlyphSelector::setupGlyphTable () {
    m_fontCharsTable = new QTableView(m_mainWidget);
    m_fontCharsTable->setModel(m_fontCharacterModel);
    m_mainSplitter->addWidget(m_fontCharsTable);
}

void DockGlyphSelector::setupFontComboBox ()
{
    m_fontComboBox = new QFontComboBox(this);
    m_fontComboBox->setCurrentFont(m_appContext->glyphFont());
    m_mainLayout->addWidget(m_fontComboBox);
}

void DockGlyphSelector::saveGlyphDockSelectorState()
{
    QSettings settings;
    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__;

    // Сохраняем состояние QSplitter
    if (m_mainSplitter) {
        settings.setValue("splitter/state", m_mainSplitter->saveState());
        // qDebug() << "Save splitter state";
    }

    // Сохраняем состояние всех QDockWidget
    foreach (QDockWidget* dock, findChildren<QDockWidget*>()) {
        settings.setValue(QString("dockGlyphSelectorWidgets/%1/geometry").arg(dock->objectName()),
                          dock->saveGeometry());
        settings.setValue(QString("dockGlyphSelectorWidgets/%1/floating").arg(dock->objectName()),
                          dock->isFloating());
        settings.setValue(QString("dockGlyphSelectorWidgets/%1/visible").arg(dock->objectName()),
                          dock->isVisible());
    }
}

void DockGlyphSelector::setFontMSB (const QString &text)
{
    bool ok;
    // QString text = m_fontMSB->text();
    int value = m_fontMSB->text().toInt(&ok, 16);  // toInt с основанием 16
    m_fontMSB->setText(text.toUpper());
    if (ok) {
        m_fontMSB->setStyleSheet("background-color: white;");
        if (text.isEmpty()) {
            // emit m_fontCharacterModel->setUnicodeMSB(-1);
            m_appContext->setFontMSB(-1);
        } else {
            // emit m_fontCharacterModel->setUnicodeMSB(value);
            m_appContext->setFontMSB(value);
        }
    } else if (m_fontMSB->text().isEmpty()) {
        m_fontMSB->setStyleSheet("background-color: #333333;");
        m_appContext->setFontMSB(-1);
    } else {
        m_fontMSB->setStyleSheet("background-color: #FFDDDD;");
    }
}

void DockGlyphSelector::setCharacter(const QModelIndex &index)
{
    QChar character = m_appContext->fontManager()->filteredCharacterAt(index.row());
    m_appContext->setCharacter(character, false);
}

void DockGlyphSelector::selectCharacter(const QModelIndex &index)
{
    QChar character = m_appContext->fontManager()->filteredCharacterAt(index.row());
    m_appContext->setCharacter(character, true);
}

void DockGlyphSelector::restoreGlyphDockSelectorState()
{
    QSettings settings;

    // Восстанавливаем QSplitter
    if (m_mainSplitter) {
        m_mainSplitter->restoreState(
            settings.value("splitter/state").toByteArray());
        // qDebug() << "Restore splitter state";
    }

    // Восстанавливаем QDockWidget
    foreach (QDockWidget* dock, findChildren<QDockWidget*>()) {
        dock->restoreGeometry(
            settings.value(QString("dockGlyphSelectorWidgets/%1/geometry").arg(dock->objectName()))
                .toByteArray());

        bool floating = settings.value(QString("dockGlyphSelectorWidgets/%1/floating").arg(dock->objectName()), false).toBool();
        dock->setFloating(floating);

        bool visible = settings.value(QString("dockGlyphSelectorWidgets/%1/visible").arg(dock->objectName()), true).toBool();
        dock->setVisible(visible);
    }
}

void DockGlyphSelector::closeEvent(QCloseEvent *event)
{
    // qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__;
    saveGlyphDockSelectorState();
    QDockWidget::closeEvent(event);
}

void DockGlyphSelector::hideEvent(QHideEvent *event)
{
    // qDebug() << __FILE__ << ":" << __LINE__ << __FUNCTION__;
    saveGlyphDockSelectorState();
    QDockWidget::hideEvent(event);
}

void DockGlyphSelector::saveDockState() {
    // qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    saveGlyphDockSelectorState();
}


void DockGlyphSelector::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    restoreGlyphDockSelectorState ();
}

void DockGlyphSelector::setGlyphFont (const QFont &font)
{
    // Q_UNUSED(font);
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << font;
    m_fontScriptList->setEnabled(false);
    m_scriptModel->setItems(m_appContext->fontManager()->fontScripts());
    m_fontScriptList->setEnabled(true);

    m_fontCategoryList->setEnabled(false);
    m_categoryModel->setItems(m_appContext->fontManager()->fontCategories());
    m_fontCategoryList->setEnabled(true);

    m_fontDecompositionList->setEnabled(false);
    m_decompositionModel->setItems(m_appContext->fontManager()->fontDecompositions());
    m_fontDecompositionList->setEnabled(true);

    m_fontCharsTable->resizeColumnsToContents();
    // Прокручиваем к началу
    m_fontCharsTable->scrollToTop();

    emit m_fontCharacterModel->layoutChanged();
}

void DockGlyphSelector::setFontCategoryFilter(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QModelIndexList selectedRows = m_fontCategoryList->selectionModel()->selectedRows();
    QVector<QChar::Category> items;
    for (const QModelIndex &item : selectedRows)
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

    QModelIndexList selectedRows = m_fontScriptList->selectionModel()->selectedRows();
    QVector<QChar::Script> items;
    for (const QModelIndex &item : selectedRows)
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

    QModelIndexList selectedRows = m_fontDecompositionList->selectionModel()->selectedRows();
    QVector<QChar::Decomposition> items;
    for (const QModelIndex &item : selectedRows)
    {
        QChar::Decomposition id = m_appContext->fontManager()->fontDecompositionAt(item.row());
        items.append(id);
    }
    m_appContext->fontManager()->setDecompositionFilter(items);
    emit m_fontCharacterModel->layoutChanged();
}

