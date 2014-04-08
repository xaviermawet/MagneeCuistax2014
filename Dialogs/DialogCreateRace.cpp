#include "DialogCreateRace.hpp"
#include "ui_DialogCreateRace.h"

DialogCreateRace::DialogCreateRace(QWidget* parent) :
    QDialog(parent), ui(new Ui::DialogCreateRace)
{
    this->ui->setupUi(this);

    // Custom calendar
    this->ui->calendarWidgetDateRace->setMinimumDate(QDate::currentDate());
}

DialogCreateRace::~DialogCreateRace(void)
{
    delete this->ui;
}
