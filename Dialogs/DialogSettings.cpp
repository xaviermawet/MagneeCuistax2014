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

void DialogSettings::setOptionalFields(
        const QList<RankingOptionalField> &optionalFields)
{
    int itemIndex(-1);

    for (int i(0); i < optionalFields.count(); ++i)
    {
        // Get the item index
        itemIndex = this->itemIndexFor(optionalFields.at(i));
        if (itemIndex < 0) // Not found
            continue;

        // Get item
        QListWidgetItem* currentItem =
                this->ui->listWidgetRankingColumns->takeItem(itemIndex);

        // Change state of index
        currentItem->setCheckState(Qt::Checked);

        // Ordered insertion
        this->ui->listWidgetRankingColumns->insertItem(i, currentItem);
    }

    this->ui->listWidgetRankingColumns->setCurrentRow(0);
}

void DialogSettings::setDataViewerGroupBoxVisible(bool visible)
{
    this->ui->groupBoxDataViewer->setVisible(visible);
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

QList<RankingOptionalField> DialogSettings::optionalFieldsSelected(void) const
{
    QList<RankingOptionalField> selectedFields;
    QListWidgetItem* currentItem;

    for (int i(0); i < this->ui->listWidgetRankingColumns->count(); ++i)
    {
        currentItem = this->ui->listWidgetRankingColumns->item(i);

        // Continue if item isn't checked
        if (currentItem->checkState() != Qt::Checked)
            continue;

        QString itemType = currentItem->whatsThis();


        if (itemType == "LAP_COUNT")
            selectedFields << lapCount;
        else if (itemType == "DISTANCE")
            selectedFields << distance;
        else if (itemType == "BEST_TIME")
            selectedFields << bestTime;
        else if (itemType == "WORST_TIME")
            selectedFields << worstTime;
        else if (itemType == "LAST_TIME")
            selectedFields << lastTime;
    }

    return selectedFields;
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

void DialogSettings::on_spinBoxFontSize_valueChanged(int fontSize)
{
    emit this->fontSizeChange(fontSize);
}

int DialogSettings::itemIndexFor(const RankingOptionalField &field)
{
    for (int i(0); i < this->ui->listWidgetRankingColumns->count(); ++i)
    {
        QListWidgetItem* currentItem =
                this->ui->listWidgetRankingColumns->item(i);

        switch (field)
        {
            case lapCount:
                if (currentItem->whatsThis() == "LAP_COUNT")
                    return i;
                break;
            case distance:
                if (currentItem->whatsThis() == "DISTANCE")
                    return i;
                break;
            case bestTime:
                if (currentItem->whatsThis() == "BEST_TIME")
                    return i;
                break;
            case worstTime:
                if (currentItem->whatsThis() == "WORST_TIME")
                    return i;
                break;
            case lastTime:
                if (currentItem->whatsThis() == "LAST_TIME")
                    return i;
                break;
            default:
                break;
        }
    }

    // No item found
    return -1;
}
