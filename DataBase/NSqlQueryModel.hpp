#ifndef __NSQLQUERYMODEL_HPP__
#define __NSQLQUERYMODEL_HPP__

#include <QSqlQueryModel>
#include <QSqlQuery>

class NSqlQueryModel : public QSqlQueryModel
{
    public:

        explicit NSqlQueryModel(QObject* parent = 0);

        void refresh(void);
};

#endif /* __NSQLQUERYMODEL_HPP__ */
