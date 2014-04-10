#include "NTableWidget.hpp"

NTableWidget::NTableWidget(QWidget* parent) :
    QTableWidget(parent)
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
}
