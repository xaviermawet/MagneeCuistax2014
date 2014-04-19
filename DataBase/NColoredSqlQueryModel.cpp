#include "NColoredSqlQueryModel.hpp"

NColoredSqlQueryModel::NColoredSqlQueryModel(QObject *parent) :
    NSqlQueryModel(parent), _or(255, 215, 0), _argent(206, 206, 206),
    _bronze(97, 85, 52), _green(170, 222, 190)
{
}

QVariant NColoredSqlQueryModel::data(const QModelIndex &item, int role) const
{
    if (!item.isValid())
        return QVariant();

    if (role == Qt::BackgroundRole)
    {
        if (item.row() == 0)
            return this->_or;
        if (item.row() == 1)
            return this->_argent;
        if (item.row() == 2)
            return this->_bronze;
//        if (item.column() == 2)
//            return this->_green;
    }

    return NSqlQueryModel::data(item, role);
}
