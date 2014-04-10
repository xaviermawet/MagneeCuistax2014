#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    _comboBoxRaceList(NULL), _stopWatch(NULL),
    _teamListModel(NULL), _raceListModel(NULL), _currentRaceID(-1)
{
    /* The value is used by the QSettings class when it is constructed using
     * the empty constructor. This saves having to repeat this information each
     * time a QSettings object is created.
     */
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

    // Frames
    delete this->_stopWatch;

    // Models
    delete this->_teamListModel;
    delete this->_raceListModel;
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
    this->_raceListModel->setQuery("SELECT name, id FROM RACE");
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

    // Update the current race id
    connect(this->_comboBoxRaceList, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateRaceID(int)));

    /* ---------------------------------------------------------------------- *
     *                                Stopwatch                               *
     * ---------------------------------------------------------------------- */
    if (this->_stopWatch != NULL)
        delete this->_stopWatch;

    // Create stopwatch
    this->_stopWatch = new NStopWatch(this);

    // Add the stopwatch to the mainToolBar
    this->ui->mainToolBar->addWidget(this->_stopWatch);

    // Informs th MainWindow that the race started
    connect(this->_stopWatch, SIGNAL(started()), this, SLOT(raceStarted()));

    // if the race changed, the stopwatch is stopped
    connect(this->_comboBoxRaceList, SIGNAL(currentIndexChanged(int)),
            this->_stopWatch, SLOT(reset()));
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
    // Restore MainWindow settings
    this->readLayoutSettings(QSETTINGS_MAINWINDOW_KEYWORD);

    // Other settings to restore...
}

void MainWindow::writeSettings(void) const
{
    // Save MainWindow settings
    this->writeLayoutSettings(QSETTINGS_MAINWINDOW_KEYWORD);

    // Other settings to save...
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
        if (this->updateDataBase(dbFilePath, DataBaseManager::openExistingDataBase))
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

    if (!this->_stopWatch->isActive())
        return;

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
    if (!this->_comboBoxRaceList ||
            this->_comboBoxRaceList->currentIndex() < 0)
        return;

    // Get race name
    QString raceName = this->_comboBoxRaceList->currentText();

    // Delete race from database
    QSqlQuery deleteQuery("DELETE FROM RACE WHERE name LIKE ?");
    deleteQuery.addBindValue(raceName);

    try
    {
        DataBaseManager::execTransaction(deleteQuery);
        this->statusBar()->showMessage(
                    tr("Race \"") + raceName + tr("\" deleted"), 4000);

        this->_raceListModel->refresh();
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to delete race ") + raceName,
                             exception.what());
    }
}

void MainWindow::updateRaceID(int currentRaceIndex)
{
    this->_currentRaceID =
            this->_raceListModel->index(currentRaceIndex, 1).data().toInt();

    qDebug() << "Mise à jour de l'id de la course = " << this->_currentRaceID;
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
