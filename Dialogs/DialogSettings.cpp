#include "DialogSettings.hpp"
#include "ui_DialogSettings.h"

DialogSettings::DialogSettings(QWidget* parent) :
    QDialog(parent), ui(new Ui::DialogSettings)
{
    // GUI Configuration
    this->ui->setupUi(this);

    // Manage changes
    connect(this->ui->buttonBox, SIGNAL(clicked(QAbstractButton*)),
            this, SLOT(applyChanges(QAbstractButton*)));
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

void DialogSettings::applyChanges(QAbstractButton* buttonApply)
{
    // Button "Apply" not pressed
    if (buttonApply != this->ui->buttonBox->button(QDialogButtonBox::Apply))
        return;

    qDebug() << "Apply changes ...TODO";
}
