#include "NSqlQueryModel.hpp"

NSqlQueryModel::NSqlQueryModel(QObject* parent) :
    QSqlQueryModel(parent)
{
    // Nothing to do here
}

void NSqlQueryModel::refresh(void)
{
    this->setQuery(this->query().executedQuery());
}
