#include "dockglyphprofiles.h"
#include "ui_dockglyphprofiles.h"

#include "appcontext.h"
#include "appsettings.h"


DockGlyphProfiles::DockGlyphProfiles(AppContext *appContext, QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::DockGlyphProfiles)
    , m_appContext(appContext)
{
    ui->setupUi(this);
    initValues();
}

DockGlyphProfiles::~DockGlyphProfiles()
{
    delete ui;
}

void DockGlyphProfiles::initValues()
{
    Q_ASSERT(m_appContext != nullptr && m_appContext->appSettings() != nullptr);
    m_appSettings = m_appContext->appSettings();
}