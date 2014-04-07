#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
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
        this->ui->statusBar->showMessage(
                tr("Latest project automatically loaded"), 4000);
}

MainWindow::~MainWindow(void)
{
    delete this->ui;
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
                    tr("Team ") + dial.teamName() + tr(" created"), 4000);
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Enable to create team ")
                             + dial.teamName(), exception.what());
    }
}
