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

void DataViewer::setTableViewModel(QAbstractItemModel* model)
{
    if (model == NULL)
        return;

    this->ui->tableViewRanking->setModel(model);
}

void DataViewer::setRaceTitle(const QString &title)
{
    this->ui->labelTitle->setText(title);
}

DataViewer::~DataViewer(void)
{
    // Widgets
    delete this->ui;
    delete this->_stopWatch;
}

void DataViewer::startStopWatch(void)
{
    this->_stopWatch->start();
}

void DataViewer::stopStopWatch(void)
{
    this->_stopWatch->stop();
}

void DataViewer::resetStopWatch(void)
{
    this->_stopWatch->reset();
}

void DataViewer::createHeader(void)
{
    if (this->_stopWatch != NULL)
        delete this->_stopWatch;

    // Create stopwatch
    this->_stopWatch = new NStopWatch(this);

    // Hide buttons
    this->_stopWatch->setButtonsVisible(false);

    // Add the stopwatch in header
    this->ui->horizontalLayoutHeader->addWidget(this->_stopWatch);
}
