#ifndef __SQLTABLEMODELIDNOTEDITABLE_HPP__
#define __SQLTABLEMODELIDNOTEDITABLE_HPP__

#include <QSqlTableModel>

class SqlTableModelIdNotEditable : public QSqlTableModel
{
    public:

        explicit SqlTableModelIdNotEditable(
                QObject* parent = 0, QSqlDatabase db = QSqlDatabase());

        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif /* __SQLTABLEMODELIDNOTEDITABLE_HPP__ */
