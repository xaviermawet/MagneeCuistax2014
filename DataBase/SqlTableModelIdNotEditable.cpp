#include "SqlTableModelIdNotEditable.hpp"

SqlTableModelIdNotEditable::SqlTableModelIdNotEditable(
        QObject* parent, QSqlDatabase db):
    QSqlTableModel(parent, db)
{
    // Nothing to do here ...
}

Qt::ItemFlags SqlTableModelIdNotEditable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlTableModel::flags(index);
    if (index.column() == 0)
        flags &= ~Qt::ItemIsEditable;
    return flags;
}
