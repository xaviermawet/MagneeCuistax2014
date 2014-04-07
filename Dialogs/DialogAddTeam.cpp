#include "DialogAddTeam.hpp"
#include "ui_DialogAddTeam.h"

DialogAddTeam::DialogAddTeam(QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogAddTeam), _regex("^[a-zA-Z0-9 ]{3,80}$"),
    _validTeamName(false), _validCuistaxNumber(false)
{
    // GUI Configuration
    this->ui->setupUi(this);
}

DialogAddTeam::~DialogAddTeam(void)
{
    delete this->ui;
}

void DialogAddTeam::updateSaveButtonState(void)
{
    this->ui->buttonBox->setStandardButtons(
                (this->_validTeamName && _validCuistaxNumber) ?
                    QDialogButtonBox::Cancel | QDialogButtonBox::Save :
                    QDialogButtonBox::Cancel);
}

void DialogAddTeam::on_lineEditTeamName_textEdited(QString const& teamName)
{
    try
    {
        // Regex match
        if (!this->_regex.exactMatch(teamName))
            throw NException(tr("Min 3 characters. Only letters and numbers"));

        // Check if a team with the same name already exists
        QVariantList param; param << teamName;

        QSqlQuery query = DataBaseManager::execQuery(
                    "SELECT COUNT(*) FROM TEAM WHERE name LIKE ?", param);

        if (!query.next())
            throw NException(query.lastError().text());

        // Check if the name already exists
        if (query.value(0).toInt() != 0)
            throw NException(tr("Team name already exists"));

        this->_validTeamName = true;
        this->ui->labelTeamNameInformation->setText(
                    tr("<font color='green'>Team name available</font>"));
    }
    catch(NException const& exception)
    {
        this->_validTeamName = false;
        this->ui->labelTeamNameInformation->setText(
                    "<font color='red'>" + exception.what() + "</font>");
    }

    this->updateSaveButtonState();
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
        if (query.value(0).toInt() != 0)
            throw NException(tr("Cuistax number already exists"));

        this->_validCuistaxNumber = true;
        this->ui->labelCuistaxNumberInformation->setText(
                    tr("<font color='green'>Cuistax number available</font>"));
    }
    catch(NException const& exception)
    {
        this->_validCuistaxNumber = false;
        this->ui->labelCuistaxNumberInformation->setText(
                    "<font color='red'>" + exception.what() + "</font>");
    }

    this->updateSaveButtonState();
}
