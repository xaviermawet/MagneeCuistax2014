#include "NColoredSqlQueryModel.hpp"

NColoredSqlQueryModel::NColoredSqlQueryModel(QObject *parent) :
    NSqlQueryModel(parent)
{
}

QVariant NColoredSqlQueryModel::data(const QModelIndex &item, int role) const
{
    if (!item.isValid())
        return QVariant();

    switch (role)
    {
        case Qt::FontRole:
            if (item.row() == 0)
            {
                QFont boldFont;
                boldFont.setBold(true);
                return boldFont;
            }
            break;
        case Qt::BackgroundRole:
            if (item.column() == 2 || item.row() == 0)
                return QColor(170, 222, 190);
        default:
            break;
    }

    return NSqlQueryModel::data(item, role);
}
