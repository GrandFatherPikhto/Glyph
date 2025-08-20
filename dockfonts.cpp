#include <QSettings>

#include "dockfonts.h"
#include "ui_dockfonts.h"

#include "appcontext.h"
#include "fontmanager.h"
#include "fontmodel.h"

DockFonts::DockFonts(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockFonts)
    , m_appContext(appContext)
    , m_fontManager(appContext->fontManager())
    , m_fontModel(new FontModel(appContext, this))
{
    ui->setupUi(this);
    setupValues();
    setupSignals();
    refreshFontsTable();
}

DockFonts::~DockFonts()
{
    delete ui;
}

void DockFonts::setupValues()
{
    ui->tableViewFonts->setModel(m_fontModel);
    ui->tableViewFonts->hideColumn(0);
}

void DockFonts::setupSignals()
{
    connect(ui->tableViewFonts->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=](const QItemSelection &selected, const QItemSelection &deselected){
        // qDebug() << __FILE__ << __LINE__ << selected << deselected;
    });

    connect(ui->tableViewFonts->selectionModel(), &QItemSelectionModel::currentChanged, this, [=](const QModelIndex &current, const QModelIndex &previous){

        int fontId = m_fontModel->idByRow(current.row());
        if (fontId >= 0)
        {
            FontContext font;
            if (m_fontManager->fontContextById(fontId, font))
            {
                // qDebug() << __FILE__ << __LINE__ << fontId << font;
                if (font.isValid())
                {
                    emit m_fontManager->changeFontContext(font);
                }
            }
        }
    });
}

void DockFonts::saveDockFontsSettings()
{
    QSettings settings(this);
    settings.beginGroup("DockFonts");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitterState", ui->splitter->saveState());
    settings.setValue("splitterGeometry", ui->splitter->saveGeometry());
    settings.endGroup();
}

void DockFonts::restoreDockFontsSettings()
{
    QSettings settings(this);
    settings.beginGroup("DockFonts");
    restoreGeometry(settings.value("geometry").toByteArray());
    ui->splitter->restoreState(settings.value("splitterState").toByteArray());
    ui->splitter->restoreGeometry(settings.value("splitterGeometry").toByteArray());
    settings.endGroup();
}

void DockFonts::refreshFontsTable()
{
    QSqlDatabase db = QSqlDatabase::database("main");

    if (!db.isOpen())
    {
        qDebug() << __FILE__ << __LINE__ << "Database is not opened";
    }

    QSqlQuery query(db);

    if(!query.prepare("SELECT id, family, style, system, name, path FROM fonts ORDER BY family ASC"))
    {
        qWarning() << __FILE__ << __LINE__ << "Error prepare" << query.lastError();
    }

    if (!query.exec())
    {
        qWarning()  << __FILE__ << __LINE__ << "Error exec query" << query.lastError();
    }

    m_fontModel->setQuery(std::move(query));
    emit m_fontModel->layoutChanged();
}

void DockFonts::showEvent(QShowEvent *event)
{
    restoreDockFontsSettings();
}

void DockFonts::hideEvent(QHideEvent *event)
{
    saveDockFontsSettings();
}

void DockFonts::closeEvent(QCloseEvent *event)
{
    saveDockFontsSettings();
}

