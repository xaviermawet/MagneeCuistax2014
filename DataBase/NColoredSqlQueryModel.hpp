#ifndef __NCOLOREDSQLQUERYMODEL_HPP__
#define __NCOLOREDSQLQUERYMODEL_HPP__

#include <QFont>
#include <QColor>
#include "NSqlQueryModel.hpp"

class NColoredSqlQueryModel : public NSqlQueryModel
{
        Q_OBJECT

    public:

        explicit NColoredSqlQueryModel(QObject* parent = 0);

        virtual QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;

    signals:

    public slots:

    protected:

        QColor _or;
        QColor _argent;
        QColor _bronze;

        QColor _green;
};

#endif /* __NCOLOREDSQLQUERYMODEL_HPP__ */
