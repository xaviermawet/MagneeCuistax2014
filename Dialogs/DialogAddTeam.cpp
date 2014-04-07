#include "DialogAddTeam.hpp"
#include "ui_DialogAddTeam.h"

DialogAddTeam::DialogAddTeam(QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogAddTeam), _regex("^[a-zA-Z0-9]{3,80}$")
{
    // GUI Configuration
    this->ui->setupUi(this);
}

DialogAddTeam::~DialogAddTeam(void)
{
    delete this->ui;
}

void DialogAddTeam::on_lineEditTeamName_textEdited(QString const& teamName)
{
    // Regex match
    if (!this->_regex.exactMatch(teamName))
    {
        this->ui->labelTeamNameInformation->setText(
                    tr("<font color='red'>Min 3 characters. "
                       "Only letters and numbers</font>"));
        return;
    }

    // Check if a team with the same name already exists
    QVariantList param; param << teamName;

    try
    {
        QSqlQuery query = DataBaseManager::execQuery(
                    "SELECT COUNT(*) FROM TEAM WHERE name LIKE ?", param);

        if (!query.next())
            throw NException(query.lastError().text());

        // Check if the name already exists
        if (query.value(0).toInt() == 0)
            this->ui->labelTeamNameInformation->setText(
                    tr("<font color='green'>Team name available</font>"));
        else
            this->ui->labelTeamNameInformation->setText(
                    tr("<font color='red'>Team name already exists</font>"));
    }
    catch(NException const& exception)
    {
        QMessageBox::warning(this, tr("Team name : Error"),
                             tr("An error occurred : ") + exception.what());
    }
}

void DialogAddTeam::on_spinBoxCuistaxNumber_valueChanged(int cuistaxNumber)
{
    // Check if a team with the same cuistax number already exists
    QVariantList param; param << cuistaxNumber;

    try
    {
        QSqlQuery query = DataBaseManager::execQuery(
                    "SELECT COUNT(*) FROM TEAM WHERE num_cuistax = ?", param);

        if (!query.next())
            throw NException(query.lastError().text());

        // Check if the name already exists
        if (query.value(0).toInt() == 0)
            this->ui->labelCuistaxNumberInformation->setText(
                   tr("<font color='green'>Cuistax number available</font>"));
        else
            this->ui->labelCuistaxNumberInformation->setText(
                  tr("<font color='red'>Cuistax number already exists</font>"));

    }
    catch(NException const& exception)
    {
        this->ui->labelCuistaxNumberInformation->setVisible(false);
        QMessageBox::warning(this, tr("Cuistax number : Error"),
                             tr("An error occurred : ") + exception.what());
    }
}
