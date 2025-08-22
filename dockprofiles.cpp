#include <QPushButton>
#include <QSpinBox>

#include "dockprofiles.h"
#include "ui_dockprofiles.h"

#include "glyphcontext.h"
#include "appcontext.h"
#include "profilemanager.h"
#include "fontmanager.h"
#include "glyphmanager.h"
#include "appsettings.h"
#include "charmapmanager.h"
#include "profilemanager.h"
#include "fontmanager.h"
#include "gridmanager.h"

DockProfiles::DockProfiles(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockProfiles)
    , m_appContext(appContext)
    , m_appSettings(appContext->appSettings())
    , m_fontManager(appContext->fontManager())
    , m_profileManager(appContext->profileManager())
    , m_glyphManager(appContext->glyphManager())
    , m_gridManager(appContext->gridManager())
    , m_profilesModel(nullptr)
{
    ui->setupUi(this);
    setupValues();
    setupSignals ();
}

DockProfiles::~DockProfiles()
{
    delete ui;
}

void DockProfiles::setupSignals()
{
    QObject::connect(m_profileManager, &ProfileManager::profileChanged, this, [=](const ProfileContext &value) {
        loadProfileContext();
    });

    connect(m_gridManager, &GridManager::gridItemChanged, this, [=](const GridContext &context) {
        loadGridContext ();
    });

    QObject::connect(ui->lineEditProfileName, &QLineEdit::editingFinished, this, [=]() {
        ProfileContext profile = m_profileManager->profile();
        profile.setName(ui->lineEditProfileName->text());
        emit m_profileManager->changeProfile(profile);
    });

    QObject::connect(ui->spinBoxWidth, &QSpinBox::valueChanged, this, [=](int value) {
        GridContext grid = m_gridManager->grid();
        grid.setWidth(value);
        m_appSettings->setValue("defaultGridWidth", value);
        emit m_gridManager->changeGridItem(grid);
    });

    QObject::connect(ui->spinBoxHeight, &QSpinBox::valueChanged, this, [=](int value) {
        GridContext grid = m_gridManager->grid();
        grid.setHeight(value);
        m_appSettings->setValue("defaultGridHeight", value);
        emit m_gridManager->changeGridItem(grid);
    });

    QObject::connect(ui->spinBoxPaddingLeft, &QSpinBox::valueChanged, this, [=](int value) {
        GridContext grid = m_gridManager->grid();
        grid.setLeft(value);
        m_appSettings->setValue("defaultGridLeft", value);
        emit m_gridManager->changeGridItem(grid);
    });

    QObject::connect(ui->spinBoxPaddingTop, &QSpinBox::valueChanged, this, [=](int value) {
        GridContext grid = m_gridManager->grid();
        grid.setTop(value);
        m_appSettings->setValue("defaultGridTop", value);
        emit m_gridManager->changeGridItem(grid);
    });

    QObject::connect(ui->spinBoxPaddingRight, &QSpinBox::valueChanged, this, [=](int value) {
        GridContext grid = m_gridManager->grid();
        grid.setRight(value);
        m_appSettings->setValue("defaultGridRight", value);
        emit m_gridManager->changeGridItem(grid);
    });

    QObject::connect(ui->spinBoxPaddingBottom, &QSpinBox::valueChanged, this, [=](int value){
        GridContext grid = m_gridManager->grid();
        grid.setBottom(value);
        m_appSettings->setValue("defaultGridBottom", value);
        emit m_gridManager->changeGridItem(grid);
    });

    QObject::connect(ui->spinBoxGlyphSize, &QSpinBox::valueChanged, this, [=](int value){
        GlyphContext glyph = m_glyphManager->glyph();
        glyph.setSize(value);
        m_appSettings->setValue("defaultGlyphSize", value);
        emit m_glyphManager->changeGlyph(glyph);
    });

    QObject::connect(ui->pushButtonCreateProfile, &QPushButton::clicked, this, [=](){
        GridContext grid = readGridContext();
        m_gridManager->appendGridItem(grid);
        ProfileContext profile = readProfileContext();
        profile.setGridId(grid.id());
        // qDebug() << __FILE__ << __LINE__ << grid;
        if (profile.isValid())
        {
            if (m_profileManager->appendProfile(profile))
            {
                // qDebug() << __FILE__ << __LINE__ << profile;
                emit m_profileManager->changeProfile(profile);
                updateProfilesTable();
            }
        }
    });

/*
    QObject::connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, [=](int value){
        QModelIndex idIdx = m_profilesModel->index(value, 0);
        if (idIdx.isValid())
        {
            int id = m_profilesModel->data(idIdx, Qt::DisplayRole).toInt();
            ProfileContext profile;
            if (m_profileManager->getProfileById(id, profile))
            {
                // qDebug() << __FILE__ << __LINE__ << "Profile Id:" << id << ", Profile: " << profile;
                m_profile = profile;
                emit m_profileManager->changeProfile(profile);
            }
        }
    });
*/
    QObject::connect(m_profileManager, &ProfileManager::profilesChanged, this, [=](){
        updateProfilesTable();
    });
}

void DockProfiles::setupValues()
{
    Q_ASSERT(m_appContext->appSettings());

    loadProfileContext ();
    loadGridContext ();

    m_profilesModel = new QSqlQueryModel(this);

    updateProfilesTable();

    m_profilesModel->setHeaderData(0, Qt::Horizontal, "id");
    m_profilesModel->setHeaderData(1, Qt::Horizontal, "Name");
    m_profilesModel->setHeaderData(2, Qt::Horizontal, "Size");
    m_profilesModel->setHeaderData(3, Qt::Horizontal, "Font Id");
    m_profilesModel->setHeaderData(4, Qt::Horizontal, "Font Name");
    m_profilesModel->setHeaderData(5, Qt::Horizontal, "Font Family");
    m_profilesModel->setHeaderData(6, Qt::Horizontal, "Font Style");

    ui->tableViewProfiles->setModel(m_profilesModel);
    ui->tableViewProfiles->resizeColumnsToContents();
}

void DockProfiles::updateProfilesTable()
{
    QSqlQuery query(QSqlDatabase::database("main"));
    if (query.prepare(QString(
        "SELECT pr.id, pr.name, pr.glyph_size, fn.id, fn.name, fn.family, fn.style "
        "FROM %1 pr JOIN %2 fn ON pr.font_id = fn.id ORDER BY pr.name ASC").arg(
            m_profileManager->tableName(),
            m_fontManager->tableName())))
    {
        if(query.exec())
        {
            m_profilesModel->setQuery(std::move(query));
        }
    } else
    {
        qWarning() << __FILE__ << __LINE__ << "Error prepare query" << query.lastQuery() << ", Error: " << query.lastError();
    }
}

void DockProfiles::loadProfileContext()
{
    ProfileContext profile = m_profileManager->profile();
    FontContext font = m_fontManager->fontContext();

    // qDebug() << __FILE__ << __LINE__ << profile;

    m_fontManager->fontContextById(profile.fontId(), font);

    if (profile.glyphSize() != ui->spinBoxGlyphSize->value())
    {
        ui->spinBoxGlyphSize->setValue(profile.glyphSize());
    }

    if (ui->lineEditProfileName->text() != profile.name())
    {
        ui->lineEditProfileName->setText(profile.name());
    }
}

void DockProfiles::loadGridContext()
{
    GridContext grid = m_gridManager->grid();

    if (grid.width() != ui->spinBoxWidth->value())
    {
        ui->spinBoxWidth->setValue(grid.width());
    }

    if (grid.height() != ui->spinBoxHeight->value())
    {
        ui->spinBoxHeight->setValue(grid.height());
    }

    if (grid.left() != ui->spinBoxPaddingLeft->value())
    {
        ui->spinBoxPaddingLeft->setValue(grid.left());
    }

    if (grid.top() != ui->spinBoxPaddingTop->value())
    {
        ui->spinBoxPaddingTop->setValue(grid.top());
    }

    if (grid.right() != ui->spinBoxPaddingRight->value())
    {
        ui->spinBoxPaddingRight->setValue(grid.right());
    }

    if (grid.bottom() != ui->spinBoxPaddingBottom->value())
    {
        ui->spinBoxPaddingBottom->setValue(grid.bottom());
    }
}

GridContext DockProfiles::readGridContext()
{
    GridContext grid;

    grid.setWidth(ui->spinBoxWidth->value());
    grid.setHeight(ui->spinBoxHeight->value());
    grid.setLeft(ui->spinBoxPaddingLeft->value());
    grid.setTop(ui->spinBoxPaddingTop->value());
    grid.setRight(ui->spinBoxPaddingRight->value());
    grid.setBottom(ui->spinBoxPaddingBottom->value());

    return grid;
}

ProfileContext DockProfiles::readProfileContext()
{
    ProfileContext profile;

    profile.setGlyphSize(ui->spinBoxGlyphSize->value());
    profile.setFontId(m_fontManager->fontContext().id());
    profile.setGridId(m_gridManager->grid().id());
    profile.setName(ui->lineEditProfileName->text());

    return profile;
}
