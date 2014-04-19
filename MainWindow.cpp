#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), _comboBoxRaceList(NULL),
    _dataViewer(NULL), _stopWatch(NULL), _teamListModel(NULL),
    _raceListModel(NULL), _rankingModel(NULL), _currentRaceID(-1),
    _currentRaceDistance(-1), _previousLapsInformation(),
    _raceTableContextMenu(NULL), _optionalFields()
{
    QCoreApplication::setOrganizationName("N4k1m");
    QCoreApplication::setApplicationName("MagneeCuistax2014");

    // Encoding configuration
    #if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    #endif

    // GUI Configuration
    this->ui->setupUi(this);
    this->createToolBar();
    this->createRaceTableContextMenu();
    this->createRankingModel();

    // Restore previous MainWindows layout settings
    this->readSettings();

    // Connect to previous database if exists
    if (DataBaseManager::restorePreviousDataBase())
    {
        this->createTeamListModel();
        this->createRaceListModel();

        // Display project file path in the main window title
        QFileInfo dbFile(QSqlDatabase::database().databaseName());
        this->setWindowTitle(tr("Cuistax lap counter - ") + dbFile.baseName());

        this->ui->statusBar->showMessage(
                tr("Latest project automatically loaded"), 4000);
    }
    else
        this->updateDataBaseActionsVisibility(false);
}

MainWindow::~MainWindow(void)
{
    // Widgets
    delete this->ui;
    delete this->_comboBoxRaceList;
    delete this->_dataViewer;

    // Frames
    delete this->_stopWatch;

    // Models
    delete this->_teamListModel;
    delete this->_raceListModel;
    delete this->_rankingModel;

    // Contextal menu
    delete this->_raceTableContextMenu;
}

void MainWindow::createTeamListModel(void)
{
    if (this->_teamListModel != NULL)
        delete this->_teamListModel;

    this->_teamListModel = new SqlTableModelIdNotEditable(this);
    this->_teamListModel->setTable("TEAM");
    this->_teamListModel->setHeaderData(0, Qt::Horizontal, tr("Cuistax number"));
    this->_teamListModel->setHeaderData(1, Qt::Horizontal, tr("Team name"));
    this->ui->tableViewTeamList->setModel(this->_teamListModel);
    this->_teamListModel->select();
}

void MainWindow::createRaceListModel(void)
{
    // Stop if the combobox (container) doesn't exist
    if (!this->_comboBoxRaceList)
        return;

    if (this->_raceListModel != NULL)
        delete this->_raceListModel;

    // Create model
    this->_raceListModel = new NSqlQueryModel(this);

    // Apply the new model to the combobox
    this->_comboBoxRaceList->setModel(this->_raceListModel);

    // Populates the model
    this->_raceListModel->setQuery("SELECT name, id, distance FROM RACE");

    // Select the first race
    this->_comboBoxRaceList->setCurrentIndex(0);
}

void MainWindow::createRankingModel(void)
{
    // delete if already exists
    if (this->_rankingModel != NULL)
        delete this->_rankingModel;

    // Create model
    this->_rankingModel = new NColoredSqlQueryModel(this);

    // Apply the model to the ranking table
    this->ui->tableViewRanking->setModel(this->_rankingModel);
}

void MainWindow::createToolBar(void)
{
    // Mainly developed with Qt Designer

    /* ---------------------------------------------------------------------- *
     *                           Race list combobox                           *
     * ---------------------------------------------------------------------- */
    if(this->_comboBoxRaceList != NULL)
        delete this->_comboBoxRaceList;

    // Create comboBox
    this->_comboBoxRaceList = new QComboBox();
    this->_comboBoxRaceList->setEditable(false);
    this->_comboBoxRaceList->setSizePolicy(QSizePolicy::Expanding,
                                           QSizePolicy::Maximum);

    // Add the comboBox to the mainToolBar
    this->ui->mainToolBar->addWidget(this->_comboBoxRaceList);

    // Update the current race id and update all the needed tables
    connect(this->_comboBoxRaceList, SIGNAL(currentIndexChanged(int)),
            this, SLOT(currentRaceChanged(int)));

    /* ---------------------------------------------------------------------- *
     *                                Stopwatch                               *
     * ---------------------------------------------------------------------- */
    if (this->_stopWatch != NULL)
        delete this->_stopWatch;

    // Create stopwatch
    this->_stopWatch = new NStopWatch(this);

    // Add the stopwatch to the mainToolBar (hidden)
    this->ui->mainToolBar->addWidget(this->_stopWatch);
    this->_stopWatch->setEnabled(false);

    // Informs th MainWindow that the race started
    connect(this->_stopWatch, SIGNAL(started()), this, SLOT(raceStarted()));

    // if the race changed, the stopwatch is stopped
    connect(this->_comboBoxRaceList, SIGNAL(currentIndexChanged(int)),
            this->_stopWatch, SLOT(reset()));
}

void MainWindow::createRaceTableContextMenu(void)
{
    if (this->_raceTableContextMenu != NULL)
        delete this->_raceTableContextMenu;

    // Create menu
    this->_raceTableContextMenu = new QMenu(this);

    // Add some "GUI created" actions
    this->_raceTableContextMenu->addAction(this->ui->actionDeleteSelectedLap);
}

void MainWindow::centerOnScreen(void)
{
    QDesktopWidget screen;

    QRect screenGeom = screen.screenGeometry(this);

    int screenCenterX = screenGeom.center().x();
    int screenCenterY = screenGeom.center().y();

    this->move(screenCenterX - width () / 2, screenCenterY - height() / 2);
}

void MainWindow::readSettings(void)
{
    // Check if settings must be restored
    QSettings settings;
    if (!settings.value(QSETTINGS_BACKUPANDRESTORE_KEYWORD, false).toBool())
        return;

    // Restore MainWindow settings
    this->readLayoutSettings(QSETTINGS_MAINWINDOW_KEYWORD);

    // Get the number of rows for the lap list
    this->ui->tableWidgetLapList->setMaxRow(
               settings.value(QSETTINGS_RACETABLEROWCOUNT_KEYWORD, 20).toInt());

    // Get ranking optional fields
    QVariantList reading = settings.value(
                QSETTINGS_RANKINGOPTIONALFIELDS_KEYWORD).toList();
    foreach (QVariant field, reading)
    {
        this->_optionalFields << (RankingOptionalField)field.toInt();
    }
}

void MainWindow::writeSettings(void) const
{
    // Check if settings must be saved
    QSettings settings;
    if (!settings.value(QSETTINGS_BACKUPANDRESTORE_KEYWORD, false).toBool())
        return;

    // Save MainWindow settings
    this->writeLayoutSettings(QSETTINGS_MAINWINDOW_KEYWORD);

    // Save the number of rows for the lap list
    settings.setValue(QSETTINGS_RACETABLEROWCOUNT_KEYWORD,
                      this->ui->tableWidgetLapList->maxRow());

    // Save ranking optional fields
    settings.setValue(QSETTINGS_RANKINGOPTIONALFIELDS_KEYWORD,
                      DataBaseManager::toVariantList(this->_optionalFields));
}

void MainWindow::readLayoutSettings(const QString& settingsGroup)
{
    QSettings settings;

    settings.beginGroup(settingsGroup);

    this->restoreGeometry(settings.value("geometry").toByteArray());
    this->ui->mainSplitter->restoreState(
                settings.value("mainSplitter").toByteArray());

    // Other MainWindow Settings ...

    settings.endGroup();
}

void MainWindow::writeLayoutSettings(const QString& settingsGroup) const
{
    QSettings settings;

    settings.beginGroup(settingsGroup);

    //settings.setValue("isMaximized",this->isMaximized());
    settings.setValue("geometry", this->saveGeometry());
    settings.setValue("mainSplitter", this->ui->mainSplitter->saveState());

    // Other MainWindow Settings ...

    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // Save the state of the MainWindow and its widgets
    this->writeSettings();

    QMainWindow::closeEvent(event);
}

void MainWindow::updateDataBaseActionsVisibility(bool visible)
{
    // Menus
    this->ui->menuTeams->menuAction()->setVisible(visible);
    this->ui->menuRace->menuAction()->setVisible(visible);

    // Actions
    this->ui->actionCreateRace->setVisible(visible);
    this->ui->actionDeleteRace->setVisible(visible);

    // enable/disable action shortcut
    this->ui->actionCreateTeam->blockSignals(!visible);
    this->ui->actionDeleteTeam->blockSignals(!visible);
    this->ui->actionCreateRace->blockSignals(!visible);
    this->ui->actionDeleteRace->blockSignals(!visible);

    // Buttons
    this->ui->pushButtonCreateTeam->setVisible(visible);
    this->ui->pushButtonDeleteTeam->setVisible(visible);
}

bool MainWindow::updateDataBase(const QString &dbFilePath,
                                bool (*dataBaseAction)(const QString &))
{
    /* ---------------------------------------------------------------------- *
     *                 Delete all sql models based on table(s)                *
     * ---------------------------------------------------------------------- */
    delete this->_teamListModel;
    this->_teamListModel = NULL;

    delete this->_raceListModel;
    this->_raceListModel = NULL;

    /* ---------------------------------------------------------------------- *
     *                           Action on data base                          *
     * ---------------------------------------------------------------------- */
    bool actionSucceed = (*dataBaseAction)(dbFilePath);

    // Hide or show 'Race' and 'Team' menu if action succeed or not
    this->updateDataBaseActionsVisibility(actionSucceed);

    // if action failed. Nothing else to do. return false
    if (!actionSucceed)
        return false;

    // Otherwise, display project file path in the main window title
    QFileInfo dbFile(QSqlDatabase::database().databaseName());
    this->setWindowTitle(tr("Cuistax lap counter - ") + dbFile.baseName());

    // Create models based on the database
    this->createTeamListModel();
    this->createRaceListModel();

    return true;
}

void MainWindow::on_actionNewProject_triggered(void)
{
    // Get new project file path
    QString dbFilePath = QFileDialog::getSaveFileName(
                this, tr("Create new project file"), QDir::homePath(),
                tr("Projet Magnee Cuistax (*.db)"));

    if(dbFilePath.isEmpty()) // User canceled (nothing to do)
        return;

    try
    {
        // Create database
        if (this->updateDataBase(dbFilePath, DataBaseManager::createDataBase))
            this->statusBar()->showMessage(
                    tr("Project successfully created"), 4000);
        else
            this->statusBar()->showMessage(
                    tr("Error : project not created"), 4000);
    }
    catch (NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to create project file"),
                             exception.what());
    }
}

void MainWindow::on_actionOpenProject_triggered()
{
    // Get existing project file path
    QString dbFilePath = QFileDialog::getOpenFileName(
                this, tr("Open project file"), QDir::homePath(),
                tr("Projet Magnee Cuistax (*.db)"));

    if (dbFilePath.isEmpty()) // User canceled (nothing to do)
        return;

    try
    {
        // Open the database
        if (this->updateDataBase(
                    dbFilePath, DataBaseManager::openExistingDataBase))
            this->statusBar()->showMessage(
                    tr("Project successfully opened"), 4000);
        else
            this->statusBar()->showMessage(
                    tr("Error : project not opened"), 4000);
    }
    catch (NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to open project file"),
                             exception.what());
    }
}

void MainWindow::on_actionQuit_triggered(void)
{
    // Save the state of the MainWindow and its widgets
    this->writeSettings();

    qApp->quit();
}

void MainWindow::on_actionOptions_triggered(void)
{
    DialogSettings dial;

    // Configure dialog
    dial.setNumberOfLaps(this->ui->tableWidgetLapList->maxRow());
    dial.setReloadPreviousLaps(false);
    QSettings settings;
    dial.setBackUpAndRestoreApplicationState(
            settings.value(QSETTINGS_BACKUPANDRESTORE_KEYWORD, false).toBool());
    dial.setOptionalFields(this->_optionalFields);

    // Open settings dialog
    if (dial.exec() != QDialog::Accepted) // User canceled
        return;

    // Save settings
    settings.setValue(QSETTINGS_BACKUPANDRESTORE_KEYWORD,
                      dial.isBackUpAndRestoreApplicationStateChecked());

    int previousNbOfRowsDisplayed = this->ui->tableWidgetLapList->rowCount();
    this->ui->tableWidgetLapList->setMaxRow(dial.numberOfLaps());

    if (dial.isReloadPreviousLapsChecked() &&
            previousNbOfRowsDisplayed < dial.numberOfLaps())
        this->updateLapListTableContent();

    this->_optionalFields = dial.optionalFieldsSelected();
    this->updateRankingModelQuery();
}

void MainWindow::on_menuDataViewer_aboutToShow(void)
{
    this->ui->actionOpenDataViewer->setText(
                this->_dataViewer == NULL ? tr("&Open") : tr("&Close"));
}

void MainWindow::on_actionOpenDataViewer_triggered(void)
{
    if (this->_dataViewer != NULL)
    {
        delete this->_dataViewer;
        this->_dataViewer = NULL;
        return;
    }

    // Create new window for data viewer
    this->_dataViewer = new DataViewer(this);

    // Add a table view model
    this->_dataViewer->setTableViewModel(this->_rankingModel);

    // Update title
    this->updateDataViewerTitle();


    // Connect signals/slots
    connect(this->_stopWatch, SIGNAL(started()),
            this->_dataViewer, SLOT(startStopWatch()));
    connect(this->_stopWatch, SIGNAL(stopped(QTime)),
            this->_dataViewer, SLOT(stopStopWatch()));
    connect(this->_stopWatch, SIGNAL(reseted()),
            this->_dataViewer, SLOT(resetStopWatch()));
    connect(this->_dataViewer, SIGNAL(closed()),
            this, SLOT(destroyDataViewer()));

    // Display the new dataViewer window
    this->_dataViewer->show();
}

void MainWindow::on_actionCreateTeam_triggered(void)
{
    DialogAddTeam dial;

    if (dial.exec() != QDialog::Accepted) // User canceled
        return;

    // Insert new team in database
    QSqlQuery insertQuery("INSERT INTO TEAM (num_cuistax, name) values (?, ?)");
    insertQuery.addBindValue(dial.cuistaxNumber());
    insertQuery.addBindValue(dial.teamName());

    try
    {
        DataBaseManager::execTransaction(insertQuery);
        this->statusBar()->showMessage(
                    tr("Team \"") + dial.teamName() + tr("\" created"), 4000);

        // If the race has already started
        if (this->_stopWatch->isActive() || this->_stopWatch->isInPause())
            this->_previousLapsInformation[dial.cuistaxNumber()] = Lap();

        this->_teamListModel->select();
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to create team ")
                             + dial.teamName(), exception.what());
    }
}

void MainWindow::on_actionDeleteTeam_triggered(void)
{
    QItemSelectionModel* select = this->ui->tableViewTeamList->selectionModel();

    // Nothing selected
    if(!select->hasSelection())
        return;

    QModelIndexList selectedIndexes = select->selectedIndexes();

    QSqlQuery deleteQuery("DELETE FROM TEAM WHERE num_cuistax = ?");
    deleteQuery.addBindValue(selectedIndexes.first().data());

    try
    {
        DataBaseManager::execTransaction(deleteQuery);
        this->statusBar()->showMessage(
                    tr("Team \"") + selectedIndexes.last().data().toString() +
                    tr("\" deleted"), 4000);

        this->_teamListModel->select();

        // Update the race table and ranking model
        this->updateLapListTableContent();
        this->_rankingModel->refresh();
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to delete team ")
                             + selectedIndexes.last().data().toString(),
                             exception.what());
    }
}

void MainWindow::on_tableViewTeamList_activated(const QModelIndex &index)
{
    Q_UNUSED(index)

    // If no race is selected
    if (this->_currentRaceID <= 0)
    {
        QMessageBox::information(this, tr("Enable to add a lap"),
                    tr("Before you can add a lap to a team, you have to "
                       "select a race or create a new one"));
        return;
    }

    // If the race is stopped
    if (!this->_stopWatch->isActive())
    {
        QMessageBox::information(this, tr("Enable to add a lap"),
                    tr("the race is stopped or has simply not yet started"));
        return;
    }

    // Get cuistax number
    QItemSelectionModel* select = this->ui->tableViewTeamList->selectionModel();
    int cuistaxNumber = select->selectedRows(0).first().data().toInt();

    if (!this->_previousLapsInformation.contains(cuistaxNumber))
        return;

    // Get previous lap time information
    Lap& lap = this->_previousLapsInformation[cuistaxNumber];
    QTime previousLapTime = lap.elapsedTime();

    // update lap with the current lap information
    lap.addLaps(1);
    lap.setTime(this->_stopWatch->elapsedTime());

    // Get time information for the current lap
    QTime currentLapTime = QTime(0, 0).addMSecs(
                previousLapTime.msecsTo(lap.elapsedTime()));

    // Insert lap information in database
    QSqlQuery insertQuery("INSERT INTO LAP (num, end_time, ref_race, ref_team) "
                          "VALUES (?, ?, ?, ?)");
    insertQuery.addBindValue(lap.lapNumber());
    insertQuery.addBindValue(currentLapTime);
    insertQuery.addBindValue(this->_currentRaceID);
    insertQuery.addBindValue(cuistaxNumber);

    try
    {
        DataBaseManager::execTransaction(insertQuery);

        // Populate the lap list table
        QVariantList params;
        params << cuistaxNumber
               << select->selectedRows(1).first().data()
               << lap.lapNumber()
               << currentLapTime.toString("mm:ss:zzz");
        this->ui->tableWidgetLapList->insertRowItems(0, params);

        // Update ranking model
        this->_rankingModel->refresh();
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to add lap"), exception.what());
    }
}

void MainWindow::on_actionCreateRace_triggered(void)
{
    DialogCreateRace dial;

    if (dial.exec() != QDialog::Accepted) // User canceld
        return;

    // Insert new race in database
    QSqlQuery insertQuery("INSERT INTO RACE (name, date, place, distance) "
                          "VALUES (?, ? , ?, ?)");
    insertQuery.addBindValue(dial.raceName());
    insertQuery.addBindValue(dial.raceDate());
    insertQuery.addBindValue(dial.racePlace());
    insertQuery.addBindValue(dial.raceDistance());

    try
    {
        DataBaseManager::execTransaction(insertQuery);
        this->statusBar()->showMessage(
                    tr("Race \"") + dial.raceName() + tr("\" created"), 4000);

        this->_raceListModel->refresh();

        // Show the newly created race in combobox
        this->_comboBoxRaceList->setCurrentIndex(
                    this->_comboBoxRaceList->count() - 1);
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to create race ")
                             + dial.raceName(), exception.what());
    }
}

void MainWindow::on_actionDeleteRace_triggered(void)
{
    // Stop if there is no race selected
    if (this->_currentRaceID <= 0)
        return;

    // Get race name
    QString raceName  = this->_comboBoxRaceList->currentText();
    int comboBoxIndex = this->_comboBoxRaceList->currentIndex();

    // Delete race from database
    QSqlQuery deleteQuery("DELETE FROM RACE WHERE name LIKE ?");
    deleteQuery.addBindValue(raceName);

    try
    {
        DataBaseManager::execTransaction(deleteQuery);
        this->statusBar()->showMessage(
                    tr("Race \"") + raceName + tr("\" deleted"), 4000);

        this->_raceListModel->refresh();

        // Show the previous race in combobox (if exists)
        if (comboBoxIndex > 0)
            --comboBoxIndex;
        this->_comboBoxRaceList->setCurrentIndex(comboBoxIndex);
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to delete race ") + raceName,
                             exception.what());
    }
}

void MainWindow::on_tableWidgetLapList_customContextMenuRequested(
        const QPoint &pos)
{
    if (!this->_raceTableContextMenu) // No custom menu created
        return;

    // Display custom contextual menu
    this->_raceTableContextMenu->exec(
                this->ui->tableWidgetLapList->viewport()->mapToGlobal(pos));
}

void MainWindow::on_actionDeleteSelectedLap_triggered(void)
{
    QItemSelectionModel* select = this->ui->tableWidgetLapList->selectionModel();

    // Get all lap informations
    int cuistaxNumber = select->selectedRows(0).first().data().toInt();
    int lapNumber = select->selectedRows(2).first().data().toInt();

    QSqlQuery deleteQuery("DELETE FROM LAP "
                          "WHERE num = ? "
                              "AND ref_race = ? "
                              "AND ref_team = ?");
    deleteQuery.addBindValue(lapNumber);
    deleteQuery.addBindValue(this->_currentRaceID);
    deleteQuery.addBindValue(cuistaxNumber);

    try
    {
        // Delete record from database
        DataBaseManager::execTransaction(deleteQuery);

        // Remove the row from the table
        this->ui->tableWidgetLapList->removeRow(
                    select->selectedIndexes().first().row());

        // Update the lap number
        QSqlQuery resetIdQuery("UPDATE LAP "
                               "SET num = num - 1 "
                               "WHERE num > ? "
                                  "AND ref_race = ? "
                                  "AND ref_team = ?");
        resetIdQuery.addBindValue(lapNumber);
        resetIdQuery.addBindValue(this->_currentRaceID);
        resetIdQuery.addBindValue(cuistaxNumber);
        DataBaseManager::execTransaction(resetIdQuery);

        // Update the race table content
        this->updateLapListTableContent();
    }
    catch(NException const& exception)
    {
        QMessageBox::critical(this, tr("Enable to delete the selected lap"),
                              exception.what());
    }
}

void MainWindow::currentRaceChanged(int currentRaceIndex)
{
    // No row selected in the combobox
    if (currentRaceIndex < 0)
    {
        this->_currentRaceID = -1;
        this->_currentRaceDistance = -1;
        this->_stopWatch->setEnabled(false);

        // clear lap list table
        this->ui->tableWidgetLapList->clearContents();
        this->ui->tableWidgetLapList->setRowCount(0);

        return;
    }

    // Get the current race id and distance
    this->_currentRaceID =
            this->_raceListModel->index(currentRaceIndex, 1).data().toInt();
    this->_currentRaceDistance =
            this->_raceListModel->index(currentRaceIndex, 2).data().toFloat();

    // Enable stop watch
    this->_stopWatch->setEnabled(true);

    // Update all the tables based on RACE or LAP sql tables
    this->updateLapListTableContent();
    this->updateRankingModelQuery();
}

void MainWindow::updateLapListTableContent(void)
{
    // clear lap list table
    this->ui->tableWidgetLapList->clearContents();
    this->ui->tableWidgetLapList->setRowCount(0);

    // Prepare binding value
    QVariantList param;
    param << this->_currentRaceID
          << this->ui->tableWidgetLapList->maxRow();

    try
    {
        QSqlQuery lapQuery = DataBaseManager::execQuery(
        "SELECT TEAM.num_cuistax, TEAM.name, LAP.num, LAP.end_time "
        "FROM LAP, TEAM "
        "WHERE LAP.ref_team = TEAM.num_cuistax "
        "AND LAP.ref_race = ? "
        "ORDER BY LAP.ROWID DESC LIMIT ?", param);

        // Populate the race table with previous laps information
        while (lapQuery.next())
        {
            QVariantList row;
            row << lapQuery.value(0)
                << lapQuery.value(1)
                << lapQuery.value(2)
                << lapQuery.value(3).toTime().toString("mm:ss:zzz");

            this->ui->tableWidgetLapList->insertRowItems(
                        this->ui->tableWidgetLapList->rowCount(), row);
        }
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to get old laps information"),
                             exception.what());
    }
}

void MainWindow::raceStarted(void)
{
    // Delete all information about "previous laps"
    this->_previousLapsInformation.clear();

    // create lap object for each team that will save the last lap information
    int teamCount = this->_teamListModel->rowCount();
    for(int i(0); i < teamCount; ++i)
        this->_previousLapsInformation[
            this->_teamListModel->index(i, 0).data().toInt()] = Lap();

    try
    {
        // Get the last lap number for each team for the current race  if exists
        QVariantList param; param << this->_currentRaceID;
        QString queryString("SELECT ref_team, MAX(num) "
                            "FROM LAP "
                            "WHERE ref_race = ? "
                            "GROUP BY ref_team");

        QSqlQuery query = DataBaseManager::execQuery(queryString, param);

        while(query.next())
            this->_previousLapsInformation[query.value(0).toInt()].setLapNumber(
                    query.value(1).toInt());
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to get previous lap information"),
                             exception.what());
    }
}

void MainWindow::updateRankingModelQuery(void)
{
    // Num cuistax and Team name are required
    QString queryString("SELECT TEAM.num_cuistax, TEAM.name ");

    // Add field(s) to SELECT clause
    foreach (RankingOptionalField field, this->_optionalFields)
    {
        switch (field)
        {
            case lapCount:
                queryString += ", MAX(LAP.num) ";
                break;
            case distance:
                queryString += ", MAX(LAP.num) * %2 ";
                break;
            case bestTime:
                queryString += ", MIN(LAP.end_time) ";
                break;
            case worstTime:
                queryString += ", MAX(LAP.end_time) ";
                break;
            case lastTime:
                if (this->_optionalFields.first() != lapCount)
                    queryString += ", LAP.end_time ";
                else
                    queryString += ", (SELECT end_time FROM LAP WHERE ref_team = TEAM.num_cuistax AND ref_race = %1 ORDER BY LAP.ROWID DESC LIMIT 1) ";
                break;
            default:
                break;
        }
    }

    // Body of query
    queryString += "FROM LAP "
                   "INNER JOIN TEAM ON TEAM.num_cuistax = LAP.ref_team "
                   "WHERE LAP.ref_race = %1 "
                   "GROUP BY TEAM.num_cuistax , TEAM.name ";

    // Add sorter field
    if (!this->_optionalFields.isEmpty())
    {
        switch (this->_optionalFields.first())
        {
            case lapCount:
            case distance:
                queryString += "ORDER BY MAX(LAP.num) DESC";
                break;
            case bestTime:
                queryString += "ORDER BY MIN(LAP.end_time)";
                break;
            case worstTime:
                queryString += "ORDER BY MAX(LAP.end_time) DESC";
                break;
            case lastTime:
                queryString += "ORDER BY LAP.end_time";
                break;
            default:
                break;
        }
    }

    // binding value
    queryString = queryString.arg(this->_currentRaceID);

    // binding second value if needed
    if (this->_optionalFields.contains(distance))
        queryString = queryString.arg(this->_currentRaceDistance);

    // Create query
    QSqlQuery query(queryString);

    // Populate model
    this->_rankingModel->setQuery(query);

    // Change hearde title
    this->_rankingModel->setHeaderData(0, Qt::Horizontal, tr("Cuistax number"));
    this->_rankingModel->setHeaderData(1, Qt::Horizontal, tr("Team name"));

    // Change headers title
    for (int i(0); i < this->_optionalFields.count(); ++i)
    {
        switch (this->_optionalFields.at(i))
        {
            case lapCount:
                this->_rankingModel->setHeaderData(
                            i + 2, Qt::Horizontal, tr("Lap count"));
                break;
            case distance:
                this->_rankingModel->setHeaderData(
                            i + 2, Qt::Horizontal, tr("Distance (m)"));
                break;
            case bestTime:
                this->_rankingModel->setHeaderData(
                            i + 2, Qt::Horizontal, tr("Best time"));
                break;
            case worstTime:
                this->_rankingModel->setHeaderData(
                            i + 2, Qt::Horizontal, tr("Worst time"));
                break;
            case lastTime:
                this->_rankingModel->setHeaderData(
                            i + 2, Qt::Horizontal, tr("Last time"));
                break;
            default:
                break;
        }
    }

    // Update DataViewer title
    this->updateDataViewerTitle();
}

void MainWindow::destroyDataViewer(void)
{
    delete this->_dataViewer;
    this->_dataViewer = NULL;
}

void MainWindow::updateDataViewerTitle(void)
{
    // Data Viwer closed
    if (this->_dataViewer == NULL)
        return;

    switch (this->_optionalFields.first())
    {
        case lapCount:
            this->_dataViewer->setRaceTitle(
                        this->_comboBoxRaceList->currentText() + tr(" : Lap Ranking"));
            break;
        case bestTime:
            this->_dataViewer->setRaceTitle(
                        this->_comboBoxRaceList->currentText() + tr(" : Time Ranking"));
            break;
        default:
            this->_dataViewer->setRaceTitle(
                        this->_comboBoxRaceList->currentText());
            break;
    }
}
