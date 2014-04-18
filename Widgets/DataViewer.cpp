#include "DataViewer.hpp"
#include "ui_DataViewer.h"

DataViewer::DataViewer(QWidget* parent, Qt::WindowFlags flag) :
    QWidget(parent, flag), ui(new Ui::DataViewer), _stopWatch(NULL)
{
    // GUI Configuration
    this->ui->setupUi(this);

    // Populate header
    this->createHeader();
}

DataViewer::~DataViewer(void)
{
    // Widgets
    delete this->ui;
    delete this->_stopWatch;
}

void DataViewer::createHeader(void)
{
    if (this->_stopWatch != NULL)
        delete this->_stopWatch;

    // Create stopwatch
    this->_stopWatch = new NStopWatch(this);

    // Add the stopwatch in header
    this->ui->horizontalLayoutHeader->addWidget(this->_stopWatch);
}
