#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "Common.hpp"

// Database
#include "DataBase/DataBaseManager.hpp"
#include "DataBase/SqlTableModelIdNotEditable.hpp"
#include "DataBase/NSqlQueryModel.hpp"
#include "DataBase/NColoredSqlQueryModel.hpp"
#include "DataBase/Lap.hpp"

// Dialogs
#include "Dialogs/DialogAddTeam.hpp"
#include "Dialogs/DialogCreateRace.hpp"
#include "Dialogs/DialogSettings.hpp"
#include "Dialogs/DialogPrint.hpp"

// Personal Widgets
#include "Widgets/NStopWatch.hpp"
#include "Widgets/DataViewer.hpp"

#define QSETTINGS_MAINWINDOW_KEYWORD            "MainWindow"
#define QSETTINGS_BACKUPANDRESTORE_KEYWORD      "BackUpAndRestore"
#define QSETTINGS_RACETABLEROWCOUNT_KEYWORD     "RaceTableRowCount"
#define QSETTINGS_RANKINGOPTIONALFIELDS_KEYWORD "RankingOptionalFields"

namespace Ui {
    class MainWindow;
}

typedef QMap<int, Lap> LapIdentifier;

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:

        /*!
         * \brief MainWindow : default constructor
         * \param parent : pointer to a parent MainWindow if exists
         */
        explicit MainWindow(QWidget* parent = 0);

        /*!
         * \brief ~MainWindow : virtual destructor
         */
        virtual ~MainWindow(void);

    protected:

        /*!
         * \brief createTeamListModel : create a team list model based on TEAM table
         */
        void createTeamListModel(void);

        /*!
         * \brief createRaceListModel : create a race list model based on RACE table
         */
        void createRaceListModel(void);

        /*!
         * \brief createRankingModels : create model for the ranking Table View
         */
        void createRankingModel(void);

        /*!
         * \brief createToolBar : create all the widgets for the mainToolBar
         */
        void createToolBar(void);

        /*!
         * \brief createRaceTableContextMenu : create a custom contextual menu
         * for the race table (called by right click)
         */
        void createRaceTableContextMenu(void);

        /*!
         * \brief centerOnScreen : center the MainWindow
         */
        void centerOnScreen(void);

        /*!
         * \brief readSettings : restore all previous MainWindow settings
         */
        void readSettings(void);

        /*!
         * \brief writeSettings : save all current MainWindow settings
         */
        void writeSettings(void) const;

        /*!
         * \brief readLayoutSettings : restore previous MainWindow layout settings
         * \param settingsGroup : name of the goup for layout settings
         */
        void readLayoutSettings(const QString& settingsGroup);

        /*!
         * \brief writeLayoutSettings : save current MainWindow layout settings
         * \param settingsGroup : name of the goup for layout settings
         */
        void writeLayoutSettings(const QString& settingsGroup) const;

        /*!
         * \brief closeEvent : overloaded method for writing settings
         */
        virtual void closeEvent(QCloseEvent* event);

        /*!
         * \brief updateDataBaseActionsVisibility : Hides or shows all actions
         * and menus that interact with the database
         * \param visible : the visibility
         */
        void updateDataBaseActionsVisibility(bool visible);

        /*!
         * \brief updateDataBase : update all the models and widgets based on
         *                         the database
         * \param dbFilePath : path to the database file (project file)
         * \return true if action succeed. Otherwise false.
         */
        bool updateDataBase(QString const& dbFilePath,
                           bool(*dataBaseAction)(QString const&));

    private slots:

        // File menu
        void on_actionNewProject_triggered(void);
        void on_actionOpenProject_triggered(void);
        void on_actionQuit_triggered(void);

        // Tools menu
        void on_actionOptions_triggered(void);

        // Data Viewer menu
        void on_menuDataViewer_aboutToShow(void);
        void on_actionOpenDataViewer_triggered(void);

        // Print menu
        void on_actionCurrentRanking_triggered(void);
        void on_actionPrintTeamlaps_triggered(void);

        // Team Management
        void on_actionCreateTeam_triggered(void);
        void on_actionDeleteTeam_triggered(void);
        void on_tableViewTeamList_activated(const QModelIndex &index);

        // Race Management
        void on_actionCreateRace_triggered(void);
        void on_actionDeleteRace_triggered(void);
        void on_tableWidgetLapList_customContextMenuRequested(const QPoint& pos);
        void on_actionDeleteSelectedLap_triggered(void);
        void currentRaceChanged(int currentRaceIndex);
        void updateLapListTableContent(void);
        void raceStarted(void);

        // Ranking Management
        void updateRankingModelQuery(void);

        // Data Viewer Management
        void destroyDataViewer(void);
        void updateDataViewerTitle(void);

    protected:

        // Widgets
        Ui::MainWindow* ui;
        QComboBox*  _comboBoxRaceList;
        DataViewer* _dataViewer;

        // Frames
        NStopWatch* _stopWatch;

        // Models
        SqlTableModelIdNotEditable* _teamListModel;
        NSqlQueryModel* _raceListModel;
        NColoredSqlQueryModel* _rankingModel;

        // Race
        int   _currentRaceID;
        float _currentRaceDistance;
        LapIdentifier _previousLapsInformation;
        QMenu* _raceTableContextMenu;

        // Ranking
        QList<RankingOptionalField> _optionalFields;
};

#endif /* __MAINWINDOW_HPP__ */
