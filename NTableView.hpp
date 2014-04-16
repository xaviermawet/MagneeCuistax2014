#ifndef __NTABLEVIEW_HPP__
#define __NTABLEVIEW_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

class NTableView : public QTableView
{
        Q_OBJECT

    public:

        /*!
         * \brief NTableView : default constructor
         * \param parent : pointer to a parent widget if exists
         */
        explicit NTableView(QWidget* parent = 0);

    signals:

    public slots:

};

#endif /* __NTABLEVIEW_HPP__ */
