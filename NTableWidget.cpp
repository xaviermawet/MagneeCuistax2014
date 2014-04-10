#include "NTableWidget.hpp"

NTableWidget::NTableWidget(QWidget* parent) :
    QTableWidget(parent), _maxRow(-1)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
}

void NTableWidget::insertRowItems(int row, QVariantList const& items)
{
    if (items.isEmpty())
        return;

    // Inserts new empty row
    this->insertRow(row);

    // Populates the new row
    for(int column(0); column < items.count(); column++)
            this->setItem(row, column, new QTableWidgetItem(
                              items.at(column).toString()));

    if (this->rowCount() > this->_maxRow)
        this->removeRow(this->_maxRow);
}

void NTableWidget::setMaxRow(int maxRowCount)
{
    this->_maxRow = maxRowCount;

    // resize the table if there is currently more rows that _maxRow
    if (this->rowCount() > this->_maxRow)
        this->setRowCount(this->_maxRow);
}

int NTableWidget::maxRow(void) const
{
    return this->_maxRow;
}

