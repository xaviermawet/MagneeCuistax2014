#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    _comboBoxRaceList(NULL), _teamListModel(NULL), _raceListModel(NULL),
    _currentRaceID(-1)
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

    // Restore previous MainWindows layout settings
    this->readSettings();

    // Connect to previous database if exists
    if (DataBaseManager::restorePreviousDataBase())
    {
        this->ui->statusBar->showMessage(
                tr("Latest project automatically loaded"), 4000);

        // Create team list model
        this->createTeamView();

        // Create combobox race list
        this->createToolBar();
    }
}

MainWindow::~MainWindow(void)
{
    delete this->ui;

    // Models
    delete this->_teamListModel;
    delete this->_raceListModel;

    delete this->_comboBoxRaceList;
}

void MainWindow::createTeamView(void)
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

void MainWindow::createToolBar(void)
{
    // Mainly developed with Qt Designer

    if(this->_comboBoxRaceList != NULL)
        delete this->_comboBoxRaceList;

    if (this->_raceListModel != NULL)
        delete this->_raceListModel;

    // Create model
    this->_raceListModel = new NSqlQueryModel(this);

    // Create comboBox based on raceListModel
    this->_comboBoxRaceList = new QComboBox();
    this->_comboBoxRaceList->setEditable(false);
    this->_comboBoxRaceList->setModel(this->_raceListModel);
    this->_comboBoxRaceList->setSizePolicy(QSizePolicy::Expanding,
                                           QSizePolicy::Maximum);

    // Add the comboBox to the toolBar of the MainWindow
    this->ui->mainToolBar->addWidget(this->_comboBoxRaceList);

    connect(this->_comboBoxRaceList, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateRaceID(int)));

    // Populates the model
    this->_raceListModel->setQuery("SELECT name, id FROM RACE");
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

    // Other MainWindow Settings ...

    settings.endGroup();
}

void MainWindow::writeLayoutSettings(const QString& settingsGroup) const
{
    QSettings settings;

    settings.beginGroup(settingsGroup);

    //settings.setValue("isMaximized",this->isMaximized());
    settings.setValue("geometry", this->saveGeometry());

    // Other MainWindow Settings ...

    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // Save the state of the MainWindow and its widgets
    this->writeSettings();

    QMainWindow::closeEvent(event);
}

void MainWindow::on_actionQuit_triggered(void)
{
    // Save the state of the MainWindow and its widgets
    this->writeSettings();

    qApp->quit();
}

void MainWindow::on_actionNewProject_triggered(void)
{
    // Get new project file path
    QString dbFilePath = QFileDialog::getSaveFileName(
                this, tr("Create new project file"), QDir::homePath(),
                tr("Projet Magnee Cuistax (*.db)"));

    // User canceled (nothing to save)
    if(dbFilePath.isEmpty())
    {
        this->ui->statusBar->showMessage(tr("Action canceled"), 4000);
        return;
    }

    try
    {
        // Create database
        if(DataBaseManager::createDataBase(dbFilePath))
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

    QItemSelectionModel* select = this->ui->tableViewTeamList->selectionModel();
    qDebug() << "Ajouter un tour au cuistax : " << select->selectedRows().first().data().toString();
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
