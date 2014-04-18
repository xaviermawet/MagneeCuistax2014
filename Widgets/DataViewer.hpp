#ifndef __DATAVIEWER_HPP__
#define __DATAVIEWER_HPP__

#include <QtGlobal>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

// Personal Widgets
#include "Widgets/NStopWatch.hpp"

namespace Ui {
    class DataViewer;
}

class DataViewer : public QWidget
{
        Q_OBJECT

    public:

        /*!
         * \brief DataViewer : Default constructor
         * \param parent : Pointer to a parent widget if exists
         * \param flag : sets the window flags
         */
        explicit DataViewer(QWidget* parent = 0,
                            Qt::WindowFlags flag = Qt::Window);

        /*!
         * \brief ~DataViewer : Virtual destructor
         */
        virtual ~DataViewer(void);

    protected:

        void createHeader(void);

    protected:

        // Widgets
        Ui::DataViewer* ui;
        NStopWatch*     _stopWatch;
};

#endif /* __DATAVIEWER_HPP__ */
