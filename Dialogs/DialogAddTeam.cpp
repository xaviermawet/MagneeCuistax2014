#include "DialogAddTeam.hpp"
#include "ui_DialogAddTeam.h"

DialogAddTeam::DialogAddTeam(QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogAddTeam)
{
    this->ui->setupUi(this);
}

DialogAddTeam::~DialogAddTeam(void)
{
    delete this->ui;
}
