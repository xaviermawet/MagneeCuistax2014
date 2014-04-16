#include "NSqlQueryModel.hpp"

NSqlQueryModel::NSqlQueryModel(QObject* parent) :
    QSqlQueryModel(parent)
{
    // Nothing to do here
}

void NSqlQueryModel::refresh(void)
{
    this->setQuery(this->query().executedQuery());

    QModelIndex topLeft = this->index(0, 0);
    QModelIndex bottomRight = this->index(
                this->rowCount() - 1, this->columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);
}
