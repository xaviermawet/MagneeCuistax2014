#include "NTableView.hpp"

NTableView::NTableView(QWidget* parent) :
    QTableView(parent)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    this->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
}
