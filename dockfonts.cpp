#include <QSettings>
#include <QTimer>

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
    , m_fontModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    refreshFontsTable();
    ui->tableViewFonts->setModel(m_fontModel);
    ui->tableViewFonts->hideColumn(0);

    setupValues();
    setupSignals();
}

DockFonts::~DockFonts()
{
    delete ui;
}

void DockFonts::setupValues()
{
}

void DockFonts::setupSignals()
{
    connect(m_fontManager, &FontManager::loadingFinished, this, [=](int total){
        qDebug() << __FILE__ << __LINE__ << "Fonts loaded" << total;
        QTimer::singleShot(2000, this, &DockFonts::refreshFontsTable);
    });

    connect(ui->tableViewFonts->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=](const QItemSelection &selected, const QItemSelection &deselected){
        // qDebug() << __FILE__ << __LINE__ << selected << deselected;
    });

    connect(ui->lineEditName, &QLineEdit::textChanged, this, [=](const QString &value) {
        refreshFontsTable();
    });

    connect(ui->lineEditFamily, &QLineEdit::textChanged, this, [=](const QString &value) {
        refreshFontsTable();
    });

    connect(ui->lineEditStyle, &QLineEdit::textChanged, this, [=](const QString &value) {
        refreshFontsTable();
    });

    connect(ui->tableViewFonts->selectionModel(), &QItemSelectionModel::currentChanged, this, [=](const QModelIndex &current, const QModelIndex &previous){

        int fontId = getFontId(current.row());
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

int DockFonts::getFontId(int row)
{
    const QModelIndex &idx = m_fontModel->index(row, 0);
    if (idx.isValid())
    {
        return m_fontModel->data(idx, Qt::DisplayRole).toInt();
    }

    return -1;
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
    if (!db.isOpen()) return;

    // Строим запрос динамически
    QString sql = "SELECT id, name, family, style, system, path FROM fonts WHERE 1=1";

    if (!ui->lineEditName->text().isEmpty()) {
        sql += " AND name LIKE :name";
    }
    if (!ui->lineEditFamily->text().isEmpty()) {
        sql += " AND family LIKE :family";
    }
    if (!ui->lineEditStyle->text().isEmpty()) {
        sql += " AND style LIKE :style";
    }

    sql += " ORDER BY name ASC";

    QSqlQuery query(db);
    query.prepare(sql);

    if (!ui->lineEditName->text().isEmpty()) {
        query.bindValue(":name", "%" + ui->lineEditName->text() + "%");
    }
    if (!ui->lineEditFamily->text().isEmpty()) {
        query.bindValue(":family", "%" + ui->lineEditFamily->text() + "%");
    }
    if (!ui->lineEditStyle->text().isEmpty()) {
        query.bindValue(":style", "%" + ui->lineEditStyle->text() + "%");
    }

    if (!query.exec()) {
        qWarning() << "Query failed:" << query.lastError();
        return;
    }

    m_fontModel->setQuery(std::move(query));
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

