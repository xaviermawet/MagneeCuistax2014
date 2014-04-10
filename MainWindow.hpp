#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

// Database
#include "DataBase/DataBaseManager.hpp"
#include "DataBase/SqlTableModelIdNotEditable.hpp"
#include "DataBase/NSqlQueryModel.hpp"

// Dialogs
#include "Dialogs/DialogAddTeam.hpp"
#include "Dialogs/DialogCreateRace.hpp"

#define QSETTINGS_MAINWINDOW_KEYWORD "MainWindow"

namespace Ui {
    class MainWindow;
}

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
         * \brief createToolBar : create widget for toolbar
         */
        void createToolBar(void);

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

        // Team Management
        void on_actionCreateTeam_triggered(void);
        void on_actionDeleteTeam_triggered(void);
        void on_tableViewTeamList_activated(const QModelIndex &index);

        // Race Management
        void on_actionCreateRace_triggered(void);
        void on_actionDeleteRace_triggered(void);
        void updateRaceID(int currentRaceIndex);

    protected:

        // Widgets
        Ui::MainWindow* ui;
        QComboBox* _comboBoxRaceList;

        // Models
        SqlTableModelIdNotEditable* _teamListModel;
        NSqlQueryModel* _raceListModel;

        // Race
        int _currentRaceID;
};

#endif /* __MAINWINDOW_HPP__ */
