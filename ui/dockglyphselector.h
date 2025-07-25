#ifndef GLYPHSELECTORDOCK_H
#define GLYPHSELECTORDOCK_H

#include <QObject>
#include <QWidget>

#include <QVBoxLayout>

#include <QDockWidget>
#include <QTableView>

#include <QLineEdit>
#include <QFontComboBox>
#include <QComboBox>
#include <QDockWidget>
#include <QSplitter>
#include <QListWidget>
#include <QSpinBox>
#include <QListView>

#include <QHideEvent>

#include <QVector>

#include <QSettings>
#include <QShowEvent>

#include "fontcharactermodel.h"
#include "fontmetadatamodel.h"
#include "fontscriptmodel.h"
#include "fontcategorymodel.h"
#include "fontdecompositionmodel.h"
#include "glyphmanager.h"
#include "appcontext.h"

class DockGlyphSelector : public QDockWidget {
    Q_OBJECT
public:
    explicit DockGlyphSelector(AppContext *glyphManager, QWidget *parent = nullptr);
    ~DockGlyphSelector();
    
    
signals:
    void glyphChanged (QSharedPointer<GlyphMeta> glyphMeta);

public slots:
    void saveDockState();

private slots:

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void saveGlyphDockSelectorState ();
    void restoreGlyphDockSelectorState ();

    void setupUI();
    void setupSignals ();
    void setupFontCategoryList ();
    void setupFontScriptList ();
    void setupFontDecompositionList ();
    void setupFontMSBSelector ();
    void setupGlyphTable ();
    void setupFontComboBox ();

    void setGlyphFont (const QFont &font);
    void setFontMSB (const QString &text);
    void selectCharacter(const QModelIndex &index);
    void setCharacter(const QModelIndex &index);

    void fillCategories (const QVector<quint32> &categories);
    void fillScripts(const QVector<quint32> &scripts);

    void setFontCategoryFilter(const QItemSelection &selected, const QItemSelection &deselected);
    void setFontScriptFilter(const QItemSelection &selected, const QItemSelection &deselected);
    void setFontDecompositionFilter(const QItemSelection &selected, const QItemSelection &deselected);
    
    AppContext *m_appContext;

    QWidget     *m_mainWidget;
    QVBoxLayout *m_mainLayout;
    QSplitter   *m_mainSplitter;

    QSplitter   *m_glyphSplitter;

    QListView   *m_fontCategoryList;
    QListView   *m_fontScriptList;
    QListView   *m_fontDecompositionList;

    // QSpinBox    *m_fontMSB;
    QLineEdit   *m_fontMSB;
    QTableView  *m_fontCharsTable;
    QFontComboBox *m_fontComboBox;


    FontCharacterModel *m_fontCharacterModel;

    FontScriptModel *m_scriptModel;
    FontCategoryModel *m_categoryModel;
    FontDecompositionModel *m_decompositionModel;
};

#endif // GLYPHSELECTORDOCK_H
