#include "DialogCreateRace.hpp"
#include "ui_DialogCreateRace.h"

DialogCreateRace::DialogCreateRace(QWidget* parent) :
    QDialog(parent), ui(new Ui::DialogCreateRace),
    _regex("^[a-zA-Z0-9àáâãäåçèéêëìíîïðòóôõöùúûüýÿ ]{3,80}$"),
    _validName(false), _validPlace(false), _validDistance(false)
{
    this->ui->setupUi(this);

    // Custom calendar
    this->ui->calendarWidgetDateRace->setMinimumDate(QDate::currentDate());
}

DialogCreateRace::~DialogCreateRace(void)
{
    delete this->ui;
}

void DialogCreateRace::updateSaveButtonState(void)
{
    this->ui->buttonBox->setStandardButtons(
               (_validName && _validPlace && _validDistance) ?
                    QDialogButtonBox::Cancel | QDialogButtonBox::Save :
                    QDialogButtonBox::Cancel);
}

void DialogCreateRace::on_lineEditName_textEdited(QString const& raceName)
{
    try
    {
        // Regex match
        if(!this->_regex.exactMatch(raceName))
            throw NException(tr("Min 3 characters. Only letters and numbers"));

        // Checks if a race with the same name already exists
        QVariantList param; param << raceName;
        QSqlQuery checkQuery = DataBaseManager::execQuery(
                    "SELECT COUNT(*) FROM RACE WHERE name LIKE ?", param);

        if (!checkQuery.next())
            throw NException(checkQuery.lastError().text());

        // Check if the name already exists
        if (checkQuery.value(0).toInt() != 0)
            throw NException(tr("A race with the same name already exists"));

        this->_validName = true;
        this->ui->labelNameInformation->setText(
                    tr("<font color='green'>Race name available</font>"));
    }
    catch(NException const& exception)
    {
        this->_validName = false;
        this->ui->labelNameInformation->setText(
                    "<font color='red'>" + exception.what() + "</font>");
    }

    this->updateSaveButtonState();
}

void DialogCreateRace::on_lineEditPlace_textEdited(QString const& racePlace)
{
    // Regex match
    this->ui->labelPlaceInformation->setText(
                ((this->_validPlace = this->_regex.exactMatch(racePlace))) ?
                    tr("<font color='green'>Valid place</font>") :
                    tr("<font color='red'>Min 3 characters. "
                       "Only letters and numbers</font>"));

    this->updateSaveButtonState();
}

void DialogCreateRace::on_doubleSpinBoxDistance_valueChanged(double distance)
{
    this->_validDistance = distance > 0.0;
    this->updateSaveButtonState();
}
