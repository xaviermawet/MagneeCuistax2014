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

void DialogSettings::applyChanges(QAbstractButton* buttonApply)
{
    // Button "Apply" not pressed
    if (buttonApply != this->ui->buttonBox->button(QDialogButtonBox::Apply))
        return;

    qDebug() << "Apply changes ...TODO";
}
