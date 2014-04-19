#include "DialogSettings.hpp"
#include "ui_DialogSettings.h"

DialogSettings::DialogSettings(QWidget* parent) :
    QDialog(parent), ui(new Ui::DialogSettings)
{
    // GUI Configuration
    this->ui->setupUi(this);

    // Update push button state
    this->on_listWidgetRankingColumns_currentRowChanged(
                this->ui->listWidgetRankingColumns->currentRow());
}

DialogSettings::~DialogSettings(void)
{
    delete this->ui;
}

void DialogSettings::setNumberOfLaps(int numberOfLaps)
{
    this->ui->spinBoxNumberOfLaps->setValue(numberOfLaps);
}

void DialogSettings::setReloadPreviousLaps(bool reload)
{
    this->ui->checkBoxReloadPreviousLaps->setChecked(reload);
}

void DialogSettings::setBackUpAndRestoreApplicationState(bool backUpAndRestore)
{
    this->ui->checkBoxBackUpRestoreState->setChecked(backUpAndRestore);
}

int DialogSettings::numberOfLaps(void) const
{
    return this->ui->spinBoxNumberOfLaps->value();
}

bool DialogSettings::isReloadPreviousLapsChecked(void) const
{
    return this->ui->checkBoxReloadPreviousLaps->isChecked();
}

bool DialogSettings::isBackUpAndRestoreApplicationStateChecked(void) const
{
    return this->ui->checkBoxBackUpRestoreState->isChecked();
}

void DialogSettings::on_listWidgetRankingColumns_currentRowChanged(
        int currentRow)
{
    if (currentRow == 0)
    {
        this->ui->pushButtonUP->setEnabled(false);
        this->ui->pushButtonDown->setEnabled(true);
    }
    else if (currentRow == this->ui->listWidgetRankingColumns->count() -1)
    {
        this->ui->pushButtonUP->setEnabled(true);
        this->ui->pushButtonDown->setEnabled(false);
    }
    else
    {
        this->ui->pushButtonUP->setEnabled(true);
        this->ui->pushButtonDown->setEnabled(true);
    }
}

void DialogSettings::on_listWidgetRankingColumns_itemActivated(QListWidgetItem *item)
{
    item->setCheckState(item->checkState() == Qt::Checked ?
                            Qt::Unchecked : Qt::Checked);
}


void DialogSettings::on_pushButtonUP_clicked(void)
{
    // Get index of the selected item
    int selectedIndex = this->ui->listWidgetRankingColumns->currentRow();

    QListWidgetItem* selectedItem =
            this->ui->listWidgetRankingColumns->takeItem(selectedIndex);

    this->ui->listWidgetRankingColumns->insertItem(
                selectedIndex - 1, selectedItem);

    this->ui->listWidgetRankingColumns->setCurrentRow(selectedIndex - 1);
}

void DialogSettings::on_pushButtonDown_clicked(void)
{
    // Get index of the selected item
    int selectedIndex = this->ui->listWidgetRankingColumns->currentRow();

    QListWidgetItem* selectedItem =
            this->ui->listWidgetRankingColumns->takeItem(selectedIndex);

    this->ui->listWidgetRankingColumns->insertItem(
                selectedIndex + 1, selectedItem);

    this->ui->listWidgetRankingColumns->setCurrentRow(selectedIndex + 1);
}
