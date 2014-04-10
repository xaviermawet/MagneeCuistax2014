#ifndef __NTABLEWIDGET_HPP__
#define __NTABLEWIDGET_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

class NTableWidget : public QTableWidget
{
        Q_OBJECT

    public:

        enum RowMaxOption {InfinityRows = -1};

        /*!
         * \brief NTableWidget : default constructor
         * \param parent : pointer to a parent widget if exists
         */
        explicit NTableWidget(QWidget* parent = 0);

        void insertRowItems(int row, QVariantList const& items);

        void setMaxRow(int maxRowCount = NTableWidget::InfinityRows);
        int maxRow(void) const;

    signals:

    public slots:

    protected:

        int _maxRow;
};

#endif /* __NTABLEWIDGET_HPP__ */
